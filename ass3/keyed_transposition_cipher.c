#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define BUFF_SIZE 1024
#define KEY_SIZE 5

int Key[2][KEY_SIZE] = {
    {1, 2, 3, 4, 0},
    {4, 0, 1, 2, 3}
};


char *encryptHelper(char *plain_block)
{
    char *cipher_block = (char*)malloc(KEY_SIZE*sizeof(char));
    int i;
    for(i=0; i<KEY_SIZE; i++)
    {
        //printf("%d ", Key[1][i]);
        cipher_block[i] = plain_block[Key[0][i]];
    }
    return cipher_block;
}

char *encrypt(char *plain_text){
    char *cipher_text = (char*)malloc(BUFF_SIZE);
    int i=0, j=0;
    char *plain_block = (char *)malloc(KEY_SIZE*sizeof(char));
    while(plain_text[i]!='\0'){
        for(j=0;j<KEY_SIZE;j++)
        {
            plain_block[j]=plain_text[i+j];
        }
        if(strlen(plain_block)<KEY_SIZE)
        {
            int t;
            for(t=strlen(plain_block); t<KEY_SIZE;t++)
                plain_block[t]='z';
        }
        strcat(cipher_text, encryptHelper(plain_block));
        i+=KEY_SIZE;
        //printf("%s\n", plain_block);
    }
    return cipher_text;
}


char *decryptHelper(char *cipher_block)
{
    char *plain_block = (char*)malloc(KEY_SIZE*sizeof(char));
    int i;
    for(i=0; i<KEY_SIZE; i++)
    {
        //printf("%d ", Key[1][i]);
        plain_block[i] = cipher_block[Key[1][i]];
    }
    return plain_block;
}

char *decrypt(char *cipher_text){
    char *plain_text = (char*)malloc(BUFF_SIZE);
    int i=0, j=0;
    char *cipher_block = (char *)malloc(KEY_SIZE*sizeof(char));
    while(cipher_text[i]!='\0'){
        for(j=0;j<KEY_SIZE;j++)
        {
            cipher_block[j]=cipher_text[i+j];
        }
        if(strlen(cipher_block)<KEY_SIZE)
        {
            int t;
            for(t=strlen(cipher_block); t<KEY_SIZE;t++)
                cipher_block[t]='z';
        }
        strcat(plain_text, decryptHelper(cipher_block));
        i+=KEY_SIZE;
        //printf("%s\n", plain_block);
    }
    return plain_text;
}

int main(void)
{
    char *palin_text=(char*)malloc(BUFF_SIZE);
    char *cipher_text=(char*)malloc(BUFF_SIZE);
    //printf("%s\n", encrypt("abcdefghijkl"));
    scanf("%s", palin_text);
    cipher_text = encrypt(palin_text);
    printf("%s\n%s\n%s\n", palin_text, cipher_text, decrypt(cipher_text));
    return 0;
}