#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
#include<time.h>

#define STREAM (1024*8)
#define BLOCK_SIZE 8
#define NO_OF_ROUDS 5

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
    KeyFile = fopen(keyfile, "r");
    unsigned char *keyStream = (unsigned char *)malloc(STREAM);
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

/*
        Implementation of P-BOX.
INPUT:  input array (It uses a table, defined inside this function).
OUTPUT: An array of permutated input according to the table.
*/
unsigned char *permutation_box(unsigned char *input){           // 8 bit permutation box;
    unsigned char *output = (unsigned char *)malloc(BLOCK_SIZE);
    int table[BLOCK_SIZE] = {0,3,4,6,2,5,1,7};
    int i;
    for(i=0; i<BLOCK_SIZE; i++){
        output[i] = input[table[i]];
    }
    return output;
}

/*
        To convert a 4-bit binary array in decimal number.
INPUT:  input binary array.
OUTPUT: a decimal number.
*/
unsigned char getNumber(unsigned char *input){                  // input 4 bit and get its decimal value;
    unsigned char res = 8*input[0]+4*input[1]+2*input[2]+1*input[3];
    return res;
}


/*
        Implementation of S-BOX.
INPUT:  input array (It uses a table, defined inside this function).
OUTPUT: An array according to the subsitution table.
*/
unsigned char *subsitution_box(unsigned char *input){           // 4 bit substitution box;
    unsigned char *output = (unsigned char *)malloc(4);
    unsigned char *output1 = (unsigned char *)malloc(4);
    unsigned char in = getNumber(input);
    int table[16] = {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7};
    int res = table[in];
    int i;
    for(i=0; i<4; i++)
    {
        output[i] = (res & (0x1<<(i))) ==0 ? 0: 1;
        //printf("%d ", output[i]);
    }
    for(i=0; i<4; i++)
        output1[i] = output[3-i];
    //printf("\nres %d\n", res);
    free(output);
    return output1;
}

/*
        To slice an arry from i to j.
INPUT:  array, i and j.
OUTPUT: sliced array.
*/
unsigned char *slice(unsigned char *input, int i, int j){
    unsigned char *output = (unsigned char *)malloc(BLOCK_SIZE);
    int k=0, l;
    for(l=i; l<j; l++)
        output[k++] = input[l];
    return output;
}

/*
        To concaticate two array.
INPUT:  array1, length1, array2 length2.
OUTPUT: joined array.
*/
unsigned char *concat(unsigned char *input1, int len1, unsigned char *input2, int len2){
    unsigned char *output = (unsigned char *)malloc(len1+len2);
    int i, j=0;
    for(i=0; i<len1; i++)
        output[j++] = input1[i];
    for(i=0; i<len2; i++)
        output[j++] = input2[i];
    return output;
}

/*
        To get the round key.
INPUT:  key, round number.
OUTPUT: key for the given round.
*/
unsigned char *getRoundKey(unsigned char *key, int round){
    unsigned char *output = (unsigned char *)malloc(BLOCK_SIZE);
    int i, j=0;
    for(i=(4*round)-4; i<(4*round)+4; i++)
        output[j++] = key[i];
    return output;
}

/*
        To perform the actions in each round (encryption).
INPUT:  input array, key and round number.
OUTPUT: output array for given round.
*/
unsigned char *each_round(unsigned char *input, unsigned char *key, int round){

    unsigned char *output = (unsigned char *)malloc(BLOCK_SIZE);
    unsigned char *this_key = (unsigned char *)malloc(BLOCK_SIZE);
    int i;
    printf("\n\tROUND--%d\n", round);
    printf("plain \t");
    for(i=0; i<8; i++){
        printf("%d ", input[i]);
    }
    printf("\n");

    //getting round key for this round
    this_key = getRoundKey(key, round);

    printf("Key \t");
    for(i=0; i<BLOCK_SIZE; i++){
        printf("%d ", this_key[i]);
    }
    printf("\n");

    // XOR opeartion 
    unsigned char *after_xor = (unsigned char *)malloc(8);
    printf("\nAfter XOR op\t");
    for(i=0; i<BLOCK_SIZE; i++)
    {
        after_xor[i] = input[i]^this_key[i];
        printf("%d ", after_xor[i]);
    }
    printf("\n");

    // S-BOX
    unsigned char *after_sbox = (unsigned char *)malloc(8);
    unsigned char *sbox1in = (unsigned char *)malloc(4);
    unsigned char *sbox2in = (unsigned char *)malloc(4);
    unsigned char *sbox1out = (unsigned char *)malloc(4);
    unsigned char *sbox2out = (unsigned char *)malloc(4);

    sbox1in = slice(after_xor, 0, 4);
    sbox2in = slice(after_xor, 4, 8);

    sbox1out = subsitution_box(sbox1in);
    sbox2out = subsitution_box(sbox2in);

    after_sbox = concat(sbox1out, 4, sbox2out, 4);
    printf("AFTER S-BOX \t");
    for(i=0; i<BLOCK_SIZE; i++)
        printf("%d ", after_sbox[i]);
    printf("\n");


    // p-box opeartion
    unsigned char *after_pbox = (unsigned char *)malloc(8);
    after_pbox = permutation_box(after_sbox);

    printf("AFTER P-BOX \t");
    for(i=0; i<BLOCK_SIZE; i++)
        printf("%d ", after_pbox[i]);
    printf("\n");

    // Whitning operation with key
    for(i=0; i<BLOCK_SIZE; i++)
    {
        output[i] = after_pbox[i]^this_key[i];
    }

    printf("AFTER WHITE \t");
    for(i=0; i<BLOCK_SIZE; i++)
        printf("%d ", output[i]);
    printf("\n");



    // free all local variables
    free(after_xor);
    free(after_sbox);
    free(after_pbox);
    free(sbox1in);
    free(sbox2in);
    free(sbox1out);
    free(sbox2out);
    free(this_key);

    return output;
}


char *encrypt(unsigned char *plain_text, unsigned char *key){
    int round = 1;
    unsigned char *out = (unsigned char *)malloc(BLOCK_SIZE);
    char *cipher_text = (char *)malloc(2*BLOCK_SIZE);
    out = plain_text;
    for(;round<=NO_OF_ROUDS; round++)
        out = each_round(out, key, round);
    
    int i;
    for(i=0;i<BLOCK_SIZE;i++)
    {
        cipher_text[i]=out[i]==1?'1':'0';
        //cipher_text[i+1]=' ';
    }
    //printf("ABCD%s", cipher_text);
    return cipher_text;
}


int main(void){
    //generateKey("key.txt", 24);
    int i;
    unsigned char *key = loadKey("key.txt");
    for(i=0; i<24; i++)
        printf("%d ", key[i]);
    printf("\n\n");
    unsigned char s[BLOCK_SIZE] = {1, 0, 1, 1, 0, 1, 1, 0};
    char *cip = encrypt(s, key);
    printf("\n%s\n", cip);
    
    return 0;
}