#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>

FILE *KeyFile;


char encryptHelper(char ch)
{
    
    int r = rand()%26;
    int p = ch-'a';
    p = (p+r)%26;
    fprintf(KeyFile, "%d ", r);
    return (char)p+'a';
}

void encrypt(char *plain_text_file, char *key_file)
{
    FILE *InFile, *OutFile ;
    char ch;
    InFile = fopen(plain_text_file, "r");
    OutFile = fopen("cipher_text.txt", "w");
    KeyFile = fopen(key_file, "w");
    
    if (InFile == NULL)
    {
        printf("File is not available \n");
    }
    else
    {
        while ((ch = fgetc(InFile)) != EOF)
        {
            if(ch != '\n' && ch != ' ')
            {
                ch = tolower(ch);
                char c = encryptHelper(ch);
                fprintf(OutFile, "%c", c);
                //printf("%c", ch);
            }
            //printf("%c", ch);
        }
    }
    fclose(InFile);
    fclose(OutFile);
    fclose(KeyFile);
}


void decrypt(char *cipher_file_name, char *key_file)
{
    FILE *InFile, *OutFile ;
    char ch;
    InFile = fopen(cipher_file_name, "r");
    OutFile = fopen("plain_text_decrypted.txt", "w");
    KeyFile = fopen(key_file, "r");
    if (InFile == NULL || KeyFile == NULL)
    {
        printf("File is not available \n");
    }
    else
    {
        while ((ch = fgetc(InFile)) != EOF)
        {
            int k;
            fscanf(KeyFile, "%d ", &k);
            //printf("%d ", k);
            int p = ch-'a';
            p = (26+p-k)%26;
            fprintf(OutFile, "%c", (char)p+'a');
        }
    }
    fclose(InFile);
    fclose(OutFile);
    fclose(KeyFile);
}

int main(void)
{
    
    srand(time(NULL));
    encrypt("plain_text.txt", "key.txt");
    decrypt("cipher_text.txt", "key.txt");
    return 0;
    
}
