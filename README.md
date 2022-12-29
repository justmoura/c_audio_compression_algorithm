# Audio Compression Algorithm

This code implements an audio compression algorithm that reduces the resolution of an audio signal by averaging together consecutive samples. It can be used to compress audio files and reduce their file size.

## Requirements

- GCC or a similar C compiler
- LAME MP3 encoder (optional, needed for MP3 support)

## Compiling the code

To compile the code, run the following command:

gcc compress.c -o compress


This will create an executable program called "compress".

To include support for MP3 files, you will need to install the LAME MP3 encoder on your system and include the necessary header files in the code. You will also need to link against the LAME library when compiling the code. For example:

gcc compress.c -o compress -lmp3lame


## Running the program

To run the program, use the following command:

./compress input_file output_file


Replace "input_file" with the name of the input audio file, and "output_file" with the name of the output file. The input file can be in any format supported by LAME (such as WAV or MP3), and the output file will be a compressed version of the input file in the same format.

## Adjusting the block size

The block size is a parameter that determines the number of samples that are averaged together to create a single output sample. It can be adjusted to trade off between compression rate and audio quality.

To adjust the block size, modify the following line in the code:

#define BLOCK_SIZE 8


Change the value of 8 to the desired block size. Larger block sizes will result in higher compression rates, but will also result in lower audio quality. Conversely, smaller block sizes will result in lower compression rates, but will also result in higher audio quality.
