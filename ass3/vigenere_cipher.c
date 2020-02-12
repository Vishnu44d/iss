#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define BUFF_SIZE 1024

char *encrypt(char *plain_text, char *Key){
    char *cipher_text = (char*)malloc(BUFF_SIZE);
    int i=0;
    int len_of_key = strlen(Key);
    while(plain_text[i]!='\0'){
        int p = plain_text[i]-'a';
        int K=Key[i%len_of_key]-'A';
        //printf("\n%d\n", K);
        p = (p+K)%26;
        //printf("%c\n", (char)p+'a');
        cipher_text[i]=(char)p+'a';
        i++;
    }
    return cipher_text;
}


char *decrypt(char *cipher_text, char *Key){
    char *plain_text = (char*)malloc(BUFF_SIZE);
    int i=0;
    int len_of_key = strlen(Key);
    while(cipher_text[i]!='\0'){
        int p = cipher_text[i]-'a';
        int K=Key[i%len_of_key]-'A';
        p = (26+p-K)%26;
        //printf("%c\n", (char)p+'a');
        plain_text[i]=(char)p+'a';

        i++;
    }
    return plain_text;
}


int main(void)
{
    char *palin_text=(char*)malloc(BUFF_SIZE);
    char *cipher_text=(char*)malloc(BUFF_SIZE);
    scanf("%s", palin_text);
    //palin_text = strlwr(palin_text);
    cipher_text = encrypt(palin_text, "BCDCGSDJH");
    printf("%s\n%s\n%s\n", palin_text, cipher_text, decrypt(cipher_text, "BCDCGSDJH"));
    return 0;
}