#ifndef GLOBALS_H
#define GLOBALS_H


//STRUCTS
typedef struct{
    int imaginary;
    int real;
} Complex;

typedef struct{
  unsigned int time;
  unsigned int file_id;
} DataPoint;

//PROGRAM SETTINGS
#define SAMPLE_RATE 22050      //Any audio file that is not 22.05 KHz will not be accepted in this program

#define SIZE_OF_FFT_CHUNK 4096 //We will only process 4 kb of audio for each FFT. This will contain 2048 ints
#define SIZE_OF_FFT_CHUNK_SECONDS 0.09287981859 //An int is 2 bytes. A chunk contains 2048 ints. 2048/22050 is the total time for each chunk

#define FREQ_LOWERLIMIT = 40;
#define FREQ_UPPERLIMIT = 300;
const int FREQ_RANGES[5] = { 40, 80, 120, 180, 300 };


//CUDA SETTINGS
#define BLOCK_SIZE 256


#endif