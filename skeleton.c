#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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
        printf("Sample Rate: %u Hz\n", header.sampleRate);
        printf("Marker data: %.4s\n", header.subChunk2Id);
        printf("Marime subChunk2: %u byti\n", header.subChunk2Size);
        uint32_t numSamples = 0;
        double *audio_data = readAudioData(fp, header, &numSamples);
        printf("We have %u samples\n", numSamples);
        writeAudioData("output.wav", header, audio_data, numSamples);
        free(audio_data);
        fclose(fp);
    }
    
    return 0;
}