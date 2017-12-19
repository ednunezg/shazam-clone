#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "../common/globals.h"
#include "../common/gpu_helpers.h"
#include "../common/cpu_helpers.h"
#include "../common/audiodatabase.h"

using namespace std;

int main(int argc, char *argv[])
{   
    /* STEP 0: Decide whether to run in CUDA mode or not */
    int CUDA_MODE = 0;
    if(argc==2 && (strcmp(argv[1], "-cuda") == 0)) {
        CUDA_MODE = 1;
    }
    
    /* STEP 1: Request the name of the input file */

    char inputFilePath[100];
    cout << "Enter the path of the audio file you want to identify: ";
    cin.getline(inputFilePath, sizeof inputFilePath);
    cout << endl;

    /* STEP 2: Get the array of audio samples. Each sample is saved into a Complex struct with imaginary component of 0 */

    Complex * audio;
    int audioSize, numChunks;
    wavToComplex(inputFilePath , &audio, &audioSize);
    numChunks = audioSize / CHUNK_SAMPLES;

    // Print back the int array of audio samples (for debugging)
    // for(int i=0; i<audioSize; i++){ cout << audio[i].re << endl; }

    /* STEP 3: Compute the hashes for every single chunk of 1024 samples */

    unsigned long * hashes;
    if(CUDA_MODE==0){
        audioToHashes(audio, numChunks, &hashes);
    } 
    else{
        audioToHashes_CUDA(audio, numChunks, &hashes);
    }
    /* STEP 4: Using the hashes we computed, we try to find a matching song from a DATABASE*/

    DB* database = new DB();
    database->initFromFile((char *) "../../database/HASHES.txt", (char *) "../../database/FILENAMES.txt");
    database->getBestMatchingSong( numChunks, hashes);

    return 0;
}
