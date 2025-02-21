#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
  //  uint image_capacity;
//    uint bits_per_pixel;
    //char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;
    char extn_size;
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

} DecodeInfo;

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the Decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_file(DecodeInfo *decInfo);

/* Store Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/*Decode secret file extention size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode function, which does the real encoding */
Status decode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/*find the size ofthe img */
Status decode_size_to_image(int data, DecodeInfo *decInfo);

/* Decode a byte into LSB of image data array */
Status decode_byte_to_lsb(char data, char *image_buffer);

/* Decode a size into LSB of image data array */
Status decode_size_to_lsb(int size, char *image_buffer);

Status decode_secret_file_extn_size(DecodeInfo *decInfo);
#endif
