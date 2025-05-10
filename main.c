#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>




uint64_t c = 1767567;
uint64_t d = 23894;
uint64_t a = (128+1) % 4;
uint64_t N = 1875814;
uint64_t x_one = 0xFFF;


void nkg(uint64_t* key, uint64_t* end){
    for(uint64_t* p = key+1; p < end; p++){
        uint64_t x = *(p-1);
        *p = (d*x*x + a*x + c) % N;
    }
}





void gammirovanie(char* text, char* text_end, void* KEY, void* KEY_end, size_t elem_sz){
        char *p_st = (char*) KEY;
        char *p_end = (char*) KEY_end;
        
        char * key_buf = malloc(elem_sz);
        if(!key_buf){return;}


    for(; p_st < p_end; p_st+=elem_sz){

        memcpy(key_buf, p_st, elem_sz);

        for(int i = 0; (i < elem_sz) && (text < text_end); text++, i++)
        {
            *text ^= key_buf[i];              
        }
                      
    }
    
    free(key_buf);
}

char sk_one(char text_block){
    text_block = ((((text_block >> 6) ^ (text_block >> 2))  & 0x01) << 7); 
    return text_block;
}

char sk_two(char text_block){
    text_block = (((text_block >> 4) ^ (text_block >> 3) ^ (text_block >> 2) & 0x01) << 7);
    return text_block;
}


void skrambler(char* text, char* text_end, char (*sk_pol) (char)){
    for(char *p = text; p < text_end; p++){
        char bit_out;
        for(int i = 0; i < 7; i++){
            char bit = sk_pol(*p);
            bit_out = *p & 0x01;
            *p = (*p >> 1) | bit; 
        }
        *p = bit_out;
    }
}


int main(void){

    char * filetext = "text.txt";
    char * fileshifrtext = "shifr.txt";
    char * filekey = "key.txt";
    char * filedeshifrtext = "deshifr.txt";

    FILE* ftext = fopen(filetext, "r");
    FILE* fshifr = fopen(fileshifrtext, "w");
    FILE* fkey = fopen(filekey, "w");
    FILE* fdeshifr = fopen(filedeshifrtext, "w");

    unsigned char BUFFER[256];
    uint64_t KEY[32];

    KEY[0] = x_one;

    if(!ftext){ 
        fclose(fshifr);
        fclose(fkey);
    }
    else{
        while(fread(BUFFER, sizeof(char), 256, ftext)){
            nkg(KEY, KEY+32);

            fwrite(KEY, sizeof(uint64_t), 32, fkey);
            
            gammirovanie(BUFFER, BUFFER+256, KEY, KEY+32, sizeof(*KEY));

            fwrite(BUFFER, 1, 256, fshifr);
            *KEY = *KEY+31;
        }
    }
    fclose(ftext);
    fclose(fkey);
    fclose(fshifr);
    char KEY_BUF[256];
    fkey = fopen(filekey, "r");
    fshifr = fopen(fileshifrtext, "r");

    if(fkey){
        while (fread(BUFFER, sizeof(char), 256, fshifr) && fread(KEY_BUF, sizeof(char), 256, fkey))
        {
            gammirovanie(BUFFER, BUFFER+256, KEY_BUF, KEY_BUF+256, sizeof(char));
            fwrite(BUFFER, 1, 256, fdeshifr);
        }
    }


    fclose(fshifr);
    fclose(fkey);

    char * fileskoneshifr = "shifrskone.txt";
    char * filesktwoshifr = "shifrsktwo.txt";
    char * fileskonedeshifr = "deshifrskone.txt";
    char * filesktwodeshifr = "deshifrsktwo.txt";


    ftext = fopen(filetext, "r");
    FILE* fskos = fopen(fileskoneshifr, "w");
    FILE* fskts = fopen(filesktwoshifr, "w");



    char (*one) (char) = sk_one;
    char (*two) (char) = sk_two; 

    while(fread(BUFFER, 1, 256, ftext)){
        char tmp_one[256];
        memcpy(tmp_one, BUFFER, sizeof(BUFFER));

        char tmp_two[256];
        memcpy(tmp_two, BUFFER, sizeof(BUFFER));

        skrambler(tmp_one, tmp_one+256, one);
        skrambler(tmp_two, tmp_two+256, two);
        fwrite(tmp_one, 1, 256, fskos);
        fwrite(tmp_two, 1, 256, fskts);
    }

    fclose(ftext);
    fclose(fskos);
    fclose(fskts);

    fskos = fopen(fileskoneshifr, "r");
    FILE* fskod = fopen(fileskonedeshifr, "w");
    
    while(fread(BUFFER, 1, 256, fskos)){
        skrambler(BUFFER, BUFFER+256, one);
        fwrite(BUFFER, 1, 256, fskod);
    }

    fclose(fskos);
    fclose(fskod);

    fskts = fopen(filesktwoshifr, "r");
    FILE* fsktd = fopen(filesktwodeshifr, "w");
            
    while(fread(BUFFER, 1, 256, fskts)){
        skrambler(BUFFER, BUFFER+256, two);
        fwrite(BUFFER, 1, 256, fsktd);
    }

    fclose(fskts);
    fclose(fsktd);







   
  


}