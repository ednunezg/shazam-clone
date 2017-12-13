/* Helper functions for Audio Conversion, FFT, and Hashing */

#include <iostream>
#include <stdlib.h>
#include <sndfile.h>
#include <math.h>

#include "globals.h"
#include "audiodatabase.cpp"

using namespace std;


/*
    wavToInts
    ==========
    Encodes a WAV file into an array of integers

    @param - inputFilePath: string containing input file for WAV
           - output: pointer to a pointer of an a Complex array (the function will malloc this Complex array )
           - outputSize: size of output complex array
    @return
*/

void wavToComplex(char * inputFilePath , Complex ** output, int * outputSize){
  SNDFILE *sf;
  SF_INFO info;
  int num_channels;
  int num, num_items;
  float *buf;
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
  cout << "Reading " << inputFilePath << ":" << endl;
  cout << "frames=" << f << endl;
  cout << "samplerate=" << sr << endl;
  cout << "channels=" << c << endl;
  num_items = f*c;
  cout << "num_items=" << num_items << endl << endl;

  if(sr != SAMPLE_RATE){
    cout << "ERROR: " << endl;
    cout << "The only sample rate supported for this program is 22.05 KHz." << endl;
    cout << "Please downsample / upsample '" << inputFilePath << "' to 22.05 KHz." << endl;
    exit(-1);
  }
  
  /* Allocate space for the data to be read in a buffer, then read it. */
  /* TODO: Find a way to optimize this so we only read one channel */
  buf = (float *) malloc(num_items*sizeof(float));
  num = sf_read_float(sf,buf,num_items);
  sf_close(sf);
  printf("Read %d items\n",num);
  
  /* Write the data from the very first channel of buffer to the output array */
  *output = (Complex *) malloc( sizeof(Complex) * f);

  int j=0;
  for (int i = 0; i < num; i += c) {
    (*output)[j].re = buf[i];
    (*output)[j].im = 0;
    j++;
  }
  *outputSize = f;
  free(buf);
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
  const int FUZ_FACTOR = 2;
  return  (f4-(f4%FUZ_FACTOR)) * 100000000 + (f3-(f3%FUZ_FACTOR)) * 100000 + (f2-(f2%FUZ_FACTOR)) * 100 + (f1-(f1%FUZ_FACTOR));
}

void audioToHashes(Complex * input, int numChunks, unsigned long ** outputHashes){ //All input elements have 0 for the im component
  
  //Process one chunk at a time
  *outputHashes = (unsigned long *) malloc( sizeof(unsigned long) * numChunks);

  for(int k=0; k<numChunks; k++){

    cout << "--- PROCESSING CHUNK # " << k <<  " ---" << endl;
    

    //STEP 1: FFT of audio
    Complex * chunk = input + (k*CHUNK_BYTES/8);
    Complex scratch[CHUNK_SAMPLES];
    FFT(chunk, CHUNK_SAMPLES, scratch);


    //STEP 2: Compute the most prominent frequencies for each of the 4 frequency ranges
    
    int firstBin = 0;
    int lastBin = 30;
    // int firstBin = ( FREQ_LOWERLIMIT / (float) (1/CHUNK_SECONDS));
    // int lastBin = ( FREQ_UPPERLIMIT / (float) (1/CHUNK_SECONDS));
    int f1, f2, f3, f4;
    float f1val, f2val, f3val, f4val;

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
    cout << "Hash= " << (*outputHashes)[k] << endl;
    cout << endl;
  }

  return;
}


int getBestMatchingSong(DB * database, int hashesSize, unsigned long * hashes){

  //TODO: THIS IS A NAIVE IMPLEMENTATION FOR NOW... WE NEED TO ALSO CONSIDER TIME OFFSET
  
  //Compute histogram
  int * histogram = new int[database->numFiles];  //Histogram of what files match each audio chunk
  int curOffset = 0;

  for(int i=0; i< hashesSize; i++){
    unsigned long hash = hashes[i];
    list<DataPoint> datapoints = database->hashmap[hash];
    for(auto dp : datapoints){
      histogram[dp.file_id]++;
    }
    curOffset++;
  }

  //Get best song from the histogram
  int best = -1;
  int bestScore = 0;

  for(int i=0; i<database->numFiles; i++){
    if(histogram[i] > bestScore){
      best = i;
      bestScore = histogram[i];
    }
  }

  cout << "The audiofile " << database->filenames[best] << " was the BEST matching song! 🙌 ";

  return best;
}

