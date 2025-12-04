#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include "encode.h"
#include "types.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e")==0)
    return e_encode;
    else if(strcmp(argv[1],"-d")==0)
    return e_decode;
    else
    return e_unsupported;

}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    FILE *fp;

if(argv[2]==NULL)
{
    printf("❌  Please enter a bmp file to encode\n");
    return e_failure;
}

else if(strstr(argv[2],".bmp")!=NULL)
{
    encInfo->src_image_fname=argv[2];
    fp=fopen(encInfo->src_image_fname, "r");
    if(fp!=NULL)
    {
    printf("    ✔️ %s is present\n",encInfo->src_image_fname);
    }
    else
    {
    printf("    ❌ %s is not present\n",encInfo->src_image_fname);
    return e_failure;
    }
    fclose(fp);
}

else
{
    fprintf(stderr,"❌ The source image %s should be .bmp file\n",argv[2]);
    return e_failure;
}

if(argv[3]==NULL)
{
    printf("❌  Please enter a txt file to encode\n");
    return e_failure;
}
else if(strstr(argv[3],".txt")!=NULL)
{
    encInfo->secret_fname=argv[3];
    fp=fopen(encInfo->secret_fname, "r");
    if(fp!=NULL)
    {
    printf("    ✔️ %s is present\n",encInfo->secret_fname);
    }
    else
    {
    printf("    ❌ %s is not present\n",encInfo->secret_fname);
    return e_failure;
    }
    fclose(fp);
}

else
{
    fprintf(stderr,"❌ The source image %s should be .txt file\n",argv[3]);
    return e_failure;
}

if(argv[4]==NULL){
    printf("    ❌ stego bmp file is not given\nCreating default.bmp by default\n");
    encInfo->stego_image_fname="default.bmp";
    printf("    ✔️ %s is created sucessfully\n",encInfo->stego_image_fname);
}
else if(strstr(argv[4],".bmp")!=NULL)
{
    encInfo->stego_image_fname=argv[4];
   fp=fopen(encInfo->stego_image_fname, "r");
    if(fp==NULL)
    {

        printf("    ❌ %s is not present\n",encInfo->stego_image_fname);
        printf("    Creating %s by default\n",encInfo->stego_image_fname);
        printf("    ✔️ %s is created sucessfully\n",encInfo->stego_image_fname);
        
    }
    else
    {
         printf("    ✔️ %s is present\n",encInfo->stego_image_fname);
         fclose(fp);
    }
    
    return e_success;
}
else
{
    fprintf(stderr,"❌ The stego image %s should be .txt file\n",argv[4]);
    return e_failure;
}
return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    printf("🔍 Opening all files...\n");
    if(open_files(encInfo)==e_success)
    {
        printf("✅ Files opened sucessfully\n");

        if(check_capacity(encInfo)==e_success)
        {
            printf("✅ Image capacity checked successfully\n");
            if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
            {
                printf("✅ BMP header copied successfully\n\n");

                printf("╔══════════════════════════════╗\n");
                printf("║     \033[32mStarted Data encoding\033[0m    ║\n");
                printf("╚══════════════════════════════╝\n\n");

                if(gets_magic_string(encInfo)==e_success);
                else return e_failure;
                if(encode_magic_string(encInfo->magic_string,encInfo)==e_success)
                {
                    printf("✅ Magic string encoded sucessfully\n");
                    if(encode_secret_file_extn_size(size_of_extn(encInfo->secret_fname),encInfo)==e_success)
                    {
                        printf("✅ File extension size encoded sucessfully\n");

                        if(encode_secret_file_extn(encInfo->secret_fname,encInfo)==e_success)
                        {
                            printf("✅ File extension encoded sucessfully\n");

                            if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
                            {
                                printf("✅ Secret file size encoded sucessfully\n");
                                
                                if(encode_secret_file_data(encInfo)==e_success)
                                {
                                    printf("✅ Secret file data encoded sucessfully\n");
                                    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
                                    {
                                        printf("✅ Remaining image data encoded sucessfully\n");
                                        return e_success;
                                    }
                                    else
                                    {
                                        printf("❌ Error, Remaining image data not encoded sucessfully\n");
                                        return e_failure; 
                                    }

                                }
                                else
                                {
                                    printf("❌ Error, Secret file data not encoded sucessfully\n");
                                    return e_failure;
                                }
                            }
                            else
                            {
                                printf("❌ Error, Secret file size not encoded sucessfully\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("❌ Error, File extension not encoded sucessfully\n");
                            return e_failure;
                        }
                        
                    }
                    else
                    {
                        printf("❌ Error, File extension size not encoded sucessfully\n");
                        return e_failure;
                    }

                }
                else{
                    printf("❌ Error, Magic string not encoded\n");
                    return e_failure;
                }
                
            }
        

            else
            {
                printf("❌ Error, BMP header not copied\n");
                return e_failure;
            }
        }
        else
        {
            printf("❌ Error, Image size is not sufficient\n");
            return e_failure;
        }
    }
    else
    {
        return e_failure;
    }
    
return e_success;

}
int size_of_extn(char *fname)
{
    char *a=strrchr(fname,'.');
    int si=strlen(a);
return si;
}


Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char data[1024];
    uint size;

    while ((size=fread(data,1,sizeof(data),fptr_src))>0)
    {
        fwrite(data,1,size,fptr_dest);
    }

    return e_success;
}


Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char data[32];
    fread(data,sizeof(data),1,encInfo->fptr_src_image);
    for(int i=0;i<sizeof(data);i++)
    {
        data[i]=(data[i]&~1)|((file_size>>i)&1);
    }
    fwrite(data,sizeof(data),1,encInfo->fptr_stego_image);
    return e_success;
}


Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char *secret=malloc(encInfo->size_secret_file);
    if(secret==NULL)
    return e_failure;

    rewind(encInfo->fptr_secret);

    if (fread(secret, 1, encInfo->size_secret_file, encInfo->fptr_secret) != encInfo->size_secret_file)
    {
        printf("❌ Error reading secret file\n");
        free(secret);
        return e_failure;
    }
    
    encode_data_to_image(secret,encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image);
    free(secret);
    return e_success;
}

Status encode_secret_file_extn_size(int extnsize, EncodeInfo *encInfo)
{
    char data[32];
    fread(data,sizeof(data),1,encInfo->fptr_src_image);
    for(int i=0;i<sizeof(data);i++)
    {
        data[i]=(data[i]&~1)|((extnsize>>i)&1);
    }
    fwrite(data,sizeof(data),1,encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
   char *a=strrchr(file_extn,'.');
   strcpy(encInfo->extn_secret_file,a);
   encInfo->extn_secret_file[MAX_FILE_SUFFIX] = '\0';
   //printf("%s\n",encInfo->extn_secret_file);
   encode_data_to_image(encInfo->extn_secret_file,strlen(encInfo->extn_secret_file), encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}


Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    if(encode_data_to_image((char*)magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
    {
        return e_success;
    }
    else
    printf("❌ Error, Magic string not sucessfully encoded\n");
    return e_failure;
}

Status gets_magic_string(EncodeInfo *encInfo)
{
    char pass[3];
de:
                    printf("\033[1;32m Enter magic string (max 2 chars):\033[0m ");
                    scanf("%s",pass);
                    if(strlen(pass)>2){
                    printf("❌ Magic string must be smaller\n");
                    goto de;
                    }
                    strcpy(encInfo->magic_string,pass);
                    return e_success;
}

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char str[MAX_IMAGE_BUF_SIZE];
    for(uint i=0;i<size;i++)
    {
        fread(str,8,1,fptr_src_image);
        encode_byte_to_lsb(data[i],str);
        fwrite(str,8,1,fptr_stego_image);
    }
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
for(uint i=0;i<MAX_IMAGE_BUF_SIZE;i++)
{
  image_buffer[i]=(image_buffer[i]&~1)|((data>>i)&1);
}
return e_success;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
char str[54];
rewind(fptr_src_image);
//fseek(fptr_src_image,0,SEEK_SET);
fread(str,54,1,fptr_src_image);
fwrite(str,54,1,fptr_dest_image);
return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
encInfo->size_secret_file=(long)get_file_size(encInfo->fptr_secret);
if(encInfo->image_capacity<(16+32+32+32+encInfo->size_secret_file*8))
{
    return e_failure;
}
return e_success;
}

uint get_file_size(FILE *fptr)
{
    uint size;
    fseek(fptr,0,SEEK_END);
    size=(uint)ftell(fptr);
    rewind(fptr);
    return size;
}

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    //printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
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
    	fprintf(stderr, "❌ ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }
    else
    {
        printf("  ✔️ Source file %s opened sucessfully\n",encInfo->src_image_fname);
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "❌ ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }
    else
    {
        printf("  ✔️ Secret file %s opened sucessfully\n",encInfo->secret_fname);
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "❌ ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }
    else
    {
        printf("  ✔️ Stego file %s opened sucessfully\n",encInfo->stego_image_fname);
    }

    // No failure return e_success
    return e_success;
}