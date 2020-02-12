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

// key1 for mult and key2 for add

char *encrypt(char *palin_text, int Key1, int Key2)
{
    int i=0;
    char *cipher_text = (char*)malloc(BUFF_SIZE);
    while(palin_text[i]!='\0'){
        int p = palin_text[i]-'a';
        p = (p*Key1 + Key2)%26;
        //printf("%c\n", (char)p+'a');
        cipher_text[i]=(char)p+'a';
        i++;
    }
    
    return cipher_text;
}

char *decrypt(char *cipher_text, int Key1, int Key2)
{
    int i=0;
    char *palin_text = (char*)malloc(BUFF_SIZE);
    int inv_key = mult_inv(Key1, 26);
    printf("INV KEY = %d\n", inv_key);
    while(cipher_text[i]!='\0'){
        int p = cipher_text[i]-'a';
        p = 26+p-Key2;
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
    int k1, k2;
    scanf("%d %d", &k1, &k2);
    int inv = mult_inv(k1, 26);
    if(inv == 0)
        printf("<invalid key>\n");
    else
    {
        cipher_text = encrypt(palin_text, k1, k2);
        printf("%s\n%s\n%s\n", palin_text, cipher_text, decrypt(cipher_text, 3, 4));
    }
    

    return 0;
}