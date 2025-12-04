#ifndef DECODE_H
#define DECODE_H

#include "types.h"
#include "common.h"


typedef struct _DecodeInfo
{
    char *stego_image_fname;
    FILE *fptr_stego_image;

    char *secret_fname;
    FILE *fptr_secret;

    char magic_string[MAX_MAGIC_LEN+1];

    long size_extn_file;
    char *extn_secret_file;
    long size_secret_data;
    char *secret_data;
} DecodeInfo;

#endif

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo);

Status open_files_decode(DecodeInfo *decInfo );

Status get_magic_string(DecodeInfo *decInfo);

Status check_magic_string(char *magic_string,DecodeInfo *decInfo);

Status decode_lsb_to_byte(char* pass, char *image_buffer);

Status decode_secret_file_extn_size(DecodeInfo *decInfo);

Status decode_secret_file_extn(DecodeInfo *decInfo);

Status decode_secret_file_size(DecodeInfo *decInfo);

Status decode_secret_file_data(DecodeInfo *decInfo);

Status copy_secret_file_data_to_file(DecodeInfo *decInfo);

Status open_secret_file(DecodeInfo *decInfo);