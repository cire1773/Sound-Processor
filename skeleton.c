#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#define PI 3.14159265358979323846

#pragma pack(push, 1)

typedef struct {
    char chunkId[4];
    uint32_t chunkSize;
    char format[4];
    char subChunkId1[4];
    uint32_t subChunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char subChunk2Id[4];
    uint32_t subChunk2Size;
} WavHeader;

#pragma pack(pop)

typedef struct{
    double real;
    double imag;
} Complex;

WavHeader readHeader(FILE *fp){
    WavHeader head;
    fread(&head, sizeof(WavHeader), 1, fp);
    while(strncmp(head.subChunk2Id, "data", 4) != 0){
        fseek(fp, head.subChunk2Size, SEEK_CUR);
        fread(head.subChunk2Id, 1, 4, fp);
        fread(&head.subChunk2Size, 4, 1, fp);
    }
    return head;
}

double* readAudioData(FILE *fp, WavHeader header, uint32_t *numSamples){
    *numSamples = header.subChunk2Size / (header.bitsPerSample / 8);
    int16_t *array_int16 = malloc(*numSamples * sizeof(int16_t));
    fread(array_int16, sizeof(int16_t), *numSamples, fp);
    double *array_double = malloc(*numSamples * sizeof(double));
    for(uint32_t i = 0; i < *numSamples; i++){
        array_double[i] = (double)array_int16[i] / 32768.0;
    }
    free(array_int16);
    return array_double;
}

void writeAudioData(const char *filename, WavHeader header, double *audioData, uint32_t numSamples){
    FILE *fp = fopen(filename, "wb");
    if(fp == NULL){
        printf("Error: The file %s can't be opened", filename);
        return;
    }
    header.subChunk2Size = numSamples * (header.bitsPerSample / 8);
    header.chunkSize = 36 + header.subChunk2Size;
    fwrite(&header, sizeof(WavHeader), 1, fp);
    int16_t *array_int16 = malloc(numSamples * sizeof(int16_t));
    for(uint32_t i = 0; i < numSamples; i++){
        if(audioData[i] < -1.0)
            audioData[i] = -1.0;
        if(audioData[i] > 1.0)
            audioData[i] = 1.0;
        array_int16[i] = (int16_t)(audioData[i] * 32767.0);
    }
    fwrite(array_int16, sizeof(int16_t), numSamples, fp);
    free(array_int16);
    fclose(fp);
}

Complex add(Complex a, Complex b){
    Complex c;
    c.real = a.real + b.real;
    c.imag = a.imag + b.imag;
    return c;
}

Complex sub(Complex a, Complex b){
    Complex c;
    c.real = a.real - b.real;
    c.imag = a.imag - b.imag;
    return c;
}

Complex multiply(Complex a, Complex b){
    Complex c;
    c.real = (a.real * b.real) - (a.imag * b.imag);
    c.imag = (a.real * b.imag) + (a.imag * b.real);
    return c;
}

void fft(Complex *X, int N){
    if(N <= 1)
        return;
    Complex *even = malloc((N / 2) * sizeof(Complex));
    Complex *odd = malloc((N / 2) * sizeof(Complex));
    for(int i = 0; i <= (N/2) - 1; i++){
        even[i] = X[i * 2];
        odd[i] = X[i * 2 + 1];
    }
    fft(even, N / 2);
    fft(odd, N / 2);
    for(int k = 0; k <= N / 2 - 1; k++){
        double teta = (-2 * PI * k) / N;
        Complex W;
        W.real = cos(teta);
        W.imag = sin(teta);
        Complex t = multiply(W, odd[k]);
        X[k] = add(even[k], t);
        X[k + N / 2] = sub(even[k], t);
    }
    free(even);
    free(odd);
}





int main(int argc, char *argv[]){

    if(argc != 2){
        printf("! Error !\n");
        printf("The command should be: ./skeleton <audio.wav>");
        return 1;
    }
    else{
        FILE *fp = fopen(argv[1], "rb");
        if(fp == NULL){
            printf("Error: The file %s can't be opened", argv[1]);
            return 1;
        }
        WavHeader header = readHeader(fp);
        uint32_t numSamples = 0;
        double *audio_data = readAudioData(fp, header, &numSamples);
        printf("We have %u samples\n", numSamples);
        writeAudioData("output.wav", header, audio_data, numSamples);
        free(audio_data);
        fclose(fp);
    }
    
    return 0;
}