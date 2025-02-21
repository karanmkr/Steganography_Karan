/*
Name: Karan KR
Date: 18/10/2024
Description:  WAP to LSB Image Steganography?
-----------------------------------------------------------------------
SAMPLE INPUT FOR ENCODING:
./a.out -e beautiful.bmp secret.txt
-----------------------------------------------------------------------
SAMPLE OUTPUT FOR ENCODING:
Select  Encoding
Step1:Read and Validate argument is success
Step2:Open files is success
Value = 744
width = 1024
height = 768
Step3:Check capacity is success
Step4:Copy bmp header file successful
Step5:Encode magic sting successfully
Step6:Extension size encoded successfully
Step7:Secret file extension get successfully
Step8:Secret file size encoded get successfully
Step9:Secret file data encoded get successfully
Step10:Remaining data Copied Successfully

ENCODING IS COMPLETED...
SUCCESSFULLY...
-----------------------------------------------------------------------------
SAMPLE INPUT FOR DECODING:
./a.out -d stego.bmp
----------------------------------------------------------------------------
SAMPLE INPUT FOR DECODING:
Slected Decoding
Step1:Read and Validate argument is success
Step2:Open File is Success
Step3:Decode Magic String is Success
Step4:Secret file extn size decoded success
Step5:Secret file Extension is success
Step6:Secret file size decoded Success
step7:Secret file data decoded success

DECODING IS COMPLETED...
SUCCESSFULLY...
-----------------------------------------------------------------------------
*/
#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

//main function 
int main(int argc, char *argv[])
{
	if(argc >= 3)
	{
		if(check_operation_type(argv) == e_encode)  //To check the arguments present or not
		{
			EncodeInfo encIfo;   //Structure variable
			printf("\033[35mSelect  Encoding\n"); 
			if(read_and_validate_encode_args(argv, &encIfo) == e_success) //Function Call
			{
                        	printf("\033[34mStep1:Read and Validate argument is success\n");
				if(do_encoding(&encIfo) == e_success)   //Function call to do encoding
				{
					printf("\033[33mENCODING IS COMPLETED...\nSUCCESSFULLY...\n");
				}
				else
				{
					 printf("Error:Encoding is Failed...\n");
				}
			}
			else
			{
				 printf("Invalid argument\n");
			}
		}
		else  if(check_operation_type(argv) == e_decode)  //To check the decoding
		{
			DecodeInfo decIfo;   //Structure variable
			printf("\033[35mSlected Decoding\n");
			if(read_and_validate_decode_args(argv, &decIfo) == e_success)   
			{
				 printf("\033[34mStep1:Read and Validate argument is success\n");
				 if(do_decoding(&decIfo) == e_success)
                                {
					 printf("\033[33mDECODING IS COMPLETED...\nSUCCESSFULLY...\n");
                                }
                                else
                                {
                                         printf("Error:Decoding is Failed...\n");
				}
			}
		}
	}
	else
	{
		 printf("Insufficient argument\n");
	}
    return 0;
}
