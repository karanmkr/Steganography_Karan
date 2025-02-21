#include <stdio.h>
#include<string.h>
#include "decode.h"
#include "common.h"
#include "types.h"
#include<stdlib.h>
#include <unistd.h>


Status open_file(DecodeInfo *decInfo)
{
	//To open the file
  	decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r"); 
    	if (decInfo->fptr_src_image == NULL) 
   	{
		perror("fopen");
		fprintf(stderr, "ERROR:Not open file %s\n", decInfo->src_image_fname);  //Print the error
		return e_failure;
    	}
    	fseek(decInfo->fptr_src_image, 54, SEEK_SET);
    	return e_success;
}
Status decode_image_to_data(FILE * fptr_src_image)
{
	//Function to decode image to data
  	int i;  	
	char image_buffer[8],sum = 0;
    	fread(image_buffer, 8, 1, fptr_src_image); //Read the source file 
    	for(i = 0;i <= 7; i++) 
    	{
		sum = sum+((image_buffer[i] & 1) << 7 - i); //image buffer left shifted to 7-i times
    	}
    	return sum;
}

Status decode_image_to_size(FILE * fptr_src_image)
{
	//function decode the image to size
   	int i;
	char image_buffer[32],sum = 0;
    	fread(image_buffer, 32, 1, fptr_src_image);
    	for(i = 0;i <= 31; i++)
    	{
		sum = sum + ((image_buffer[i] & 1) << 31 - i);
    	}
    	return sum;
}


Status read_and_validate_decode_args(char *argv[],DecodeInfo *decinfo)
{
	//Function to read and validate the decode argumewnts
    	if(strstr(argv[2], ".bmp" ) != NULL)  //To check 2 argv ".bmp" is present or not
    	{
		decinfo->src_image_fname = argv[2]; 
		return e_success;
    	}
    	return e_failure;
}

Status decode_magic_string(DecodeInfo *decInfo)
{
	//Function to decode the magic string
	int i;
    	char magic_string[3];
    	for(i = 0;i < 2; i++)
    	{
		magic_string[i] = decode_image_to_data(decInfo->fptr_src_image); //Function call 
    	}
    	if(!strcmp(magic_string, MAGIC_STRING))  //To compare the magic strings
    	{
		return e_success; 
    	}
    	else
    	{
		return e_failure;
    	}
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
	//Function to secret file extenstion size
    	decInfo->extn_size = decode_image_to_size(decInfo->fptr_src_image);  //Function call
    	return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
	//Function to decode the file extension
    	int i;
    	for( i = 0;i < decInfo->extn_size; i++) 
    	{
		decInfo->extn_secret_file[i] = decode_image_to_data(decInfo->fptr_src_image);  
    	}
    	decInfo->extn_secret_file[i]= '\0';

    	char fname[100]= "output"; //To decler the output file name
    	strcat(fname,decInfo->extn_secret_file);  //Concatinate
    	decInfo-> secret_fname = fname; 
    	decInfo->fptr_secret = fopen(decInfo->secret_fname,"w");  //Opern the file 
    	return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
	//Decode the secret file size
    	decInfo->size_secret_file=decode_image_to_size(decInfo->fptr_src_image);  //Function call
    	return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
	//Function to decode the file data
    	char ch;int i;
    	for(i = 0; i < decInfo->size_secret_file; i++)
    	{
		ch = decode_image_to_data(decInfo->fptr_src_image);
		putc(ch,decInfo->fptr_secret); 
    	}
    	return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
	//Function to Decodeing
    	if(open_file(decInfo) == e_success)
    	{
		sleep(1);
		printf("Step2:Open File is Success\n");
		if(decode_magic_string(decInfo) == e_success)
		{
	    		sleep(1);
	    		printf("Step3:Decode Magic String is Success\n");
	    		if(decode_secret_file_extn_size(decInfo) == e_success)
	    		{
				sleep(1);
				printf("Step4:Secret file extn size decoded success\n");
				if(decode_secret_file_extn(decInfo) == e_success )
				{
		    			sleep(1);
		    			printf("Step5:Secret file Extension is success\n");
		    			if(decode_secret_file_size(decInfo) == e_success)
		    			{
						sleep(1);
						printf("Step6:Secret file size decoded Success\n");
						if(decode_secret_file_data(decInfo) == e_success)
						{
			    				sleep(1);
			    				printf("step7:Secret file data decoded success\n\n");
			    				return e_success;
						}
						else
						{
			    				printf("ERROR:Secret file data decoding failed\n");
						}
		    			}
		   	 		else
		    			{
						printf("ERROR:Secret file data size decoding failed\n");
		    			}
				}
				else
				{
		    			printf("ERROR:Sectet file extension decoding failed\n");
				}
	    		}
	    		else
	    		{
				printf("ERROR:Secret file extension size decoding failed\n");
	    		}
		}
		else
		{
	    		printf("ERROR:Magic string decoding failed\n");
		}
    	}
    	else
    	{
		printf("ERROR:Coping header failed\n");
    	}
}
