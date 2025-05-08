#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <malloc.h>
#include <stdlib.h>



uint64_t c = 1767567;
uint64_t d = 128;
uint64_t a = (128+1) % 4;
uint64_t N = 1875814;
uint64_t x_one = 18967381;


void nkg(uint64_t* key, uint64_t* end){
    for(uint64_t* p = key+1; p < end; p++){
        uint64_t x = *(p-1);
        *p = (d*x*x + a*x + c) % N;
    }
}





// void gammirovanie(char* text, char* text_end, uint64_t* KEY, uint64_t* KEY_end){


// }


int main(void){

    char * filetext = "text.txt";
    char * fileshifrtext = "shifr.txt";
    char * filekey = "key.txt";
    char * filedeshifrtext = "deshifr.txt";

    FILE* ftext = fopen(filetext, "r");
    FILE* fshifr = fopen(fileshifrtext, "w");
    FILE* fkey = fopen(filekey, "w");

    char BUFFER[256];
    uint64_t KEY[32];

    KEY[0] = x_one;

    if(!ftext || !fshifr || !fkey){ goto EXIT;}
    else{
        size_t bytes_read;
        while(fread(BUFFER, sizeof(char), 256, ftext)){
            nkg(KEY, KEY+32);

            fwrite(KEY, sizeof(uint64_t), 32, fkey);
            
            for(int i = 0,  j = 0; i < 32; i++, j+=8){
                BUFFER[j] ^= KEY[i];                
            }

            fwrite(BUFFER, 1, 256, fshifr);
            *KEY = *KEY+31;
        }
    }








    fclose(ftext);
    fclose(fshifr);
    fclose(fkey);
  

    EXIT:
        return 0;
}