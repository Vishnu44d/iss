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


char *encrypt(char *palin_text, int Key)
{
    int i=0;
    char *cipher_text = (char*)malloc(BUFF_SIZE);
    while(palin_text[i]!='\0'){
        int p = palin_text[i]-'a';
        p = (p*Key)%26;
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
    int inv_key = mult_inv(Key, 26);
    printf("INV KEY = %d\n", inv_key);
    while(cipher_text[i]!='\0'){
        int p = cipher_text[i]-'a';
        p = (p*inv_key)%26;
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
    int key;
    scanf("%d", &key);
    if(mult_inv(key, 26)==0)
        printf("<invalid key>\n");
    else
    {
        cipher_text = encrypt(palin_text, key);
        printf("%s\n%s\n%s\n", palin_text, cipher_text, decrypt(cipher_text, key));
    }

    return 0;
}