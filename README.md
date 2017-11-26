SHAZAM CLONE
=================

For now, this is a staring point for creating a succesful implementation of a shazam clone. A shazam clone needs is made up of two programs. This is kindof a high level description of what to accomplish:

(1) footprint-songs: This program takes a massive library of WAV files as an input, and stores a hash table into a database of some sort.

(2) identify-song: This takes a WAV file as an input, and then matches the song with a list of songs in the database.

The most useful resource for coding these two is taking a look at this tutorial: http://royvanrijn.com/blog/2010/06/creating-shazam-in-java/. Essentially, the same needs to be done in C++. Then, we find some CUDA optimizations to it.

Pre-requisites
------------
Intsall the lsndfile library. This is needed to read a .wav file into an array of integers

To compile the identify-songs program
------------------------------------------
g++ main.cpp helpers.cpp -lsndfile