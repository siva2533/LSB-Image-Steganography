#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "encode.h"
#include "types.h"


void warn()
{
    printf("Usage: \n\n");
    printf("For encoding : ./a.out -e <source_imgage> <secret_file> <Output_image>\n");
    printf("For decoding : ./a.out -d <stego_imgage> <output_file>\n");
}


int main(int argc,char *argv[])
{
    if(argc<2)
    {
      printf("❌ Error, Invalid arguments!\n");
      warn();
      return 0;  
    }
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    
                    
  
        if(check_operation_type(argv)==e_encode)
        {
                    printf("\n");
                    printf("╔══════════════════════════════╗\n");
                    printf("║ \033[32m--Encoding process started--\033[0m ║\n");
                    printf("╚══════════════════════════════╝\n\n");
             printf("🔍 Checking all files...\n");
            
            if(read_and_validate_encode_args(argv,&encInfo)==e_success)
            {
                if(do_encoding(&encInfo)==e_failure)
                {
                    fprintf(stderr, "❌ ERROR: Unable to open %s\n","file do_encoding()");
                    return 1;
                }
                else
                {
                    printf("\n");
                    printf("╔══════════════════════════════╗\n");
                    printf("║  \033[32mData encoded successfully!\033[0m  ║\n");
                    printf("╚══════════════════════════════╝\n\n");
                    fclose(encInfo.fptr_src_image);
                    fclose(encInfo.fptr_secret);
                    fclose(encInfo.fptr_stego_image);
                    return 0;
                } 
            }
            else{
                fprintf(stderr, "❌ ERROR: Unable to open file %s\n","read_and_validate_encode_args()");
                return 1;
            }
        }
        else if(check_operation_type(argv)==e_decode)
        {
                    printf("\n");
                    printf("╔══════════════════════════════╗\n");
                    printf("║ \033[32m--Decoding process started--\033[0m ║\n");
                    printf("╚══════════════════════════════╝\n\n");
                    printf("🔍 Checking all files...\n");
            if(read_and_validate_decode_args(argv,&decInfo)==e_success)
            {
                if(do_decoding(&decInfo)==e_success)
                {
                    printf("\n");
                    printf("╔══════════════════════════════╗\n");
                    printf("║  \033[32mData decoded successfully!\033[0m  ║\n");
                    printf("╚══════════════════════════════╝\n\n");
                    fclose(decInfo.fptr_stego_image);
                    fclose(decInfo.fptr_secret);
                    
                    return 0;
                }
                else
                {
                    fprintf(stderr, "❌ ERROR: Unable to open %s\n","file do_decoding()");
                    return 1;
                }
            }
            else{
                fprintf(stderr, "❌ ERROR: Unable to open file %s\n","read_and_validate_decode_args()");
                return 1;
            }

        }

        else if(check_operation_type(argv)==e_unsupported)
        {
        printf("❌ Error, Invalid arguments!\n");
        warn();
        return 0;  
        }
    
   

}    
 
    
    