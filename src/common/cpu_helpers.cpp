/* Helper functions for Audio Conversion, FFT, and Hashing */

#include <iostream>
#include <stdlib.h>
#include <sndfile.h>

#include "globals.h"
#include "audiodatabase.cpp"

using namespace std;


/*
    wavToInts
    ==========
    Encodes a WAV file into an array of integers

    @param - inputFilePath: string containing input file for WAV
           - outputSamples: pointer to a pointer of an int array (the function will malloc this int )
           - outputSamples: pointer to a pointer of an int array (the function will malloc this int )
    @return
*/

void wavToInts(char * inputFilePath , int ** output, int * outputSize){
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

void multipleFFTsToFrequencySetsFile(Complex ** chunks, char * outputFile){
  return;
}

void singleFFTToFrequencySet(Complex *frequencies, int *output){
  return;
}

unsigned long computeHash(int f1, int f2, int f3, int f4){
  //YET TO BE IMPLEMENTED
  const int FUZ_FACTOR = 2;

  return 0;
}

int getBestMatchingSong(DB * database, char * frequencySetsFilePath){

  //TODO: THIS IS A NAIVE IMPLEMENTATION FOR NOW... WE NEED TO ALSO CONSIDER TIME OFFSET

  fstream frequencySetsFile(frequencySetsFilePath, ios_base::in);
  if (!frequencySetsFile) {
    cout << "Unable to open file with frequency sets";
    exit(-1); // terminate with error
  }

  int f1, f2, f3, f4;
  
  //Compute histogram

  int * histogram = new int[database->numFiles];  //Histogram of what files match each audio chunk
  int curOffset = 0;

  while ( frequencySetsFile >> f1 >> f2 >> f3 >> f4 ){
    unsigned long hash = computeHash(f1,f2,f3,f4);
    list<DataPoint> datapoints = database->hashmap[hash];
    
    for(auto dp : datapoints){
      histogram[dp.file_id]++;
    }
    curOffset++;
  }

  //Get best song from the histogram

  int best = -1;
  for(int i=0; i<database->numFiles; i++){
    if(histogram[i] > best) best = histogram[i];
  }

  cout << "The audiofile " << database->filenames[best] << " was the BEST matching song! 🙌 ";

  return best;
}

