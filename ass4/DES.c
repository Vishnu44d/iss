#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
#include<time.h>

#define STREAM (1024*8)
#define BLOCK_SIZE 64
#define NO_OF_ROUDS 16

/*
        To generate a random key.
INPUT:  File name to store key and n for key length.
OUTPUT: Write a random bit of length n in provided file.
*/
void generateKey(char *keyfile, int n){
    FILE *KeyFile;
    KeyFile = fopen(keyfile, "w");
    srand(time(NULL));
    int i;
    for(i=0; i<n; i++)
    {
        int k = rand()%2;
        fprintf(KeyFile, "%d ", k);
    }
    fclose(KeyFile);
}


/*
        To load a key from a file.
INPUT:  File name of stored key.
OUTPUT: Read the file and load them in array and returns the array.
*/
unsigned char *loadKey(char *keyfile){
    FILE *KeyFile;
    //printf("LOADING KEY..");
    KeyFile = fopen(keyfile, "r");
    unsigned char *keyStream = (unsigned char *)malloc(64);
    int i=0;
    while(!feof(KeyFile)){
        int k;
        
        fscanf(KeyFile, "%d ", &k);
        //printf("%d \n", k);
        keyStream[i++] = k;
    }
    fclose(KeyFile);
    return keyStream;
}

unsigned char *permutation_box(unsigned char *input, int *table, int input_size, int out_size){           // 64 bit straight permutation box;
    unsigned char *output = (unsigned char *)malloc(out_size);
    //int table[BLOCK_SIZE] = {0,3,4,6,2,5,1,7};
    int i, j=0;
    for(i=0; i<input_size; i++){
        output[j++] = input[table[i]];
    }
    return output;
}

unsigned char *parityDrop(unsigned char *key){
    unsigned char *parityDropKey = (unsigned char *)malloc(56);            // 56 bit key
    int i, j=0;
    for(i=0; i<64; i++){
        
        if(i%8!=7){
            parityDropKey[j++]=key[i];
        }
        else{
            //printf("Dropping:: %d, %d ",i, key[i]);
        }
    }
    // printf("After drop\n\n");
    // for(i=0; i<56; i++)
    //     printf("%d ", parityDropKey[i]);
    return parityDropKey;
}

unsigned char *slice(unsigned char *input, int i, int j){
    unsigned char *output = (unsigned char *)malloc(BLOCK_SIZE);
    int k=0, l;
    for(l=i; l<j; l++)
        output[k++] = input[l];
    return output;
}

unsigned char *leftShift(unsigned char *input){
    unsigned char *output = (unsigned char *)malloc(28);
    int i;
    for(i=0; i<27; i++){
        output[i]=input[i+1];
    }
    output[27]= input[0];
    return output;
}

unsigned char *concat(unsigned char *input1, int len1, unsigned char *input2, int len2){
    unsigned char *output = (unsigned char *)malloc(len1+len2);
    int i, j=0;
    for(i=0; i<len1; i++)
        output[j++] = input1[i];
    for(i=0; i<len2; i++)
        output[j++] = input2[i];
    return output;
}

unsigned char *getRoundKey(unsigned char *key, int round){

    printf("inside getround key\n");
    unsigned char *after_parity_drop = parityDrop(key);

    printf("After drop parity %d\n", after_parity_drop[55]);
    // int j;
    // for(j=0; j<56; j++)
    //     printf("%d ", after_parity_drop[j]);

    unsigned char *left = (unsigned char *)malloc(28);
    unsigned char *right = (unsigned char *)malloc(28);
    unsigned char *after_shift_concat = (unsigned char *)malloc(56);
    // slicing the 56 bit key into 2 28 bit
    
    left = slice(after_parity_drop, 0, 28);
    // printf("\nLeft half\n");
    // for(j=0; j<28; j++)
    //     printf("%d ", left[j]);
    right = slice(after_parity_drop, 28, 56);
    // printf("\nRight half\n");
    // for(j=0; j<28; j++)
    //     printf("%d ", right[j]);

    //left shifting both
    
    left = leftShift(left);
    // printf("\nLeft half shift\n");
    // for(j=0; j<28; j++)
    //     printf("%d ", left[j]);
    right = leftShift(right);
    // printf("\nRight half shift\n");
    // for(j=0; j<28; j++)
    //     printf("%d ", right[j]);

    
    after_shift_concat = concat(left, 28, right, 28);
    free(left);
    free(right);
    // printf("After concatinating\n");
    // for(j=0; j<56; j++)
    //     printf("%d ", after_shift_concat[j]);
    // compression p-box
    int table[48] = {
        14,  17,  11,  24,   1,   5,
        3 , 28 , 15 ,  6 , 21 , 10,
        23,  19,  12,   4,  26,   8,
        16,   7,  27,  20,  13,   2,
        41,  52,  31,  37,  47,  55,
        30,  40,  51,  45,  33,  48,
        44,  49,  39,  56,  34,  53,
        46,  42,  50,  36,  29,  32
    };
    printf("%d ", table[47]);
    int j;
    for(j=0; j<48; j++)
        table[j]-=1;
    printf("\n");
    for(j=0; j<48; j++)
        printf("%d ", table[j]);
    unsigned char *output = (unsigned char *)malloc(48);
    //output = permutation_box(after_shift_concat, table, 56, 48);
    free(after_shift_concat);
    //unsigned char *output = (unsigned char *)malloc(48);
    return output;
}

int main(void){
    //generateKey("DESkey.txt", 64);
    int i;
    unsigned char *key = loadKey("DESkey.txt");
    for(i=0; i<64; i++)
        printf("%d ", key[i]);
    printf("\n\n");
    
    unsigned char *k = getRoundKey(key, 1);
    for(i=0; i<48; i++)
        printf("%d ", k[i]);
    
    return 0;
}