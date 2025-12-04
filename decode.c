#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decode.h"
#include "types.h"


Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    FILE *fp;

if(argv[2]==NULL)
{
    printf("❌  Please enter a bmp file name to decode\n");
    return e_failure;
}    
else if(strstr(argv[2],".bmp")!=NULL)
{
    decInfo->stego_image_fname=argv[2];
    fp=fopen(decInfo->stego_image_fname, "r");
    if(fp!=NULL)
    {
    printf("    ✔️ %s is present\n",decInfo->stego_image_fname);
    }
    else
    {
    printf("    ❌ %s is not present\n",decInfo->stego_image_fname);
    return e_failure;
    }
    fclose(fp);
}
else
{
    fprintf(stderr,"❌ The Stego image %s should be .bmp file\n",argv[2]);
    return e_failure;
}

if(argv[3]==NULL){
    printf("    ❌ secret file name not given\n");
    printf("    Using default base name: default\n");
    decInfo->secret_fname = strdup("default");
}
else
{
    decInfo->secret_fname = strdup(argv[3]);
    FILE *fp = fopen(decInfo->secret_fname, "r");

    if (fp)
    {
        printf("    ✔️ %s is present\n", decInfo->secret_fname);
        fclose(fp);
    }
    else
    {
        printf("    ❌ %s is not present, will create new file with decoded extension\n", decInfo->secret_fname);
    }
}
return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    printf("🔍 Opening image file...\n");
    if(open_files_decode(decInfo)==e_success)
    {
        printf("✅ Image files opened sucessfully\n");
        get_magic_string(decInfo);
        if(check_magic_string(decInfo->magic_string,decInfo)==e_success)
        {
            printf("╔══════════════════════════════╗\n");
                printf("║     \033[32mStarted Data decoding\033[0m    ║\n");
                printf("╚══════════════════════════════╝\n\n");
            if(decode_secret_file_extn_size(decInfo)==e_success)
            {
                printf("✅ Secret files extention size decoded sucessfully\n");
                if(decode_secret_file_extn(decInfo)==e_success)
                {
                    printf("✅ Secret files extention decoded sucessfully\n");

                    if(decode_secret_file_size(decInfo)==e_success)
                    {
                        printf("✅ Secret files size decoded sucessfully\n");

                        if(decode_secret_file_data(decInfo)==e_success)
                        {
                            printf("✅ Secret files data decoded sucessfully\n");

                            if(copy_secret_file_data_to_file(decInfo)==e_success)
                            {
                                printf("✅ Secret files data copied to text file sucessfully\n");
                            }
                            else
                            {
                                 printf("❌ Secret files data not copied to text file\n");
                            }
                        }
                        else
                        {
                            printf("❌ Secret files data not decoded\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("❌ Secret files size not decoded\n");
                        return e_failure;
                    }

                }
                else
                {
                    printf("❌ Secret files extention not decoded\n");
                    return e_failure;
                }
            }
            else
            {
                printf("❌ Secret files extention not decoded\n");
                return e_failure;
            }
        }
        else
        {
            return e_failure;
        }
        
    }
    else
    {
        return e_failure;
    }
return e_success;
}


Status copy_secret_file_data_to_file(DecodeInfo *decInfo)
{
open_secret_file(decInfo);
fwrite(decInfo->secret_data,decInfo->size_secret_data,1,decInfo->fptr_secret);
return e_success;
}


Status decode_secret_file_data(DecodeInfo *decInfo)
{
char buffer[8];
decInfo->secret_data = malloc(decInfo->size_secret_data+1);

for(int i=0;i<decInfo->size_secret_data;i++)
{
    fread(buffer,8,1,decInfo->fptr_stego_image);
    decode_lsb_to_byte(&decInfo->secret_data[i],buffer);
}
decInfo->secret_data[decInfo->size_secret_data]='\0';
return e_success;
}


Status decode_secret_file_size(DecodeInfo *decInfo)
{
char data[32];
long size=0;
        fread(data,32,1,decInfo->fptr_stego_image);
        for(int i=0;i<32;i++){
            int bit=data[i]&1;
            size|=(bit<<i);
        }
    decInfo->size_secret_data=size;
    return e_success;
}



Status decode_secret_file_extn(DecodeInfo *decInfo)
{
char buffer[8];
decInfo->extn_secret_file = malloc(decInfo->size_extn_file + 1);

for(int i=0;i<decInfo->size_extn_file;i++)
{
    fread(buffer,8,1,decInfo->fptr_stego_image);
    decode_lsb_to_byte(&decInfo->extn_secret_file[i],buffer);
}
decInfo->extn_secret_file[decInfo->size_extn_file]='\0';
return e_success;
}


Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
char data[32];
long size=0;
        fread(data,32,1,decInfo->fptr_stego_image);
        for(int i=0;i<32;i++){
            int bit=data[i]&1;
            size|=(bit<<i);
        }
    decInfo->size_extn_file=size;
    return e_success;
}


Status get_magic_string(DecodeInfo *decInfo)
{
    char pass[3];
de:
                    printf("\033[1;32m Enter magic string (max 2 chars):\033[0m ");
                    scanf("%s",pass);
                    if(strlen(pass)>2){
                    printf("❌ Magic string must be smaller\n");
                    goto de;
                    }
                    strcpy(decInfo->magic_string,pass);
                    return e_success;
}

Status check_magic_string(char *magic_string,DecodeInfo *decInfo)
{
    char buffer[8];
    char decoded_magic[3];   
    decoded_magic[2] = '\0';

    fseek(decInfo->fptr_stego_image,54,SEEK_SET);

    for (int i=0;i<2;i++)
    {
        fread(buffer,8,1,decInfo->fptr_stego_image);
        decode_lsb_to_byte(&decoded_magic[i],buffer);
    }

    //printf("🔍 Decoded magic: %s\n",decoded_magic);

    if (strcmp(decoded_magic, magic_string) == 0)
    {
        printf("✅ Magic string verified!\n");
        return e_success;
    }
    else
    {
        printf("❌ Magic string mismatch!\n");
        return e_failure;
    }
}


    Status decode_lsb_to_byte(char *out_char, char *image_buffer)
{
    unsigned char ch=0;

    for (int i=0;i<8;i++)
    {
        int bit=image_buffer[i]&1;
        ch|=(bit << i);
    }

    *out_char = ch;
    return e_success;
}

    


Status open_files_decode(DecodeInfo *decInfo)
{

    
    // Stego Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "❌ ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

    	return e_failure;
    }
    else
    {
        printf("  ✔️ Stego file %s opened sucessfully\n",decInfo->stego_image_fname);
    }

    return e_success;
}

Status open_secret_file(DecodeInfo *decInfo)
{
    char *dot = strrchr(decInfo->secret_fname, '.');
    if (dot == NULL)
    {
        int newlen = strlen(decInfo->secret_fname) + strlen(decInfo->extn_secret_file) + 1;
        char *newname = malloc(newlen);
        sprintf(newname, "%s%s", decInfo->secret_fname, decInfo->extn_secret_file);
        decInfo->secret_fname = newname;
    }
    else
    {
        FILE *fp = fopen(decInfo->secret_fname, "r");

        if (fp == NULL)
        {
            int base_len = dot - decInfo->secret_fname;
            char *basename = malloc(base_len + 1);
            strncpy(basename, decInfo->secret_fname, base_len);
            basename[base_len] = '\0';

            int newlen = base_len + strlen(decInfo->extn_secret_file) + 1;
            char *newname = malloc(newlen);
            sprintf(newname, "%s%s", basename, decInfo->extn_secret_file);

            decInfo->secret_fname = newname;

            free(basename);
        }
        else
        {
            fclose(fp);
        }
    }

    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
    if (decInfo->fptr_secret == NULL)
    {
        perror("fopen");
        printf("❌ ERROR: Cannot create %s\n", decInfo->secret_fname);
        return e_failure;
    }

    printf("  ✔️  New Secret file %s opened successfully\n", decInfo->secret_fname);
    return e_success;
}