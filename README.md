# SoundProc

SoundProc is a lightweight audio processing utility written in C. It provides a foundation for reading, processing, and writing WAV files, with a core implementation of digital signal processing (DSP) algorithms.

## What it Implements

### 1. WAV File Handling
The program includes a custom parser for the RIFF/WAV file format. It can:
- Read and parse WAV headers.
- Extract raw PCM audio data and normalize it to floating-point values (-1.0 to 1.0).
- Re-encode processed audio data back into a valid WAV file.

### 2. Fast Fourier Transform (FFT)
The **FFT** is an efficient algorithm to compute the Discrete Fourier Transform (DFT). It transforms a signal from its original domain (often time) to a representation in the frequency domain.
- **Implementation**: This project uses the recursive **Cooley-Tukey algorithm**, which follows a "divide and conquer" approach to reduce the complexity of the DFT from $O(N^2)$ to $O(N \log N)$.
- **Purpose**: Moving to the frequency domain allows for advanced processing like spectral analysis, noise reduction, and equalization.

### 3. Inverse Fast Fourier Transform (IFFT)
The **IFFT** performs the reverse operation of the FFT. It converts the frequency domain representation back into the time domain (audio samples). This is essential for hearing the result of any frequency-based modifications.

---

## How it was Made
SoundProc was developed using standard C (C11) with minimal dependencies. The core math utilizes the `math.h` library for trigonometric functions. 

The architecture is designed to be modular:
- **Structs**: Custom `WavHeader` and `Complex` types for clean data management.
- **Normalization**: Converts integer-based audio samples (16-bit PCM) into doubles for higher precision during processing.
- **Clamping**: Includes safety logic to prevent audio clipping by clamping values between -1.0 and 1.0 before writing back to disk.

---

## Usage

### Compilation
To compile the program, use a C compiler (like GCC):
```bash
gcc skeleton.c -o skeleton -lm
```

### Running
```bash
./skeleton <input_file.wav>
```
The program will read the input, process the samples, and generate an `output.wav`.

---

## Upcoming Updates

We are continuously working to expand SoundProc's capabilities:

### 1. Advanced Post-Processing
- **File Format Support**: While currently focused on `.wav` files, we are expanding support to include other common audio formats (like `.aiff`, `.flac`, etc.) to provide a more versatile post-processing toolkit.
- **Performance Optimizations**: Replacing the recursive FFT with an iterative version to reduce stack overhead and improve speed.
- **Bit-reversal Permutation**: Pre-computed bit-reversal tables for faster frequency bin sorting.

### 2. Audio Effects
We plan to implement several classic DSP effects:
- **Delay & Echo**: Time-delayed signal mixing and feedback-based decay.
- **Reverb**: Environmental acoustic simulation.
- **Filters**: Low-pass, High-pass, and Band-pass filters.
- **Normalization/Gain**: Dynamic range adjustment.

### 3. Real-time Sound Processing
- **Live Input/Output**: Moving beyond file-to-file processing to handle live audio streams.
- **Audio Library Integration**: Utilizing libraries like PortAudio or SDL for real-time sound manipulation and playback.
