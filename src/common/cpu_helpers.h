#ifndef CPU_HELPERS_H
#define CPU_HELPERS_H

void wavToComplex(char * inputFilePath , Complex ** output, int * outputSize);

void FFT(Complex * v, int n, Complex *tmp);

unsigned long computeHash(int f1, int f2, int f3, int f4);

void audioToHashes(Complex * input, int numChunks, unsigned long ** outputHashes);

#endif