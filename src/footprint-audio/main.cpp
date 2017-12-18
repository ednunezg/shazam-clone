#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "../common/globals.h"
// #include "../common/gpu_helpers.h"
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

    /* STEP 1: Request the folder containing .wav files */
    char inputDirPath[100];
    cout << "Enter the name of folder containing WAVs to footprint: ";
    cin.getline(inputDirPath, sizeof inputDirPath);
    cout << endl;

    /* STEP 2: Build the database */
    DB* database = new DB();
    database->initFromScratch(inputDirPath, CUDA_MODE);

    /* STEP 3: Serialize the database into text files */
    database->serializeToFiles((char *) "../../database/HASHES.txt", (char *) "../../database/FILENAMES.txt");

    return 0;
}
