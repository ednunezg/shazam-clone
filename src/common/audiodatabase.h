#ifndef DATABASE_H
#define DATABASE_H

#include <unordered_map>
#include <list>
#include <vector>

using namespace std;

class DB{

  private:
    unordered_map< unsigned long, list<DataPoint> > hashmap; //Maps a hash (long) --> list of matching datapoints
    vector< char * > filenames; //Index of vector is file_id. Slot of vector is filename
    int numFiles;

  public:
    DB();

    int initFromScratch(char * folderPath);

    int serializeToFiles(char * outputHashesFile, char * outputFilenamesFile);

    int initFromFile(char * hashesFile, char * filenamesFile);

    int getBestMatchingSong(int numHashes, unsigned long * hashes);

};

#endif