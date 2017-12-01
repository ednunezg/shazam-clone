#ifndef HELPERS_H_
#define HELPERS_H_

typedef struct{
    int imaginary;
    int real;
} Complex;

void wavToInts(char * inputFilePath , int ** output, int * outputSampleRate, int * outputSize);

void multipleAudioChunksToFFT(int * audioSamples, Complex **fftsOutput);

void audioChunkToFFT(int * audioSamples, int fromIndex, Complex *fftOutput);

void getHighestFrequencySet(Complex *frequencies, int ranges[], int *output);

void getAllHighestFrequencySets(Complex ** chunks, int ranges[] , char * outputFilePath);

long hash(std::string line);

#endif