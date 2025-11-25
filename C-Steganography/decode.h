#ifndef DECODE_H
#define DECODE_H

#include "types.h"

#define MAX_IMAGE_BUFFER_SIZE 8

typedef struct _DecodeInfo
{
    /* Stego image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    char image_data[MAX_IMAGE_BUFFER_SIZE];

    /* output file info */
    char *out_fname;
    char out_file_name[20];
    FILE *fptr_out;
    int out_file_extn_size;
    char out_file_extn[4];
    int out_file_size;

    /*magic string*/
    char original_magic_string[20];
    int magic_string_size;

}DecodeInfo;




/* Decoding functions prototype */

/*Read and validate Decode args from argv*/
Status read_and_validate_decode_args(char *argv[], DecodeInfo *deccInfo);

/* to perform Decoding*/
Status do_decoding(DecodeInfo *decInfo);

/*To open stego.bmp and output.txt files*/
Status open_files_decode(DecodeInfo *decInfo);

/*Function to skip the header of stego.bmp file*/
Status skip_bmp_header(FILE *fptr_stego);

/*Function to decode magic string  size*/
Status decode_magic_string_size(DecodeInfo *decInfo);

/*Function to decode magic string */
Status decode_magic_string(DecodeInfo *decInfo);

/* Function to decode data from image*/
Status decode_data_from_image(char * data, int size, DecodeInfo *decInfo);

/*To decode a byte from the lsb of image buffer of 8 bytes*/
//Status decode_byte_from_lsb(char data, char *image_buffer);
Status decode_byte_from_lsb(char* data, int pos, char *image_buffer);

/*To decode the file extension size of the secret file*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/*To decode integer type data(ie size) from lsb of image data*/
Status decode_size_from_lsb(int *size, DecodeInfo *decInfo);

/*To decode the file extension of the secret file*/
Status decode_secret_file_extn(DecodeInfo *decInfo);

/*To decode the file size of the secret file*/
Status decode_secret_file_size(DecodeInfo *decInfo);

/*To decode the data inside the secret file*/
Status decode_secret_data(DecodeInfo *decInfo);



#endif