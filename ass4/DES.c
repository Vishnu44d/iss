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
    for(i=0; i<out_size; i++){
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

unsigned char *leftShift(unsigned char *input, int shift){
    unsigned char *output = (unsigned char *)malloc(28);
    int i, j;
    for(i=0; i<28-shift; i++){
        output[i]=input[i+shift];
    }
    for(j=0; j<shift; j++)
        output[i++]= input[j];
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

    int j;
    unsigned char *after_parity_drop = parityDrop(key);

    unsigned char *left = (unsigned char *)malloc(28);
    unsigned char *right = (unsigned char *)malloc(28);
    unsigned char *after_shift_concat = (unsigned char *)malloc(56);
    // slicing the 56 bit key into 2 28 bit
    
    left = slice(after_parity_drop, 0, 28);
    right = slice(after_parity_drop, 28, 56);

    //left shifting number of round times both
    left = leftShift(left, round);
    right = leftShift(right, round);

    after_shift_concat = concat(left, 28, right, 28);
    free(left);
    free(right);

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
    
    for(j=0; j<48; j++)
        table[j]-=1;
    unsigned char *output = (unsigned char *)malloc(48);
    output = permutation_box(after_shift_concat, table, 56, 48);
    free(after_shift_concat);
    return output;
}


unsigned char *s_box(unsigned char *input, int table[4][16]){  // input is 6 bit long
    int row, col;
    row = 2*input[0]+1*input[5];
    col = 8*input[1]+4*input[2]+2*input[3]+1*input[4];
    int res = table[row][col];
    unsigned char *output = (unsigned char *)malloc(4);
    int i;
    for(i=4; i>=0; i--)
    {
        output[i] = (res & (0x1<<(i))) ==0 ? 0: 1;
    }
    return output;
}

unsigned char *des_function(unsigned char*input, unsigned char *key){
    int expansion_table[48] = {
        32,	1,	2,	3,	4,	5,
        4,	5,	6,	7,	8,	9,
        8,	9,	10,	11,	12,	13,
        12,	13,	14,	15,	16,	17,
        16,	17,	18,	19,	20,	21,
        20,	21,	22,	23,	24,	25,
        24,	25,	26,	27,	28,	29,
        28,	29,	30,	31,	32,	1
    };
    unsigned char *after_expansion = (unsigned char *)malloc(48);
    unsigned char *after_xor = (unsigned char *)malloc(48);
    after_expansion = permutation_box(input, expansion_table, 32, 48);
    
    int i;
    for(i=0; i<48; i++)
        after_xor[i] = after_expansion[i]^key[i];
    free(after_expansion);

    int s1_table[4][16] = {
        {14,	4,	13,	1,	2,	15,	11,	8,	3,	10,	6,	12,	5,	9,	0,	7},
        {0,	15,	7,	4,	14,	2,	13,	1,	10,	6,	12,	11,	9,	5,	3,	8},
        {4,	1,	14,	8,	13,	6,	2,	11,	15,	12,	9,	7,	3,	10,	5,	0},
        {15,	12,	8,	2,	4,	9,	1,	7,	5,	11,	3,	14,	10,	0,	6,	13}
    };
    int s2_table[4][16] = {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    };
    int s3_table[4][16] = {
        {10, 0,  9,  14, 6,  3,  15, 5 , 1,  13, 12, 7,  11, 4,  2,  8},
        {13, 7,  0,  9,  3,  4,  6,  10, 2,  8 , 5,  14, 12, 11, 15, 1},
        {13, 6,  4,  9,  8,  15, 3,  0 , 11, 1 , 2,  12, 5,  10, 14, 7},
        {1 , 10, 13, 0,  6,  9,  8,  7 , 4,  15, 14, 3,  11, 5,  2,  12}
    };
    int s4_table[4][16] = {
        {7,  13, 14, 3,  0,  6,  9,  10, 1,  2,  8,  5,  11, 12, 4,  15},
        {13, 8,  11, 5,  6,  15, 0,  3,  4,  7,  2,  12, 1,  10, 14, 9},
        {10, 6,  9,  0,  12, 11, 7,  13, 15, 1,  3,  14, 5,  2,  8,  4},
        {3,  15, 0,  6,  10, 1,  13, 8,  9,  4,  5,  11, 12, 7,  2,  14}
    };
    int s5_table[4][16] = {
        {2,  12, 4,  1,  7,  10, 11, 6,  8,  5,  3,  15, 13, 0,  14, 9},
        {14, 11, 2,  12, 4,  7,  13, 1,  5,  0,  15, 10, 3,  9,  8,  6},
        {4,  2,  1,  11, 10, 13, 7,  8,  15, 9,  12, 5,  6,  3,  0,  14},
        {11, 8,  12, 7,  1,  14, 2,  13, 6,  15, 0,  9,  10, 4,  5,  3}
    };
    int s6_table[4][16] = {
        {12, 1,  10, 15, 9,  2,  6,  8,  0,  13, 3,  4,  14, 7,  5,  11},
        {10, 15, 4,  2,  7,  12, 9,  5,  6,  1,  13, 14, 0,  11, 3,  8},
        {9,  14, 15, 5,  2,  8,  12, 3,  7,  0,  4,  10, 1,  13, 11, 6},
        {4,  3,  2,  12, 9,  5,  15, 10, 11, 14, 1,  7,  6,  0,  8,  13}
    };
    int s7_table[4][16] = {
        {4,  11, 2,  14, 15, 0,  8,  13, 3,  12, 9,  7,  5,  10, 6,  1},
        {13, 0,  11, 7,  4,  9,  1,  10, 14, 3,  5,  12, 2,  15, 8,  6},
        {1,  4,  11, 13, 12, 3,  7,  14, 10, 15, 6,  8,  0,  5,  9,  2},
        {6,  11, 13, 8,  1,  4,  10, 7,  9,  5,  0,  15, 14, 2,  3,  12}
    };
    int s8_table[4][16] = {
        {13, 2,  8,  4,  6,  15, 11, 1,  10, 9,  3,  14, 5,  0,  12, 7},
        {1,  15, 13, 8,  10, 3,  7,  4,  12, 5,  6,  11, 0,  14, 9,  2},
        {7,  11, 4,  1,  9,  12, 14, 2,  0,  6,  10, 13, 15, 3,  5,  8},
        {2,  1,  14, 7,  4,  10, 8,  13, 15, 12, 9,  0,  3,  5,  6,  11}
    };
    unsigned char *s1_in = (unsigned char *)malloc(4);
        s1_in = slice(after_xor, 0, 6);
    unsigned char *s1_out = (unsigned char *)malloc(4);
        s1_out = s_box(s1_in, s1_table);

    unsigned char *s2_in = (unsigned char *)malloc(4);
        s2_in = slice(after_xor, 6, 12);
    unsigned char *s2_out = (unsigned char *)malloc(4);
        s2_out = s_box(s2_in, s2_table);

    unsigned char *s3_in = (unsigned char *)malloc(4);
        s3_in = slice(after_xor, 12, 18);
    unsigned char *s3_out = (unsigned char *)malloc(4);
        s3_out = s_box(s3_in, s3_table);

    unsigned char *s4_in = (unsigned char *)malloc(4);
        s4_in = slice(after_xor, 18, 24);
    unsigned char *s4_out = (unsigned char *)malloc(4);
        s4_out = s_box(s4_in, s4_table);

    unsigned char *s5_in = (unsigned char *)malloc(4);
        s5_in = slice(after_xor, 24, 30);
    unsigned char *s5_out = (unsigned char *)malloc(4);
        s5_out = s_box(s5_in, s5_table);

    unsigned char *s6_in = (unsigned char *)malloc(4);
        s6_in = slice(after_xor, 30, 36);
    unsigned char *s6_out = (unsigned char *)malloc(4);
        s6_out = s_box(s6_in, s6_table);

    unsigned char *s7_in = (unsigned char *)malloc(4);
        s7_in = slice(after_xor, 36, 42);
    unsigned char *s7_out = (unsigned char *)malloc(4);
        s7_out = s_box(s7_in, s7_table);

    unsigned char *s8_in = (unsigned char *)malloc(4);
        s8_in = slice(after_xor, 42, 48);
    unsigned char *s8_out = (unsigned char *)malloc(4);
        s8_out = s_box(s8_in, s8_table);
    
    unsigned char *join = (unsigned char *)malloc(32);
    join = concat(s1_out, 4, s2_out, 4);
    join = concat(join, 8, s3_out, 4);
    join = concat(join, 12, s4_out, 4);
    join = concat(join, 16, s5_out, 4);
    join = concat(join, 20, s6_out, 4);
    join = concat(join, 24, s7_out, 4);
    join = concat(join, 28, s8_out, 4);

    free(s1_in);
    free(s2_in);
    free(s3_in);
    free(s4_in);
    free(s5_in);
    free(s6_in);
    free(s7_in);
    free(s8_in);

    free(s1_out);
    free(s2_out);
    free(s3_out);
    free(s4_out);
    free(s5_out);
    free(s6_out);
    free(s7_out);
    free(s8_out);

    int p_table[32] = {
        16, 7,  20, 21, 29, 12, 28, 17,
        1,  15, 23, 26, 5,  18, 31, 10,
        2,  8,  24, 14, 32, 27, 3,  9,
        19, 13, 30, 6,  22, 11, 4,  25
    };

    unsigned char *output = (unsigned char *)malloc(32);
    output = permutation_box(join, p_table, 32, 32);

    return output;
}

unsigned char *each_round(unsigned char *input, unsigned char *key, int round){
    unsigned char *left = (unsigned char *)malloc(32);
    unsigned char *right = (unsigned char *)malloc(32);
    unsigned char *this_key = (unsigned char *)malloc(48);
    this_key = getRoundKey(key, round);
    left = slice(input, 0, 32);
    right = slice(input, 32, 64);
    unsigned char *from_des_funct = (unsigned char *)malloc(32);
    from_des_funct = des_function(right, this_key);
    unsigned char *after_xor = (unsigned char *)malloc(32);
    int i;
    for(i=0; i<32; i++){
        after_xor[i] = left[i]^from_des_funct[i];
    }
    unsigned char *swapped = (unsigned char *)malloc(64);
    swapped = concat(right, 32, left, 32);


    free(left);
    free(right);
    free(this_key);
    free(from_des_funct);
    free(after_xor);
    
    return swapped;

}




int main(void){
    //generateKey("DESkey.txt", 64);
    int i;
    unsigned char *key = loadKey("DESkey.txt");
    for(i=0; i<64; i++)
        printf("%d ", key[i]);
    printf("\n\n");
    
    unsigned char *k = getRoundKey(key, 1);
    printf("\nKEY\t");
    for(i=0; i<48; i++)
        printf("%d ", k[i]);
    
    return 0;
}