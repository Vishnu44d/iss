#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define BUFF_SIZE 1024


char Key[5][5] = {
            {'a', 'f', 'j', 'm', 'o'},
            {'p', 'b', 'g', 'k', 'n'},
            {'t', 'q', 'c', 'h', 'l'},
            {'w', 'u', 'r', 'd', 'i'},
            {'y', 'x', 'v', 's', 'e'}
        };



void getIndex(char c, int *m, int *n)
{
    int i, j;
    for(i=0; i<5; i++)
    {
        for(j=0; j<5; j++)
        {
            if(Key[i][j] == c)
            {
                *m = i;
                *n = j;
            }
        }
    }
}


char *encrypt(char *plain_text){
    char *cipher_text = (char*)malloc(BUFF_SIZE);
    int ind1i=-1, ind1j=-1;
    int ind2i=-1, ind2j=-1;
    int i=0;
    while(plain_text[i] != '\0'){

        getIndex(plain_text[i], &ind1i, &ind1j);
        getIndex(plain_text[i+1], &ind2i, &ind2j);

        if(ind1i==ind2i){
            //same row
            cipher_text[i] = Key[ind1i][(ind1j+1)%5];
            cipher_text[i+1] = Key[ind1i][(ind2j+1)%5];
        }
        else if(ind1j==ind2j){
            //same col
            cipher_text[i] = Key[(ind1i+1)%5][ind1j];
            cipher_text[i+1] = Key[(ind2i+1)%5][ind1j];
        }
        else{
            //different row different col
            cipher_text[i] = Key[ind1i][ind2j];
            cipher_text[i+1] = Key[ind2i][ind1j];
        }
        ind1i=-1;
        ind1j=-1;
        ind2i=-1;
        ind2j=-1;
        i+=2;
    }
    return cipher_text;
}


char *decrypt(char *cipher_text){
    char *plain_text = (char*)malloc(BUFF_SIZE);
    int ind1i=-1, ind1j=-1;
    int ind2i=-1, ind2j=-1;
    int i=0;
    do{

        getIndex(cipher_text[i], &ind1i, &ind1j);
        getIndex(cipher_text[i+1], &ind2i, &ind2j);

        if(ind1i==ind2i){
            //same row
            plain_text[i] = Key[ind1i][(ind1j-1)%5];
            plain_text[i+1] = Key[ind2i][(ind2j-1)%5];
        }
        else if(ind1j==ind2j){
            //same col
            plain_text[i] = Key[(5+ind1i-1)%5][ind1j];
            plain_text[i+1] = Key[(5+ind2i-1)%5][ind2j];
        }
        else{
            //different row different col
            plain_text[i+1] = Key[ind2i][ind1j];
            plain_text[i] = Key[ind1i][ind2j];
        }
        ind1i=-1;
        ind1j=-1;
        ind2i=-1;
        ind2j=-1;
        i+=2;
    }while(cipher_text[i] != '\0');
    //plain_text[i-1]='\0';
    return plain_text;
}


int main(void)
{
    char *palin_text=(char*)malloc(BUFF_SIZE);
    char *cipher_text=(char*)malloc(BUFF_SIZE);
    scanf("%s", palin_text);
    
    if(strlen(palin_text)%2 !=0 )
        strcat(palin_text, "x"); 
    //palin_text = strlwr(palin_text);
    cipher_text = encrypt(palin_text);
    printf("%s\n%s\n%s\n", palin_text, cipher_text, decrypt(cipher_text));
    return 0;
}