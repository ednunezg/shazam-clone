// includes, system
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// includes, project
#include "/usr/local/cuda/include/cufft.h"
#include "globals.h"
#include "gpu_helpers.h"

__device__ unsigned long computeHash_CUDA(int f1, int f2, int f3, int f4){
    return 0x0;
}

__global__ void tranformToHashes_CUDA(Complex * input, int numChunks){
    return;
}

void audioToHashes_CUDA(Complex * input, int numChunks, unsigned long ** outputHashes){

    /* Step 1. Perform a batch of FFTs on the complex inputs */

        //Use cuFFT, according to StackOverflow forums, it beats the benchmarks of any other FFT library available for CUDA

    /* Step 2: Launch another CUDA kernel that computes a hash for each chunk of the transformed input */

        //Use transformToHashes here

    return;
}