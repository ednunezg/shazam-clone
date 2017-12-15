#ifndef GLOBALS_H
#define GLOBALS_H


//STRUCTS
typedef struct{
    float im;
    float re;
} Complex;

typedef struct{
  unsigned int time;
  unsigned int file_id;
} DataPoint;

//MATH

#define PI_VAL  3.141592653589793

//PROGRAM SETTINGS

#define FREQ_LOWERLIMIT 40
#define FREQ_UPPERLIMIT 300

#define SAMPLE_RATE 22050      //Any audio file that is not 22.05 KHz will not be accepted in this program
#define CHUNK_SAMPLES  2048
#define CHUNK_BYTES CHUNK_SAMPLES*8   //We will only process 8 kb of audio for each FFT. This will contain 1024 complex numbers
#define CHUNK_SECONDS (float)CHUNK_SAMPLES/(float)SAMPLE_RATE //A Complex type is 8 bytes. A chunk contains 1024 Complex numbers. 1024/22050 is the total time for each chunk



//CUDA SETTINGS
#define BLOCK_SIZE 256


const int FREQ_RANGES[5] = { 40, 80, 120, 180, 300 };

#endif