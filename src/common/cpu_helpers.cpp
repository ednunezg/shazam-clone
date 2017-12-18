/* Helper functions for Audio Conversion, FFT, and Hashing */

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "AudioFile.h"
#include "globals.h"

using namespace std;


/*
    wavToComplex
    ==========
    Encodes a WAV file into an array of integers

    @param - inputFilePath: string containing input file for WAV
           - output: pointer to a pointer of an a Complex array (the function will malloc this Complex array )
           - outputSize: size of output complex array
    @return
*/

void wavToComplex(char * inputFilePath , Complex ** output, int * outputSize){
  

  /* Open the audio file */
  AudioFile<float> audioFile;
  audioFile.load (inputFilePath);

  /* Check if the settings are correct */
  bool isMono = audioFile.isMono();
  int sampleRate = audioFile.getSampleRate();

  if(sampleRate != 22050){
    cout << "ERROR: " << endl;
    cout << "The only sample rate supported for this program is 22.05 KHz." << endl;
    cout << "Please downsample / upsample '" << inputFilePath << "' to 22.05 KHz." << endl;
    exit(-1);
  }

  /* Write the data to the output array output array */
  int numSamples = audioFile.getNumSamplesPerChannel();
  int channel = 0;
  *output = (Complex *) malloc( sizeof(Complex) * numSamples);

  for (int i = 0; i < numSamples; i++) {
    (*output)[i].re = audioFile.samples[channel][i];
    (*output)[i].im = 0;
  }
  *outputSize = numSamples;
  return;
}

void FFT(Complex * v, int n, Complex *tmp){ // FROM https://www.math.wustl.edu/~victor/mfmm/fourier/fft.c
  if(n>1) {			/* otherwise, do nothing and return */
    int k,m;
    Complex z, w, *vo, *ve;
    ve = tmp; vo = tmp+n/2;
    for(k=0; k<n/2; k++) {
      ve[k] = v[2*k];
      vo[k] = v[2*k+1];
    }
    FFT( ve, n/2, v );		/* FFT on even-indexed elements of v[] */
    FFT( vo, n/2, v );		/* FFT on odd-indexed elements of v[] */
    for(m=0; m<n/2; m++) {
      w.re = cos(2*PI_VAL*m/(float)n);
      w.im = -sin(2*PI_VAL*m/(float)n);
      z.im = w.re * vo[m].im + w.im * vo[m].re;	/* Im(w*vo[m]) */
      z.re = w.re * vo[m].re - w.im * vo[m].im;	/* Re(w*vo[m]) */
      v[  m  ].re = ve[m].re + z.re;
      v[  m  ].im = ve[m].im + z.im;
      v[m+n/2].re = ve[m].re - z.re;
      v[m+n/2].im = ve[m].im - z.im;
    }
  }
  return;
}

unsigned long computeHash(int f1, int f2, int f3, int f4){
  const unsigned long FUZ_FACTOR = 3;

  if((f1 == -1 || f2==-1) || (f3==-1 || f4==-1)) return 0; //We return zero as the value of a hash in case of a silence
  unsigned long result = 0;
  result += (unsigned long) ((f4-(f4%FUZ_FACTOR)) * 100000000);
  result += (unsigned long) ((f3-(f3%FUZ_FACTOR)) * 100000);
  result += (unsigned long) ((f2-(f2%FUZ_FACTOR)) * 100);
  result += (unsigned long) (f1-(f1%FUZ_FACTOR));
  return result;
}

void audioToHashes(Complex * input, int numChunks, unsigned long ** outputHashes){ //All input elements have 0 for the im component
  
  //Process one chunk at a time
  *outputHashes = (unsigned long *) malloc( sizeof(unsigned long) * numChunks);

  for(int k=0; k<numChunks; k++){

    cout << "--- PROCESSING CHUNK # " << k <<  " --- " << "( " << CHUNK_SECONDS*k << " s ) " << endl;
    

    //STEP 1: FFT of audio
    Complex * chunk = input + (k*CHUNK_BYTES/8);
    Complex scratch[CHUNK_SAMPLES];
    FFT(chunk, CHUNK_SAMPLES, scratch);


    //STEP 2: Compute the most prominent frequencies for each of the 4 frequency ranges
    
    // int firstBin = 0;
    // int lastBin = 120;

    int firstBin = (CHUNK_SECONDS * FREQ_LOWERLIMIT)-2;
    int lastBin = (CHUNK_SECONDS  * FREQ_UPPERLIMIT)+2;
    int f1, f2, f3, f4;
    float f1val, f2val, f3val, f4val;

    f1 = -1; f2 = -1; f3 = -1 ; f4 = -1;
    f1val = 0; f2val = 0; f3val = 0 ; f4val = 0;

    for(int i=firstBin; i<lastBin; i++){
      int binFreq =  ((float) SAMPLE_RATE / (float) CHUNK_SAMPLES) * i;
      float binMag = sqrt( chunk[i].re * chunk[i].re  + chunk[i].im * chunk[i].im);
      cout << binFreq << " Hz --> " << binMag << endl;

      if((binFreq >= FREQ_RANGES[0] && binFreq < FREQ_RANGES[1]) && binMag > f1val ) { f1 = binFreq; f1val = binMag; }
      if((binFreq >= FREQ_RANGES[1] && binFreq < FREQ_RANGES[2]) && binMag > f2val ) { f2 = binFreq; f2val = binMag; }
      if((binFreq >= FREQ_RANGES[2] && binFreq < FREQ_RANGES[3]) && binMag > f3val ) { f3 = binFreq; f3val = binMag; }
      if((binFreq >= FREQ_RANGES[3] && binFreq < FREQ_RANGES[4]) && binMag > f4val ) { f4 = binFreq; f4val = binMag; }
    }

    //STEP 3: Hash all of the top 4 frequencies
    (*outputHashes)[k] = computeHash(f1, f2, f3, f4);
    
    cout << endl;
    cout << "Top frequencies= " << f1 << "," << f2 << "," << f3 << "," << f4 << endl;
    cout << "Hash= " << std::hex << "0x" << (*outputHashes)[k] << std::dec << endl;
    cout << endl;
  }

  return;
}