#include<stdio.h>
#include<string.h>
#include "encode.h"
#include "common.h" 
#include<unistd.h>
/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
 	uint width, height;
    	// Seek to 18th byte
    	fseek(fptr_image, 18, SEEK_SET);

    	// Read the width (an int)
    	fread(&width, sizeof(int), 1, fptr_image);
    	printf("\033[37mwidth = %u\n", width);

    	// Read the height (an int)
    	fread(&height, sizeof(int), 1, fptr_image);
    	printf("height = %u\n", height);

    	// Return image capacity
    	return width * height * 3;
}
uint get_file_size(FILE *fptr)
{
    	fseek(fptr,0,SEEK_END);
    	return ftell(fptr);
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    	// Src Image file
    	encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    	// Do Error handling
    	if (encInfo->fptr_src_image == NULL)
    	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
		return e_failure;
    	}

    	// Secret file
    	encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    	// Do Error handling
    	if (encInfo->fptr_secret == NULL)
    	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
		return e_failure;
    	}
    	// Stego Image file
    	encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    	// Do Error handling
    	if (encInfo->fptr_stego_image == NULL)
    	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
		return e_failure;
    	}

    	// No failure return e_success
    	return e_success;
}


/* Check operation type */
OperationType check_operation_type(char *argv[])
{
   	if(strcmp(argv[1],"-e") == 0)
   	{
		sleep(1);
		return e_encode;
    	}
    	else if(strcmp(argv[1],"-d") == 0)
    	{
		sleep(1);
		return e_decode;
    	}
    	else
    	{
		return e_unsupported;
    	}
}
Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)
{
    	if(strstr(argv[2],".bmp") != NULL)//checking argument 2 file contains .bmp ext or not
    	{
		encInfo -> src_image_fname = argv[2];
		if((strstr(argv[3],".txt") != NULL) || (strstr(argv[3],".c") != NULL) || (strstr(argv[3],".sh") != NULL))
		{
	    		encInfo -> secret_fname = argv[3];
	    		strcpy(encInfo -> extn_secret_file,strstr(argv[3],"."));
	    		if(argv[4] == NULL)
	    		{
				encInfo -> stego_image_fname = "stego.bmp";
				return e_success;
	    		}
	    		else if(strstr(argv[4],".bmp") != NULL)
	    		{
				encInfo -> stego_image_fname = argv[4];
				return e_success;
	    		}
		}
		return e_failure;
    	}
    	return e_failure;
}

Status check_capacity(EncodeInfo *encInfo)
{
	//To check the capacity
 	int value = (54 + (strlen(MAGIC_STRING) + 4 + strlen(strstr(encInfo -> secret_fname,".")) + 4 + get_file_size(encInfo -> fptr_secret))) * 8;
    	printf("\033[37mValue = %d\n",value); //Print the value
    	if(value < get_image_size_for_bmp(encInfo -> fptr_src_image))
    	{
		return e_success;
    	}
    	return e_failure;
}
Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_dest_image)
{
	//Copy the header files
    	fseek(fptr_src_image,0,SEEK_SET);
    	char buffer[54];
    	fread(buffer, 54, 1, fptr_src_image); //Read the source image
    	fwrite(buffer, 54, 1, fptr_dest_image);  //To copy the source image to destination image
    	return e_success;
}

Status encode_magic_string(const char *magic_string,EncodeInfo *encInfo)   //To encode the magic string
{
	//Encode the data from image function call
    	encode_data_to_image(MAGIC_STRING,strlen(MAGIC_STRING),encInfo -> fptr_src_image,encInfo -> fptr_stego_image);
    	return e_success;
}

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    	char image_buffer[8];
    	for(int i = 0;i < size;i++)   //To run loop upto size times
    	{
		fread(image_buffer, 8, 1, fptr_src_image); //Read the source image
		encode_byte_to_lsb(data[i], image_buffer); //byte to lsb function call
		fwrite(image_buffer,8,1,fptr_stego_image);  //Copy the image to stego image file
    	}
    	return e_success;
}

Status encode_byte_to_lsb(char data,char *image_buffer) //Function Defination 
{
    	int i;
    	for(i = 0;i < 8;i++)
    	{
		image_buffer[i] = image_buffer[i] & ~1 | ((data & (1 << (7 - i))) >> (7 - i));  
    	}
}

Status encode_extn_size(int size,EncodeInfo *encInfo) 
{
    	char image_buffer[32];
    	fread(image_buffer, 32, 1, encInfo -> fptr_src_image);  //Read the image bufffer
    	encode_size_to_lsb(size, image_buffer);   //Function call
    	fwrite(image_buffer, 32, 1, encInfo -> fptr_stego_image);  //Write the data to stego image 
    	return e_success;
}

Status encode_size_to_lsb(int size,char *image_buffer)
{
    	int i;
    	for(i = 0;i < 32; i++) 
    	{
		image_buffer[i] = image_buffer[i] & ~1 | (unsigned)((size & (1 << (31 - i))) >> (31 - i));
    	}
} 

Status encode_secret_file_extn(const char *file_extn,EncodeInfo *encInfo)
{
	//Function call encode the data to image
    	encode_data_to_image(encInfo -> extn_secret_file,strlen(encInfo -> extn_secret_file),encInfo -> fptr_src_image,encInfo -> fptr_stego_image);
    	return e_success;
}

Status encode_secret_file_size(long file_size,EncodeInfo *encInfo)
{
	//Encode the secret file
    	char secret_buffer[32];
    	fread(secret_buffer, 32, 1, encInfo -> fptr_src_image);  
    	encode_size_to_lsb(file_size, secret_buffer);
    	fwrite(secret_buffer,32,1,encInfo -> fptr_stego_image);
    	return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
	//Encode the file from data
    	encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);
    	char secret_buffer[encInfo -> size_secret_file];
    	rewind(encInfo -> fptr_secret);
    	fread(secret_buffer, encInfo -> size_secret_file, 1, encInfo -> fptr_secret);
    	encode_data_to_image(secret_buffer, encInfo -> size_secret_file, encInfo -> fptr_src_image, encInfo -> fptr_stego_image);
    	return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src,FILE *fptr_dest)
{ 
	//Function to Copy thr remaining datas
    	char ch;
    	while(fread(&ch,1,1,fptr_src) != 0)
    	{
		fwrite(&ch,1,1,fptr_dest);
    	}
    	return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    	if(open_files(encInfo)==e_success)
    	{
		sleep(1);
		printf("Step2:Open files is success\n");
		if(check_capacity(encInfo)==e_success)
		{
	    		sleep(1);
	    		printf("\033[34mStep3:Check capacity is success\n");
	    		if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
	    		{
				sleep(1);
				printf("Step4:Copy bmp header file successful\n");
				if(encode_magic_string(MAGIC_STRING,encInfo)==e_success)
				{
		    			sleep(1);
		    			printf("Step5:Encode magic sting successfully\n");
		    			if(encode_extn_size(strlen(strstr(encInfo->secret_fname,".")),encInfo)==e_success)
		    			{
						sleep(1);
						printf("Step6:Extension size encoded successfully\n");
						if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_success)
						{
			    				sleep(1);
			    				printf("Step7:Secret file extension get successfully\n");
			    				if(encode_secret_file_size(get_file_size(encInfo->fptr_secret),encInfo)==e_success)
			    				{
								sleep(1);
								printf("Step8:Secret file size encoded get successfully\n");
								if(encode_secret_file_data(encInfo)==e_success)
								{
					    				sleep(1);
				  		  			printf("Step9:Secret file data encoded get successfully\n");
				   					if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
				    					{
										sleep(1);
										printf("Step10:Remaining data Copied Successfully\n\n");
										fclose(encInfo->fptr_src_image);  //TO close the source image
										fclose(encInfo->fptr_secret);   //To close the secret file
										fclose(encInfo->fptr_stego_image);   //To Claose the stego image
				    					}
				    					else
				    					{
				    						printf("ERROR:Secret file data encode failure\n");
				    					}
								}
								else
								{
									printf("ERROR:Secret file size encode failure\n");
								}
			    				}
			    				else
			    				{
			    					printf("ERROR:Secret file extn encode failure\n");
			    				}
						}
						else
						{
							printf("ERROR:Extension size encode failure\n");
		    				}
					}
					else
					{
		    				printf("ERROR:Magic string encode failure\n");
					}
				}
				else
				{
					printf("ERROR:Copy bmp header failure\n");
				}
       			}
			else
			{
	    			printf("ERROR:Check capacity failure\n");
			}
		}
		else
		{	
			printf("ERROR:File is Not open\n");
    		}
	}
}
