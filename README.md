SHAZAM CLONE
=================

A shazam clone needs is made up of two programs:

(1) footprint-songs: This program takes a folder directory containing several WAV files as an input, and stores a [hash] -> [audio file name] mapping into a local database.

(2) identify-song: This takes a WAV file as an input, and then matches the song with a list of songs in the database.

Restrictions on all audio files used for this project:
------------------------------------------
WAV file must have:
- Sample rate of 22050 KHz
- Mono channel

Pre-requisites
---------------
Intsall the lsndfile library. This is needed to read a .wav file into an array of integers

Compile and run the identify-audio program (a makefile would be nice...)
----------------------------------------------------------------------------
make
./identify-audio [-cuda]

Compile and run the footprint-audio program (a makefile would be nice...)
----------------------------------------------------------------------------
make
./footprint-audio [-cuda]