#pragma once

#include <stdlib.h>
#include <dirent.h>
#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <fstream>
#include <sstream>
#include "../common/globals.h"


using namespace std;

class DB{

  private:

  public:

    unordered_map< unsigned long, list<DataPoint> > hashmap; //Maps a hash (long) --> list of matching datapoints
    vector< char * > filenames; //Index of vector is file_id. Slot of vector is filename
    int numFiles;


    /* Constructor */
    DB(){
      numFiles = 0;
    }

    /*
    dbInitFromScratch
    ====================
    Initializes database by scanning a directory containing WAV files.
    For each of the wav files, we perform FFTs, find dominant frequencies,
    and hash dominant frequencies for each data point

    @param - folderPath: string containing input file for WAV

    @return - 0 for success, 1 for error
    */
    int initFromScratch(char * folderPath){

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

      //FAKE DATA FOR NOW....

      list<DataPoint> l1;
      list<DataPoint> l2;
      list<DataPoint> l3;

      DataPoint d1 = {1,0};
      DataPoint d2 = {1,0};
      DataPoint d3 = {1,0};
      DataPoint d4 = {2,0};
      DataPoint d5 = {2,0};
      DataPoint d6 = {3,0};
      DataPoint d7 = {4,0};
      DataPoint d8 = {5,0};

      l1.push_back(d1); l1.push_back(d2); l1.push_back(d3); l1.push_back(d4);
      l2.push_back(d5); l2.push_back(d6); l2.push_back(d7);
      l3.push_back(d8);
      
      hashmap[0x11112222] = l1;
      hashmap[0x22223333] = l2;
      hashmap[0x33334444] = l3;

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

    int serializeToFiles(char * outputHashesFile, char * outputFilenamesFile){
      
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
          hashFile << " ( " << dp.file_id << " " << dp.time << " )";
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
    int initFromFile(char * hashesFile, char * filenamesFile){

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
        stream << hex;
        stream >> hash;

        list<DataPoint> dpList;
        char openParentheses;
        char closeParentheses;
        unsigned int dataPointTime;
        unsigned int dataPointFileId;
        int i = 0;

        stream << dec;

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

};