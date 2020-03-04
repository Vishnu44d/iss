#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STREAM (1024*8)


char* stringToBinary(char* s) {
    if(s == NULL) 
        return 0; 
    size_t len = strlen(s);
    char *binary = malloc(len*8 + 1); 
    binary[0] = '\0';
    for(size_t i = 0; i < len; ++i) {
        char ch = s[i];
        for(int j = 7; j >= 0; --j){
            if(ch & (1 << j)) {
                strcat(binary,"1");
            } else {
                strcat(binary,"0");
            }
        }
    }
    return binary;
}

unsigned char *generateKey(int length){
    //printf("length:: %d\n", length);
    unsigned char *key = (unsigned char *)malloc(length*8+1);
    key[0] = 1;   //p3
    key[1] = 1;   //p2
    key[2] = 0;   //p1
    key[3] = 0;   //p0
    int k;
    for(k=4; k<length; k++)
    {
        key[k] = key[k-4]^key[k-3]^key[k-2]^key[k-1];
        //printf("%01x\n", s[k]);
    }
    //printf("key:: %d\n", key[31]);
    return key;
}

char *binaryToString(unsigned char *binaryArray, int len){
    char *text = (char *)malloc(len/8+1);
    int i, j=0;
    for(i=0; i<len; i+=8){
        int p = 128*binaryArray[i+0]+
            64*binaryArray[i+1]+
            32*binaryArray[i+2]+
            16*binaryArray[i+3]+
            8*binaryArray[i+4]+
            4*binaryArray[i+5]+
            2*binaryArray[i+6]+
            1*binaryArray[i+7];
        text[j++] = (char)p;
    }
    return text;
}


int main(int argc, char argv[]) {

        unsigned char plain_stream[STREAM], cipher_stream[STREAM];
        int k;

        unsigned char *key = (unsigned char *)malloc(STREAM);
        char *plain_text = "abcd";
        char *binary_text = stringToBinary(plain_text);
        printf("%s\n", binary_text);

        int len = strlen(plain_text);
        int i;
        len = len*8;
        for(i=0; i<len; i++)
        {
            plain_stream[i] = binary_text[i]=='1'?1:0;
        }



        //printf("\n\n%s\n\n", binaryToString(plain_stream, len));

        key = generateKey(len);

        //encryption
        for(k=0;k<len;k++){
            cipher_stream[k] = plain_stream[k]^key[k];
        }

        printf("P\t");
        for(i=0; i<len; i++)
            printf("%d ",plain_stream[i]);
        printf("\nK\t");
        for(i=0; i<len; i++)
            printf("%d ",key[i]);
        printf("\nC\t");
        for(i=0; i<len; i++)
            printf("%d ",cipher_stream[i]);
        printf("\n");

        printf("Cipher Text:: %s\n", binaryToString(cipher_stream, len));
        return 0;

}