#include <stdio.h>
#include <string.h>
#include "types.h"
#include "encode.h"

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strstr(argv[2], ".bmp") != NULL)
    {
        printf(".bmp(source image) is present.\n");
        encInfo -> src_image_fname = argv[2];
    }
    else
    {
        printf(RED".bmp is not present..⚠️ \n"RESET);
        return e_failure;
    }

    if(strchr(argv[3], '.') != NULL)
    {
        printf("secret file is present.\n");
        encInfo -> secret_fname = argv[3];
        strcpy(encInfo -> extn_secret_file, strchr(encInfo -> secret_fname, '.'));
    }
    else
    {
        printf(RED"Secret file is not present..⚠️ \n"RESET);
        return e_failure;
    }

    if(strstr(argv[4], ".bmp") != NULL)
    {
        printf("stego.bmp is present.\n");
        encInfo -> stego_image_fname = argv[4];
    }
    else
    {
        encInfo -> stego_image_fname = "stego.bmp";
    }

    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_success)    //here you pass the value of the structure pointer as the address of the structure defined in main fn.
    {
        printf("All the files are opened successfully.\n");
    }
    else
    {
        printf(RED"Files opening is not successful..⚠️ \n"RESET);
        return e_failure;
    }

    if(check_capacity(encInfo) == e_success)
    {
        printf("Capacity check of source image is successful.\n");
    }
    else
    {
        printf(RED"Capacity check is unsuccessful..⚠️ \n"RESET);
        return e_failure;
    }

    if(copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
    {
        printf("Header copied successfully.\n");
    }
    else
    {
        printf(RED"Header copying is unsuccessful..⚠️ \n"RESET);
        return e_failure;
    }

    if(encode_magic_string(encInfo) == e_success)
    {
        printf("Magic string encoded successfully.\n");
    }
    else
    {
        printf(RED"Magic is not encoded successfully..⚠️ \n"RESET);
        return e_failure;
    }

    if(encode_secret_file_extn_size(strlen(encInfo -> extn_secret_file), encInfo) == e_success)
    {
      printf("Secret file extension size encoded successfully.\n");
    }
    else
    {
        printf(RED"Secret file extension size is not encoded successfully..⚠️ \n"RESET);
        return e_failure;
    } 
    
    if(encode_secret_file_extn(encInfo -> extn_secret_file, encInfo) == e_success)
    {
      printf("Secret file extension encoded successfully.\n");
    }
    else
    {
        printf(RED"Secret file extension is not encoded successfully..⚠️ \n"RESET);
        return e_failure;
    } 

    if(encode_secret_file_size(encInfo -> size_secret_file, encInfo) == e_success)
    {
      printf("Secret file size encoded successfully.\n");
    }
    else
    {
        printf(RED"Secret file size is not encoded successfully..⚠️ \n"RESET);
        return e_failure;
    } 

    if(encode_secret_file_data(encInfo) == e_success)
    {
      printf(GREEN"Secret file data encoded successfully.\n"RESET);
    }
    else
    {
        printf(RED"Secret file data is not encoded successfully..⚠️ \n"RESET);
        return e_failure;
    } 

    if(copy_remaining_img_data(encInfo) == e_success)
    {
      printf("Remaining image data is copied successfully.\n");
    }
    else
    {
        printf(RED"Remaining image data is not copied successfully..⚠️ \n"RESET);
        return e_failure;
    }

    fclose(encInfo -> fptr_src_image);
    fclose(encInfo -> fptr_stego_image);
    fclose(encInfo -> fptr_secret);




    return e_success;
}

/*
    function to open source image file, secret data file and resultant stego file.
*/
Status open_files(EncodeInfo *encInfo)
{
    encInfo -> fptr_src_image = fopen(encInfo -> src_image_fname, "r");
    if(encInfo -> fptr_src_image == NULL)
    {
        printf("Source file is not present..⚠️ \n\n");
        return e_failure;
    }

    encInfo -> fptr_secret = fopen(encInfo -> secret_fname, "r");
    if(encInfo -> fptr_secret == NULL)
    {
        printf("Secret file is not present..⚠️ \n\n");
        return e_failure;
    }

    encInfo -> fptr_stego_image = fopen(encInfo -> stego_image_fname, "w");

    return e_success;
}

/*
    to find size of source.bmp image file and secret file and compare them.
*/
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);
    printf("Secret file size : %lu\n", encInfo -> size_secret_file);

    printf("Enter the magic string : ");
    scanf(" %[^\n]", encInfo -> magic);
    encInfo -> magic_string_size = strlen(encInfo -> magic);
    printf("encInfo -> magic_string_size : %d\n", encInfo -> magic_string_size);

    encInfo -> remaining_data_size = encInfo -> image_capacity - strlen(encInfo -> magic) * 8 + 32 + 32 + 32 + encInfo -> size_secret_file * 8;
    if(encInfo -> image_capacity > 32 + strlen(encInfo -> magic) * 8 + 32 + 32 + 64 + encInfo -> size_secret_file * 8)
    {
        return e_success;
    }
    else
    {
        printf(RED"Image capacity failed..⚠️ \n"RESET);
        return e_failure;
    }
}

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;

    fseek(fptr_image, 18, SEEK_SET);

    fread(&width, 4, 1, fptr_image);    //not recommended to use fsacnf() on binary files when opened in text mode, ie not in binary mode 
    //printf("pixel width of source.bmp : %d\n", width);

    fread(&height, 4, 1, fptr_image);
    //printf("pixel height : %d\n", height);

    return width * height * 3;  //returning size of source image in bytes
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr); //with text file, you can use ftell() function, but not with a binary file when not in binary mode.
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    rewind(fptr_src_image);

    char buffer[54];
    fread(buffer, 54, 1, fptr_src_image);

    fwrite(buffer, 54, 1, fptr_dest_image);

    return e_success;
}

Status encode_magic_string(EncodeInfo *encInfo)
{
    if(encode_size_to_lsb(encInfo -> magic_string_size, encInfo) == e_success)
    {
        printf("Magic string size encoded successfully.\n");
        if(encode_data_to_image(encInfo -> magic, encInfo -> magic_string_size, encInfo) == e_success)
        {
            return e_success;
        }
        else
        {
            return e_failure;
        }
    }
    else
    {
        printf(RED"Magic string size not encoded successfully..⚠️ \n");
        return e_failure;
    }

}

/*currently, both source and stego.bmp file pointers point to 54th byte.
    In this function, we take each character of the secret data in data[i] using a for loop and we read 8 bytes of image data to image_data (buffer) variable, 
    and then pass them to to encode_byte_to_lsb() function to encode it to lsb. After encoding, the image_data is written to the stego.bmp file*/
Status encode_data_to_image(char *data, int size, EncodeInfo *encInfo)
{
    for(int i = 0; i < size; i++)
    {
        fread(encInfo -> image_data, 8, 1, encInfo -> fptr_src_image);
        encode_byte_to_lsb(data[i], encInfo -> image_data);
        fwrite(encInfo -> image_data, 8, 1, encInfo -> fptr_stego_image);
    }

    return e_success;
}
//we will use separate functions to encode character and to encode integer separately.
/* Now you have 8 byte data in image_buffer. In this function, you use loop to access each byte in image_buffer. In each byte, you clear the lsb,
(0xFE is same as ~1). Then we get i th bit from lsb of the data variable and put it at the lsb of the image_buffer[i].*/
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i = 0; i < 8; i++)
    {
        image_buffer[i] = (image_buffer[i] & 0xFE) | (data & (1 << i)) >> i;
    }
}

Status encode_secret_file_extn_size(int file_extn__size, EncodeInfo *encInfo)
{
    if(encode_size_to_lsb(file_extn__size, encInfo) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    if(encode_data_to_image(file_extn, strlen(file_extn), encInfo) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    encode_size_to_lsb(file_size, encInfo);

    return e_success;
}

/* in this function, we take single byte of data from secret file, reads 8 byts of data from source image and pass them to encode_byte_to_lsb().
The encoded 8 bytes we have in the encInfo -> image_data variable we writes in stego file. This code runs for number of times which 
equals to the size of the secret file. */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    rewind(encInfo -> fptr_secret);
    char secret_buffer[encInfo -> size_secret_file];
    fread(secret_buffer, encInfo -> size_secret_file, 1, encInfo -> fptr_secret);
    encode_data_to_image(secret_buffer, encInfo -> size_secret_file, encInfo);

    // for(int i = 0; i < encInfo -> size_secret_file; i++)
    // {   fread(encInfo -> secret_data, 1, 1, encInfo -> fptr_secret);
    //     fread(encInfo -> image_data, 8, 1, encInfo -> fptr_src_image);
    //     encode_byte_to_lsb(encInfo -> secret_data[0], encInfo -> image_data); fwrite(encInfo -> image_data, 8, 1, encInfo -> fptr_stego_image);
    // }
    return e_success;
}

Status copy_remaining_img_data(EncodeInfo *encInfo)
{
    char copy_image_data;

    while(fread(&copy_image_data, 1, 1, encInfo -> fptr_src_image))
    {
        fwrite(&copy_image_data, 1, 1, encInfo -> fptr_stego_image);
    }
    /*while(1)
    {
    char copy_image_data[1000];
    int read = fread(&copy_image_data, 1000, 1, encInfo -> fptr_src_image);
        if(read == 1)
        fwrite(&copy_image_data, 1000, 1, encInfo -> fptr_stego_image);
        else
        {
            fread(&copy_image_data, read, 1, encInfo -> fptr_src_image);
            fwrite(&copy_image_data, read, 1, encInfo -> fptr_stego_image);
            break;
        }
    }*/
    /* for(int i = 0; i < encInfo -> remaining_data_size; i++)
       {fread(&copy_image_data, 1, 1, encInfo -> fptr_src_image);
         fwrite(&copy_image_data, 1, 1, encInfo -> fptr_stego_image);} */

    return e_success;
}

Status encode_size_to_lsb(int size, EncodeInfo *encInfo)
{
    char buffer[32];
    fread(buffer, 32, 1, encInfo -> fptr_src_image);
    for(int i = 0; i < 32; i++)
    {
        buffer[i] = (buffer[i] & 0xFE) | ((size & (1 << i)) >> i);
    }
    fwrite(buffer, 32, 1, encInfo -> fptr_stego_image);

    return e_success;
}