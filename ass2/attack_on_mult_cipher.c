#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define BUFF_SIZE 1024


int mult_inv(int x, int m)
{
	int i;
	x=x%m;
	for(i=1; i<m; i++)
	{
		if((x*i)%m==1)
		{
			return i;
		}
	}
	return 0;
}


char *decrypt(char *cipher_text, int Key)
{
    int i=0;
    char *palin_text = (char*)malloc(BUFF_SIZE);
    int inv_key = mult_inv(Key, 26);
    //printf("INV KEY = %d\n", inv_key);
    while(cipher_text[i]!='\0'){
        int p = cipher_text[i]-'A';
        p = (p*inv_key)%26;
        palin_text[i] = (char)p+'a';
        i++;
    }
    return palin_text;
}

int main(void)
{
    char *cipher_text=(char*)malloc(BUFF_SIZE);
    cipher_text = "WFEJBYOFAJZEYDCMRBKJRKWABKXSWKJZSFQ";
    //scanf("%s", cipher_text);
    int i=0;
    for(;i<26;i++)
    {
        if(i%2 !=0 && i!=13)
        {
            char *palin_text=(char*)malloc(BUFF_SIZE);
            palin_text = decrypt(cipher_text, i);
            printf("%d\t%s\n", i, palin_text);
        }
    }
    return 0;
}