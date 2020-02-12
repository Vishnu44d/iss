#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define BUFF_SIZE 1024

char *encrypt(char *palin_text, int Key)
{
    int i=0;
    char *cipher_text = (char*)malloc(BUFF_SIZE);
    while(palin_text[i]!='\0'){
        int p = palin_text[i]-'a';
        p = (p+Key)%26;
        //printf("%c\n", (char)p+'a');
        cipher_text[i]=(char)p+'a';
        i++;
    }
    //cipher_text[i]='\0';
    //printf("%s\n", cipher_text);
    return cipher_text;
}

char *decrypt(char *cipher_text, int Key)
{
    int i=0;
    char *palin_text = (char*)malloc(BUFF_SIZE);
    while(cipher_text[i]!='\0'){
        int p = cipher_text[i]-'a';
        p = (26+p-Key)%26;
        palin_text[i] = (char)p+'a';
        i++;
    }
    return palin_text;
}

int main(void)
{
    char *palin_text=(char*)malloc(BUFF_SIZE);
    char *cipher_text=(char*)malloc(BUFF_SIZE);
    scanf("%s", palin_text);
    //palin_text = strlwr(palin_text);
    cipher_text = encrypt(palin_text, 4);
    printf("%s\n%s\n%s\n", palin_text, cipher_text, decrypt(cipher_text, 4));

    return 0;
}