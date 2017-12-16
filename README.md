SHAZAM CLONE
=================

A shazam clone needs is made up of two programs:

(1) footprint-songs: This program takes a folder directory containing several WAV files as an input, and stores a mapping of frequency hashes to the idea of the file and location.

(2) identify-song: This takes a WAV file as an input, and then matches the song with a list of songs in the database.

### Pre-requisites

Intsall the lsndfile library. This is needed to read a .wav file into an array of integers

All wav files you use for this project must have:
- Sample rate of 22050 KHz
- Mono channel

### Compile and run the identify-audio program

```
make
./identify-audio [-cuda]
```
When it prompts you for an audio file, enter: "test/bjork.wav" or any other audio you want to identify.


### Compile and run the footprint-audio program

```
make
./footprint-audio [-cuda]
```

When it prompts you for the path of a music library, enter: "../../music-wav/"