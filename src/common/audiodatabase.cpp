#include <stdlib.h>
#include <dirent.h>
#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <fstream>
#include <sstream>
#include "../common/globals.h"
#include "../common/audiodatabase.h"
#include "../common/cpu_helpers.h"

using namespace std;


/* Constructor */
DB::DB(){
  numFiles = 0;
}

int DB::initFromScratch(char * folderPath){

  /* Step 1: For all .wavs in this folder, assign a filename to each file ID */

  DIR*    dir;
  dirent* pdir;
  vector<string> files;

  dir = opendir(folderPath);
  pdir = readdir(dir);

  int i=0;
  while (pdir){
    string filename = pdir->d_name;
    if(filename.find(".wav") != string::npos){
      filenames.push_back(pdir->d_name);
      i++;
    }
    pdir = readdir(dir);
  }
  numFiles = i;

  /* Step 2: Compute hashes and their respective datapoints */
  
  for(int i=0; i<filenames.size(); i++){
    
    char* filepath;
    filepath = (char *) malloc(strlen(folderPath)+strlen(filenames[i])+1); 
    strcpy(filepath, folderPath ); 
    strcat(filepath, filenames[i]);


    Complex * audio;
    int audioSize, numChunks;
    wavToComplex(filepath , &audio, &audioSize);
    numChunks = audioSize / CHUNK_SAMPLES;

    unsigned long * hashes;
    audioToHashes(audio, numChunks, &hashes); //Our goal is to find the most prominent frequency between 40-80 Hz, 80-120 Hz, .. 180-300 Hz, and compute the hash for these 4 freqs


    for(int j=0; j<numChunks; j++){

        DataPoint dp;
        dp.file_id = i;
        dp.time = j;
        unsigned long hash = hashes[j];

        hashmap[hash].push_back(dp);

    }
  }


  return 0;
}

/*
  serializeToFiles
  ====================
  Takes the contents of the database and serializes them to text files
  that can be used to initialize the database again at 

  @param - folderPath: string containing input file for WAV

  @return - 0 for success, 1 for error
*/

int DB::serializeToFiles(char * outputHashesFile, char * outputFilenamesFile){
  
  fstream filenamesFile(outputFilenamesFile, ios_base::out);
  
  for(int i=0; i<filenames.size(); i++){
    filenamesFile << i << " " << filenames[i] << endl;
  }

  fstream hashFile(outputHashesFile, ios_base::out);

  for (auto it : hashmap){
    long hash = it.first;
    
    list<DataPoint> dpList = hashmap[hash];

    hashFile << std::hex << hash;

    for(auto &i : dpList){
      DataPoint dp = i;
      hashFile << std::dec << " ( " << dp.file_id << " " << dp.time << " )";
    }
    hashFile << endl;
  }


  filenamesFile.close();
  hashFile.close();

  return 0;
    
}


/*
dbInitFromFile
====================
Initializes database from a previously serialized instance of a database.
This is useful so we don't have to perform FFTs and determine frequencies
from hours of audio all over again.

@param - hashesFile: file containing serialized hashmap
        - filenamesFile: file containing serialized fileIDs and filenames

@return - 0 for success, 1 for error
*/
int DB::initFromFile(char * hashesFile, char * filenamesFile){

  //Step 1: Read in the filenames and assign file IDs
  
  fstream fstreamFilenames(filenamesFile, ios_base::in);
  if (!fstreamFilenames) {
    cout << "Unable to open filenames file";
    exit(1); // terminate with error
  }

  int id;
  string fname;

  while ( fstreamFilenames >> id >> fname ){
    char * fnameCstr = new char[fname.length() + 1];
    strcpy(fnameCstr, fname.c_str());
    filenames.push_back(fnameCstr);
    numFiles++;
  }

  fstreamFilenames.close();

  //Step 2: Read in the hash table

  fstream fstreamHashes(hashesFile, ios_base::in);
  if (!fstreamHashes) {
    cout << "Unable to open hashes file";
    exit(1); // terminate with error
  }

  string line;
  while (getline( fstreamHashes, line))
  {
    stringstream stream(line);

    unsigned long hash;
    stream >> std::hex >> hash >> std::dec;


    list<DataPoint> dpList;
    char openParentheses;
    char closeParentheses;
    unsigned int dataPointTime;
    unsigned int dataPointFileId;
    int i = 0;


    while( stream >> openParentheses >> dataPointFileId >> dataPointTime >> closeParentheses){          
      if(openParentheses!='(' || closeParentheses!=')'){
        cout << "HASHES text file not formatted correctly at line: '" << line << "'" << endl;
        exit(1); // terminate with error
      }
      DataPoint * dp = new DataPoint;
      dp->file_id = dataPointFileId;
      dp->time = dataPointTime;
      
      dpList.push_back(*dp);
    }
    
    hashmap[hash] = dpList;
  }

  return 0;
}


int DB::getBestMatchingSong(int numHashes, unsigned long * hashes){

  //TODO: THIS IS A NAIVE IMPLEMENTATION FOR NOW... WE NEED TO ALSO CONSIDER TIME OFFSET
  
  //Compute histogram
  int * histogram = (int *) malloc(numFiles * sizeof(int));  //Histogram of what files match each audio chunk
  for(int i=0; i<numFiles; i++) histogram[i] = 0;
  int curOffset = 0;

  cout << "NUM HASHES = " << numHashes << endl;

  for(int i=0; i< numHashes; i++){
    unsigned long hash = hashes[i];
    list<DataPoint> datapoints = hashmap[hash];
    for(auto dp : datapoints){
      histogram[dp.file_id]++;
    }
    curOffset++;
  }

  //Get best song from the histogram
  int best = -1;
  int bestScore = 0;

  cout << "---HISTOGRAM RESULTS---" << endl;

  for(int i=0; i<numFiles; i++){

    cout << "FILE ID " << i << " HAS SCORE OF " << histogram[i] << endl;

    if(histogram[i] > bestScore){
      best = i;
      bestScore = histogram[i];
    }
  }

  cout << "The audiofile " << filenames[best] << " was the BEST matching song! 🙌 ";

  return best;
}