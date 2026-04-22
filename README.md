# Procesor Audio si Analiza Semnalelor (C)

Acest proiect reprezinta o implementare de la zero a unui procesor de semnal audio in limbajul C. Scopul principal este citirea, manipularea si scrierea fisierelor audio format `.wav` (PCM, necomprimat), folosind Transformata Fourier Rapida (FFT) pentru trecerea din domeniul timpului in domeniul frecventei si invers (IFFT).

## Arhitectura si Componente Matematice

Codul este structurat in trei mari directii:

1. **I/O si Parsare Binara (WavHeader)**
   - Citirea corecta a blocului RIFF/WAVE.
   - Tratarea blocurilor de metadate suplimentare (ex. sari peste chunk-ul `LIST`), cautand strict blocul esential `data`.
   - Normalizarea esantioanelor pe 16-biti (`int16_t`) la valori cu virgula mobila (`double`) cuprinse intre -1.0 si 1.0, pentru a preveni overflow-ul in timpul calculelor algebrice.
   - Functia de scriere previne erorile prin "clipping" la reconversie.

2. **Aritmetica Numerelor Complexe**
   - Definirea structurii `Complex` pentru partea reala si imaginara.
   - Implementarea operatiilor de adunare, scadere si inmultire, respectand regula numerelor complexe (unde `i^2 = -1`).

3. **Procesarea Semnalului (FFT / IFFT)**
   - Implementarea algoritmului *Cooley-Tukey* (Divide et Impera), reducand complexitatea calcularilor de la `O(N^2)` la `O(N log N)`.
   - Calcularea factorilor de rotatie ("Twiddle Factors") pe cercul unitate, folosind identitatea lui Euler.
   - Transformata Inversa (IFFT) refoloseste logica recursiva, aplicand un sens de rotatie pozitiv si necesitand o scalare finala cu factorul `1/N`.

---

## Compilare si Rulare

Pentru a compila si testa cu succes codul, trebuie linkata biblioteca de matematica (`math.h`) cu flag-ul `-lm`.

**Compilare:**

```bash
gcc -Wall procesor.c -o procesor_audio -lm

**Rulare:**

```bash
./procesor_audio test.wav