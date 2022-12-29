#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <lame/lame.h>

#define BLOCK_SIZE 8 // number of samples to average together

// Structure to hold information about an audio file
typedef struct {
  int sample_rate; // sample rate in Hz
  int num_channels; // number of channels (1 for mono, 2 for stereo)
  int bits_per_sample; // bits per sample (8, 16, or 24)
  int num_samples; // total number of samples in the file
}
AudioFile;

// Function to read the header of an audio file
int read_header(FILE * file, AudioFile * audio) {
  // Read the sample rate
  if (fread( & audio -> sample_rate, sizeof(int), 1, file) != 1) {
    return 0;
  }

  // Read the number of channels
  if (fread( & audio -> num_channels, sizeof(int), 1, file) != 1) {
    return 0;
  }

  // Read the bits per sample
  if (fread( & audio -> bits_per_sample, sizeof(int), 1, file) != 1) {
    return 0;
  }

  // Read the number of samples
  if (fread( & audio -> num_samples, sizeof(int), 1, file) != 1) {
    return 0;
  }

  return 1;
}

// Function to write the header of an audio file
int write_header(FILE * file, AudioFile * audio) {
  // Write the sample rate
  if (fwrite( & audio -> sample_rate, sizeof(int), 1, file) != 1) {
    return 0;
  }

  // Write the number of channels
  if (fwrite( & audio -> num_channels, sizeof(int), 1, file) != 1) {
    return 0;
  }

  // Write the bits per sample
  if (fwrite( & audio -> bits_per_sample, sizeof(int), 1, file) != 1) {
    return 0;
  }

  // Write the number of samples
  if (fwrite( & audio -> num_samples, sizeof(int), 1, file) != 1) {
    return 0;
  }

  return 1;
}

int main(int argc, char * argv[]) {
  // Check for correct number of arguments
  if (argc != 3) {
    printf("Usage: compress input_file output_file\n");
    return 1;
  }

  // Initialize LAME
  lame_global_flags
    *
    gfp = lame_init();
  if (!gfp) {
    printf("Error initializing LAME\n");
    return 1;
  }

  // Open the input and output files
  FILE * input_file = fopen(argv[1], "r");
  FILE * output_file = fopen(argv[2], "w");
  if (!input_file || !output_file) {
    printf("Error opening file\n");
    return 1;
  }

  // Read the header of the input file
  AudioFile audio;
  if (!read_header(input_file, & audio)) {
    printf("Error reading header\n");
    return 1;
  }

  // Set the LAME parameters
  lame_set_in_samplerate(gfp, audio.sample_rate);
  lame_set_out_samplerate(gfp, audio.sample_rate);
  lame_set_num_channels(gfp, audio.num_channels);
  lame_set_quality(gfp, 2); // set quality to medium
  if (lame_init_params(gfp) < 0) {
    printf("Error setting LAME parameters\n");
    return 1;
  }

  // Determine the size of each sample in bytes
  int sample_size = audio.bits_per_sample / 8;

  // Read and compress the samples
  int sum;
  int num_samples;
  while (num_samples < audio.num_samples) {
    // Read a sample from the input file
    int sample;
    if (fread( & sample, sample_size, 1, input_file) != 1) {
      printf("Error reading sample\n");
      return 1;
    }
    sum += sample;
    num_samples++;

    if (num_samples == BLOCK_SIZE) {
      // Average the samples and write the result to the output file
      int avg = sum / BLOCK_SIZE;
      if (fwrite( & avg, sample_size, 1, output_file) != 1) {
        printf("Error writing sample\n");
        return 1;
      }
      sum = 0;
      num_samples = 0;
    }
  }

  // Write any remaining samples to the output file
  if (num_samples > 0) {
    int avg = sum / num_samples;
    if (fwrite( & avg, sample_size, 1, output_file) != 1) {
      printf("Error writing sample\n");
      return 1;
    }
  }

  // Close the files
  fclose(input_file);
  fclose(output_file);

  // Clean up LAME
  lame_close(gfp);

  return 0;
}
