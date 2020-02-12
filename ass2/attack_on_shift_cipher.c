#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define BUFF_SIZE 1024

char *decrypt(char *cipher_text, int Key)
{
    int i=0;
    char *palin_text = (char*)malloc(strlen(cipher_text));
    while(cipher_text[i]!='\0'){
        int p = cipher_text[i]-'A';
        p = (26+p-Key)%26;
        palin_text[i] = (char)p+'a';
        i++;
    }
    return palin_text;
}

int main(void)
{
    char *cipher_text=(char*)malloc(BUFF_SIZE);
    cipher_text = "BMMTDXLTANZXXYYHKMMHYKXXRHNKLXEYYKHFFXFHKR";
    //scanf("%s", cipher_text);
    int i=0;
    for(;i<26;i++)
    {
        char *palin_text=(char*)malloc(BUFF_SIZE);
        palin_text = decrypt(cipher_text, i);
        printf("%d\t%s\n", i, palin_text);

    }
    return 0;
}