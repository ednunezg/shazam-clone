Shazam clone
=================

A shazam clone is made up of two programs:

**footprint-audio**: This program takes a folder directory containing several WAV files as an input, and stores a serialized database that can be used for later identifying any piece of audio using a FFT + frequency hashing technique. 

**identify-audio**: This takes a WAV file as an input, and then matches the song with a list of songs in a database.

### Pre-requisites

* Intsall the lsndfile library. This is needed to read a .wav file into an array of integers

* All wav files you use for this project must have:
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
