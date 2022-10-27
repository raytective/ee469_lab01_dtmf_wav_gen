///////////////////////////////////////////////////////////////////////////////
///         University of Hawaii, College of Engineering
/// @brief  EE469_Lab1 - EE 469 - FALL 2022
///
/// @file main.c
/// @version 1.0
///
/// @author Rachel Watanabe <rkwatana@hawaii.edu>
/// @date   13_Oct_2022
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h> // For printf(), fopen(), etc.
#include <stdlib.h> // For EXIT_SUCCESS
#include <assert.h> // For assert()
#include <stdint.h> // For fixed-length ints
#include <math.h> // For sin()
#include <string.h> // For strlen()
#define PROGRAM_NAME "ee469_lab01_dtmf_wav_gen"
#define FILENAME "/tmp/tmp.dQGoG2Gjl6/blobby/blob.wav"
#define CHANNELS            1 /* 1 for mono 2 for stereo */
#define SAMPLE_RATE         8000 /* Samples per second */
#define BYTES_PER_SAMPLE    1 /* (BitsPerSample * Channels) / 8 *
                                * 1 - 8 bit mono *
                                * 2 - 8 bit stereo/16 bit mono *
                                * 4 - 16 bit stereo */
#define BITS_PER_SAMPLE     8 /* 256 possible values */
#define DURATION_IN_MS      2000 /* Duration in milliseconds */
#define SILENCE_IN_MS       150 /* The pause between each DTMF tone */
#define AMPLITUDE           0.1 /* Max amplitude of signal, relative *
 * to the maximum scale */
#define DTMF_ROW_1 697
#define DTMF_ROW_2 770
#define DTMF_ROW_3 852
#define DTMF_ROW_4 941
#define DTMF_COL_1 1209
#define DTMF_COL_2 1336
#define DTMF_COL_3 1477
#define DTMF_COL_4 1633

FILE *gFile = NULL; /* Global file pointer to FILENAME */
uint32_t gPCM_data_size = 0; /* Global counter of the bytes written */


double generate_tone( uint32_t index, uint32_t frequency ) {
    assert( frequency != 0 );
    double tone=0;
    double cadence = (2 * M_PI * frequency ) / ( SAMPLE_RATE );

    tone = sin( index / cadence );

    return tone;
}

double mix_tones( double f1, double f2 ) {
    return ( (f1 + f2) / (2.0) );   /// average the input samples
}

void open_audio_file() {
    assert(gFile == NULL);

    gFile = fopen(FILENAME, "w+");
    if (gFile == NULL) {
        printf(PROGRAM_NAME ": Could not open file [%s]. Exiting.\n", FILENAME);
        exit(EXIT_FAILURE);
    }

    assert(gFile != NULL);

    gPCM_data_size = 8000;

    /// Marks file as a RIFF file
    fwrite("RIFF", 1, 4, gFile); /// @todo Add error checking
    uint32_t file_size = 44 + gPCM_data_size; // 44 is the actual size of the header
                                              // (regardless of what you see below)
                                              // This gets overwritten later
    fwrite(&file_size, 1, 4, gFile); /// File size

    fwrite("WAVE", 1, 4, gFile); /// File type header

    fwrite("fmt ", 1, 4, gFile); /// Format chunk (includes trailing space)

    uint32_t format_length = 16;
    fwrite(&format_length, 1,4, gFile);      /// Length of format data

    uint16_t format_type = 1;
    fwrite(&format_type, 1, 2, gFile);      /// Type of format

    uint16_t channels = CHANNELS;
    fwrite(&channels, 1, 2, gFile);      /// Number of channels

    uint32_t sample_rate = SAMPLE_RATE;
    fwrite(&sample_rate, 1, 4, gFile);  /// Sample rate

    uint32_t yes = (SAMPLE_RATE * BITS_PER_SAMPLE * CHANNELS) / 8;
    fwrite(&yes, 1, 4, gFile); /// oh god

    uint16_t bytes_per_sample = (BITS_PER_SAMPLE * CHANNELS) / 8;
    fwrite(&bytes_per_sample, 1, 2, gFile);      /// BitsPerSample * Channels

    uint16_t bits_per_sample = BITS_PER_SAMPLE;
    fwrite(&bits_per_sample, 1, 2, gFile);     /// Bits per sample

    fwrite("data", 1, 4, gFile); /// "data" chunk header (ok)

    file_size = 44 + gPCM_data_size;
    fwrite(&file_size, 1, 4, gFile);   /// size of the data section


    unsigned char buffer[SAMPLE_RATE*1];

    uint32_t frequency = 900;
    for(uint32_t i = 0; i < SAMPLE_RATE*1; i++) {
        ///1-Sec silence
        //buffer[i] = 128;

        ///Sawtooth
       // buffer[i] = i % 256;

       ///Sin wave oh god
       buffer[i] = 50 * 128 * generate_tone(i, frequency) + 128;

        gPCM_data_size++;
        fwrite(&buffer[i], 1 , 1, gFile);
    }



    fclose(gFile);

}


int main() {
    printf("Hello world...... Gn..\n");

    open_audio_file();

    printf("Successful\n");

    return 0;
}