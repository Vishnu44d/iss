#include<stdio.h>
#include<stdlib.h>

#define BUFF_SIZE 1024

char *encrypt(char *plain_text, int Key){
    char *cipher_text = (char*)malloc(BUFF_SIZE);
    int i=0;
    while(plain_text[i]!='\0'){
        int p = plain_text[i]-'a';
        int nextKey=p;
        p = (p+Key)%26;
        //printf("%c\n", (char)p+'a');
        cipher_text[i]=(char)p+'a';
        
        Key=nextKey;
        i++;
    }
    return cipher_text;
}


char *decrypt(char *cipher_text, int Key){
    char *plain_text = (char*)malloc(BUFF_SIZE);
    int i=0;
    while(cipher_text[i]!='\0'){
        int p = cipher_text[i]-'a';
        //int nextKey=p;
        p = (26+p-Key)%26;
        //printf("%c\n", (char)p+'a');
        plain_text[i]=(char)p+'a';
        
        Key=p;
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
    cipher_text = encrypt(palin_text, 13);
    printf("%s\n%s\n%s\n", palin_text, cipher_text, decrypt(cipher_text, 13));
    return 0;
}