#include <iostream>
#include <stdlib.h>
#include <sndfile.h>

#include "../common/globals.h"
#include "../common/cpu_helpers.cpp"
#include "../common/audiodatabase.cpp"

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
    cout << "Enter the name of audio file you want to identify. It needs to be in same directory as this program: ";
    cin.getline(inputFilePath, sizeof inputFilePath);
    cout << endl;

    /* STEP 2: Get the int array of audio samples */

    int * output;
    int outputSize;
    wavToInts(inputFilePath , &output, &outputSize);
    
    // Print back the int array of audio samples (for debugging)
    cout << "Printing array of ints..." << endl;
    for(int i=0; i<outputSize; i++){ cout << output[i] << endl; }

    /* STEP 3: For each 4kb chunk of data in the audio int array, get a list of all frequencies and magnitudes */

        //Use multipleAudioChunksToFFT function here
    
    /* STEP 4: Make a file containing the most prominent frequencies in globals.h:FREQ_RANGES for each audio chunk */

        //We will write to a file where each line will contain the most prominent frequency in globals.h:FREQ_RANGES
        //In this example, our goal is to find the most prominent frequency between 40-80 Hz, 80-120 Hz, .. 180-300 Hz
        //We have to do this for EVERY SINGLE 4 KB chunk

        //Use getAllHighestFrequencySets here

    /* STEP 5: Using the file containing the most prominent frequencies in the range, we compute hashes for each line and try to find a matching song from a DATABASE*/
        
    DB* database = new DB();
    database->initFromFile((char *) "../../database/HASHES.txt", (char *) "../../database/FILENAMES.txt");

    int best = getBestMatchingSong( database, (char *) "FREQUENCY_SET_RESULTS.txt");

    return 0;
}
