#ifndef GPU_HELPERS_H
#define GPU_HELPERS_H

void audioToHashes_CUDA(Complex * input, int numChunks, unsigned long ** outputHashes);

#endif