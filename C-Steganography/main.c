/*
    Author  : Ajay Krishnan V
    Date    : 12-11-2025
    Project Description : This project performs lsb image steganography. User has to provide the secret file that contains the data
    to be encoded and a bitmap image on which the program will encode the secret data. Pass the secret file and bitmap image via 
    command line arguments and follow the specified format while passing the CLA.  
*/


#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc, char *argv[6])
{
    if(argc < 3 || argc > 5)
    {
        printf(RED"\nPlease pass correct arguments.\n"RESET);
        printf("For encoding ---> ./a.out -e  source.bmp  secret.txt  stego.bmp\n");
        printf("For decoding ---> ./a.out -d  stego.bmp   output.txt\n\n");

        return e_failure;
    }
    else if(argc == 4)
    {
        argv[4] = "\0";
    }
    else if(argc == 3)
    {
        argv[3] = "\0";
    }

    if(check_operation_type(argv) == e_encode)
    {
        printf(YELLOW"\nYou have chosen encoding.\n"RESET);
        EncodeInfo encInfo;     //declaring structure variable to store the info of the 3 files

        if(read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            printf("read_and_validate_encode_args is successful.\n");
            if(do_encoding(&encInfo) == e_success)
            {
                printf(GREEN"Encoding is successful.\n\n"RESET);
            }
            else
            {
                printf(RED"Encoding is unsuccessful..⚠️ \n\n"RESET);
                return e_failure;
            }
        }
        else
        {
            printf(RED"read_and_validate_encode_args is failure..\n\n"RESET);
            return e_failure;
        }
    }
    else if(check_operation_type(argv) == e_decode)
    {
        printf(YELLOW"\nYou have chosen decoding.\n"RESET);
        DecodeInfo decInfo;

        if(read_and_validate_decode_args(argv, &decInfo) == e_success)
        {
            printf("read_and_validate_decode_args is successful.\n");
            if(do_decoding(&decInfo) == e_success)
            {
                printf(GREEN" Decoding is successful.\n\n"RESET);
            }
            else
            {
                printf(RED"Decoding is unsuccessful..⚠️ \n\n"RESET);
            }
        }
        else
        {
            printf(RED"read_and_validate_decode_args is unsuccessful..⚠️ \n\n"RESET);
        }
    }
    else
    {
        printf(RED"\nOperation type not supported..⚠️ Please pass correct arguments.\n"RESET);
        printf("For encoding ---> ./a.out -e  source.bmp  secret.txt  stego.bmp\n");
        printf("For decoding ---> ./a.out -d  stego.bmp   output.txt\n\n");

        return e_failure;
    }

    return e_success;
}

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-e") == 0)
        return e_encode;
    else if(strcmp(argv[1], "-d") == 0)
        return e_decode;
    else
        return e_unsupported;
}
