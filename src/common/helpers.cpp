/* Helper functions for Audio Conversion, FFT, and Hashing */

#include <iostream>
#include <stdlib.h>
#include <sndfile.h>

#define SIZE_OF_FFT_CHUNK 4096 //We will only process 4 kb of audio for each FFT

using namespace std;

typedef struct{
    int imaginary;
    int real;
} Complex;

/*
    Encodes a WAV file into an array of integers

    @param - inputFilePath: string containing input file for WAV
           - outputSamples: pointer to a pointer of an int array (the function will malloc this int )
           - outputSamples: pointer to a pointer of an int array (the function will malloc this int )
    @return
*/

void wavToInts(char * inputFilePath , int ** output, int * outputSampleRate, int * outputSize){
  SNDFILE *sf;
  SF_INFO info;
  int num_channels;
  int num, num_items;
  int *buf;
  int f,sr,c;
  FILE *out;
  
  /* Open the WAV file. */
  info.format = 0;
  sf = sf_open(inputFilePath, SFM_READ,&info);
  if (sf == NULL)
  {
    printf("Failed to open the file.\n");
    exit(-1);
  }
      
  /* Print some of the info, and figure out how much data to read. */

  f = info.frames;
  sr = info.samplerate;
  c = info.channels;
  printf("frames=%d\n",f);
  printf("samplerate=%d\n",sr);
  printf("channels=%d\n",c);
  num_items = f*c;
  printf("num_items=%d\n",num_items);
  
  /* Allocate space for the data to be read in a buffer, then read it. */
  /* TODO: Find a way to optimize this so we only read one channel */
  buf = (int *) malloc(num_items*sizeof(int));
  num = sf_read_int(sf,buf,num_items);
  sf_close(sf);
  printf("Read %d items\n",num);
  
  /* Write the data from the very first channel of buffer to the output array */
  *output = (int *) malloc( sizeof(int) * f);

  int j=0;
  for (int i = 0; i < num; i += c) {
    (*output)[j++] = buf[i];
  }
  *outputSampleRate = sr;
  *outputSize = f;

  free(buf);

  return;
}


void multipleAudioChunksToFFT(int * audioSamples, Complex **fftsOutput){
  return;
}

void audioChunkToFFT(int * audioSamples, int fromIndex, Complex *fftOutput){
  return;
}

void getHighestFrequencySet(Complex *frequencies, int ranges[], int *output){
  // const int DEFAULT_RANGES[] = new int[] {40, 80, 120, 180, 300}; //Change this to whatever we think is appropiate
  return;
}

void getAllHighestFrequencySets(Complex ** chunks, int ranges[] , char * outputFilePath){
  return;
}

long hash(std::string line){
  return 0;
}

