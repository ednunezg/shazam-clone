#include <iostream>
#include <stdlib.h>
#include <sndfile.h>

#include "helpers.h"

using namespace std;

int main()
{   

    /* STEP 1: Request the name of the input file */

    cout << "This program will read an input wav file and spit back the audio bytes" << endl << endl;
    char inputFilePath[100];
    cout << "Enter the name of audio file in the same directory as this program: ";
    cin.getline(inputFilePath, sizeof inputFilePath);
    cout << endl;

    /* STEP 2: Get the int array of audio samples */

    int * output;
    int outputSampleRate;
    int outputSize;
    wavToInts(inputFilePath , &output, &outputSampleRate, &outputSize);
    
    // Print back the int array of audio samples (for debugging)
    
    cout << "Printing array of ints..." << endl;

    for(int i=0; i<outputSize; i++){
        cout << output[i] << endl;
    }

    /* STEP 3: For each 4kb chunk of data in the audio int array, get a list of all frequencies and magnitudes */

            //Use multipleAudioChunksToFFT function here
    
    /* STEP 4: Make a file containing the most prominent frequencies in a range */

        //We will write to a file where each line will contain the most prominent frequency in "ranges[]"
        //In this example, our goal is to find the most prominent frequency between 40-80 Hz, 80-120 Hz, .. 180-300 Hz
        //We have to do this for EVERY SINGLE 4 KB chunk

    const int ranges[] = {40, 80, 120, 180, 300};

        //Use getAllHighestFrequencySets here

    /* STEP 5: Using the file containing the most prominent frequencies in the range, we compute hashes for each line and try to find a matching song from a DATABASE*/

        //Use hash function here, for every single line

    /* STEP 6: Success???? */

    return 0;
}
