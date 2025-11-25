#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(strstr(argv[2], ".bmp") != NULL)
    {
        printf(".bmp is present.\n");
        decInfo -> stego_image_fname = argv[2];
    }
    else
    {
        printf(RED".bmp is not present..⚠️ \n"RESET);
        return e_failure;
    }

    /*if(strchr(argv[3], '.') != NULL)
    {
        printf("Secret file is present.\n");
        decInfo -> out_fname = argv[3];
    }*/
    
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    if(open_files_decode(decInfo) == e_success)
    {
        printf("Open files is successful.\n");
    }
    else
    {
        printf(RED"Open files is not successful.\n"RESET);
    }

    if(skip_bmp_header(decInfo -> fptr_stego_image) == e_success)
    {
        printf("skipped stego.bmp header.\n");
    }
    else
    {
        printf("skipping bmp header is unsuccesful..⚠️ \n");
        return e_failure;
    }

    if(decode_magic_string_size(decInfo) == e_success)
    {
        printf("magic string size decoded successfully.\n");
    }
    else
    {
        printf("magic string size not decoded successfully..⚠️\n");
        return e_failure;
    }

    if(decode_magic_string(decInfo) == e_success)
    {
        printf("magic string decoded and matches successfully.\n");
    }
    else
    {
        printf("magic string not decoded successfully..⚠️\n");
        return e_failure;
    }

    if(decode_secret_file_extn_size(decInfo) == e_success)
    {
      printf("Secret file extension size decoded successfully.\n");
    }
    else
    {
        printf(RED"Secret file extension size is not decoded successfully..⚠️ \n"RESET);
        return e_failure;
    }

    if(decode_secret_file_extn(decInfo) == e_success)
    {
      printf("Secret file extension decoded successfully.\n");
    }
    else
    {
      printf("Secret file extension not decoded successfully.\n");
      return e_failure;
    }

    if(decode_secret_file_size(decInfo) == e_success)
    {
      printf("Secret file size decoded successfully.\n");
    }
    else
    {
      printf("Secret file size not decoded successfully.\n");
      return e_failure;
    }

    if(decode_secret_data(decInfo) == e_success)
    {
      printf("Secret file data decoded successfully.\n");
    }
    else
    {
      printf(GREEN"Secret file data not decoded successfully.\n"RESET);
      return e_failure;
    }
    
    fclose(decInfo -> fptr_out);
    fclose(decInfo -> fptr_stego_image);
    
    return e_success;
}

Status open_files_decode(DecodeInfo *decInfo)
{
    decInfo -> fptr_stego_image = fopen(decInfo -> stego_image_fname, "r");
    if(decInfo -> fptr_stego_image == NULL)
    {
        printf("stego.bmp file is not present..⚠️ \n");
        return e_failure;
    }

    return e_success;
}

Status skip_bmp_header(FILE *fptr_stego)
{
    rewind(fptr_stego);
    fseek(fptr_stego, 54, SEEK_SET);
    return e_success;  
}

Status decode_magic_string_size(DecodeInfo *decInfo)
{
    if(decode_size_from_lsb(&decInfo -> magic_string_size, decInfo) == e_success)
    {
        //printf("original magic_string_size : %d\n", decInfo -> magic_string_size);
        return e_success;
    }
}

Status decode_magic_string(DecodeInfo *decInfo)
{
    char user_magic_string[20];
    printf("Enter the magic string for the file : ");
    scanf(" %[^\n]", user_magic_string);
    if(decode_data_from_image(decInfo -> original_magic_string, decInfo -> magic_string_size, decInfo) == e_success)
    {
        //printf("magic string buffer : %s\n", decInfo -> original_magic_string);
        if(strcmp(user_magic_string, decInfo -> original_magic_string) == 0)
        return e_success;
        else 
        {
            printf("Magic string is not matching..⚠️ \n");
            return e_failure;
        }
    }
}

Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo)
{
    for(int i = 0; i < size; i++)
    {
        fread(decInfo -> image_data, 8, 1, decInfo -> fptr_stego_image);
        decode_byte_from_lsb(data, i, decInfo -> image_data);
    }
    data[size] = '\0';
    return e_success;
}

Status decode_byte_from_lsb(char* data, int pos, char *image_buffer)
 {
    char ch = '\0';
    for(int i = 0; i < 8; i++)
    {
        ch = ch | (image_buffer[i] & 1) << i;
    }
    data[pos] = ch;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    if(decode_size_from_lsb(&decInfo -> out_file_extn_size, decInfo) == e_success)
    {
        printf("decInfo -> out_file_extn_size : %d\n", decInfo -> out_file_extn_size);
        return e_success;
    }
}

Status decode_size_from_lsb(int *size, DecodeInfo *decInfo)
{
    char image_buffer[32];
    *size = 0;
    fread(image_buffer, 32, 1, decInfo -> fptr_stego_image);
    for(int i = 0; i < 32; i++)
    {
        *size = *size | ((image_buffer[i] & 1) << i);
    }
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    if(decode_data_from_image(decInfo -> out_file_extn, decInfo -> out_file_extn_size, decInfo) == e_success)
    {
        printf("decInfo -> out_file_extn : %s\n", decInfo -> out_file_extn);
        strcpy(decInfo -> out_file_name, "output");
        decInfo -> out_fname = strcat(decInfo -> out_file_name, decInfo -> out_file_extn);
        decInfo -> fptr_out = fopen(decInfo -> out_fname, "w");
        return e_success;
    }
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    if(decode_size_from_lsb(&decInfo -> out_file_size, decInfo) == e_success)
    {
        printf("decInfo -> out_file_size : %d\n", decInfo -> out_file_size);
        return e_success;
    }
}

Status decode_secret_data(DecodeInfo *decInfo)
{
    char out_file_data[decInfo -> out_file_size];
    if(decode_data_from_image(out_file_data, decInfo -> out_file_size, decInfo) == e_success)
    {
        fwrite(out_file_data, decInfo -> out_file_size, 1, decInfo -> fptr_out);
        return e_success;
    }
}