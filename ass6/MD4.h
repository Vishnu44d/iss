/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MD4.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Vishnu <vishnu44d@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/03 00:41:43 by Vishnu            #+#    #+#             */
/*   Updated: 2020/06/03 00:42:56 by Vishnu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *MD4(char *str, int len);

typedef struct string
{
    char *c;
    int len;
    char sign;
} string;

// defining registers
static uint32_t A = 0x67452301;
static uint32_t B = 0xefcdab89;
static uint32_t C = 0x98badcfe;
static uint32_t D = 0x10325476;

// macros of three function used in three rounds
#define F(X, Y, Z) (((X) & (Y)) | ((~(X)) & (Z)))
#define MAJ(X, Y, Z) (((X) & (Y)) | ((X) & (Z)) | ((Y) & (Z)))
#define XOR(X, Y, Z) ((X) ^ (Y) ^ (Z))

// main function for generating digest
static uint32_t *MD4Digest(uint32_t *w, int len);
static void setMD4Registers(uint32_t A_, uint32_t B_, uint32_t C_, uint32_t D_);
static uint32_t changeEndianness(uint32_t x);
static void resetMD4Registers(void);
static string stringCat(string first, string second);
static string uint32ToString(uint32_t l);
static uint32_t stringToUint32(string s);

static const char *BASE16 = "0123456789abcdef=";

#define LEFTROTATE(A, N) ((A) << (N)) | ((A) >> (32 - (N)))
// Round1Macro
#define ROUND1(a, b, c, d, x, s) \
    a += F(b, c, d) + x;         \
    a = LEFTROTATE(a, s);
#define ROUND2(a, b, c, d, x, s)                  \
    a += MAJ(b, c, d) + x + (uint32_t)0x5A827999; \
    a = LEFTROTATE(a, s);
#define ROUND3(a, b, c, d, x, s)                  \
    a += XOR(b, c, d) + x + (uint32_t)0x6ED9EBA1; \
    a = LEFTROTATE(a, s);

#define BUF_SIZE 1024 // maximum size of string for

string newString(char *c, int t)
{
    string r;
    int i;
    if (c != NULL)
    {
        r.len = (t <= 0) ? strlen(c) : t;
        r.c = (char *)malloc(sizeof(char) * (r.len + 1));
        for (i = 0; i < r.len; i++)
            r.c[i] = c[i];
        r.c[r.len] = '\0';
        return r;
    }
    r.len = t;
    r.c = (char *)malloc(sizeof(char) * (r.len + 1));
    memset(r.c, (char)0, sizeof(char) * (t + 1));
    r.sign = 1;
    return r;
}

string stringCat(string first, string second)
{
    string str = newString(NULL, first.len + second.len);
    int i;

    for (i = 0; i < first.len; i++)
    {
        str.c[i] = first.c[i];
    }
    for (i = first.len; i < str.len; i++)
    {
        str.c[i] = second.c[i - first.len];
    }
    return str;
}

string base16Encode(string in)
{
    string out = newString(NULL, in.len * 2);
    int i, j;

    j = 0;
    for (i = 0; i < in.len; i++)
    {
        out.c[j++] = BASE16[((in.c[i] & 0xF0) >> 4)];
        out.c[j++] = BASE16[(in.c[i] & 0x0F)];
    }
    out.c[j] = '\0';
    return out;
}

string uint32ToString(uint32_t l)
{
    string s = newString(NULL, 4);
    int i;
    for (i = 0; i < 4; i++)
    {
        s.c[i] = (l >> (8 * (3 - i))) & 0xFF;
    }
    return s;
}

uint32_t stringToUint32(string s)
{
    uint32_t l;
    int i;
    l = 0;
    for (i = 0; i < 4; i++)
    {
        l = l | (((uint32_t)((unsigned char)s.c[i])) << (8 * (3 - i)));
    }
    return l;
}

char *MD4(char *str, int len)
{
    string m = newString(str, len);
    string digest;
    uint32_t *w;
    uint32_t *hash;
    uint64_t mlen = m.len;
    unsigned char oneBit = 0x80; //10000000 in bin
    int i, wlen;

    m = stringCat(m, newString((char *)&oneBit, 1));

    i = ((56 - m.len) % 64);
    if (i < 0)
        i += 64;
    m = stringCat(m, newString(NULL, i));

    w = malloc(sizeof(uint32_t) * (m.len / 4 + 2));

    //append length, in bits (hence <<3), least significant word first
    for (i = 0; i < m.len / 4; i++)
    {
        w[i] = stringToUint32(newString(&(m.c[4 * i]), 4));
    }
    // append length in last
    w[i++] = (mlen << 3) & 0xFFFFFFFF;
    w[i++] = (mlen >> 29) & 0xFFFFFFFF;

    wlen = i;

    //change endianness. little to big. leave the length
    for (i = 0; i < wlen - 2; i++)
    {
        w[i] = changeEndianness(w[i]);
    }

    hash = MD4Digest(w, wlen);

    digest = newString(NULL, 0);
    for (i = 0; i < 4; i++)
    {
        hash[i] = changeEndianness(hash[i]);
        digest = stringCat(digest, uint32ToString(hash[i]));
    }

    return base16Encode(digest).c;
}

uint32_t *MD4Digest(uint32_t *w, int len)
{
    //assumes message.len is a multiple of 64 bytes.
    int i, j;
    uint32_t X[16];
    uint32_t *digest = malloc(sizeof(uint32_t) * 4);
    uint32_t A_, B_, C_, D_;
    // Loop for all blocks
    for (i = 0; i < len / 16; i++)
    {
        // Creating 16 words
        for (j = 0; j < 16; j++)
        {
            X[j] = w[i * 16 + j];
        }

        A_ = A;
        B_ = B;
        C_ = C;
        D_ = D;

        // Round 1 begin.
        ROUND1(A, B, C, D, X[0], 3);
        ROUND1(D, A, B, C, X[1], 7);
        ROUND1(C, D, A, B, X[2], 11);
        ROUND1(B, C, D, A, X[3], 19);
        ROUND1(A, B, C, D, X[4], 3);
        ROUND1(D, A, B, C, X[5], 7);
        ROUND1(C, D, A, B, X[6], 11);
        ROUND1(B, C, D, A, X[7], 19);
        ROUND1(A, B, C, D, X[8], 3);
        ROUND1(D, A, B, C, X[9], 7);
        ROUND1(C, D, A, B, X[10], 11);
        ROUND1(B, C, D, A, X[11], 19);
        ROUND1(A, B, C, D, X[12], 3);
        ROUND1(D, A, B, C, X[13], 7);
        ROUND1(C, D, A, B, X[14], 11);
        ROUND1(B, C, D, A, X[15], 19);
        // Round 2 begin
        ROUND2(A, B, C, D, X[0], 3);
        ROUND2(D, A, B, C, X[4], 5);
        ROUND2(C, D, A, B, X[8], 9);
        ROUND2(B, C, D, A, X[12], 13);
        ROUND2(A, B, C, D, X[1], 3);
        ROUND2(D, A, B, C, X[5], 5);
        ROUND2(C, D, A, B, X[9], 9);
        ROUND2(B, C, D, A, X[13], 13);
        ROUND2(A, B, C, D, X[2], 3);
        ROUND2(D, A, B, C, X[6], 5);
        ROUND2(C, D, A, B, X[10], 9);
        ROUND2(B, C, D, A, X[14], 13);
        ROUND2(A, B, C, D, X[3], 3);
        ROUND2(D, A, B, C, X[7], 5);
        ROUND2(C, D, A, B, X[11], 9);
        ROUND2(B, C, D, A, X[15], 13);
        // round 3 begin
        ROUND3(A, B, C, D, X[0], 3);
        ROUND3(D, A, B, C, X[8], 9);
        ROUND3(C, D, A, B, X[4], 11);
        ROUND3(B, C, D, A, X[12], 15);
        ROUND3(A, B, C, D, X[2], 3);
        ROUND3(D, A, B, C, X[10], 9);
        ROUND3(C, D, A, B, X[6], 11);
        ROUND3(B, C, D, A, X[14], 15);
        ROUND3(A, B, C, D, X[1], 3);
        ROUND3(D, A, B, C, X[9], 9);
        ROUND3(C, D, A, B, X[5], 11);
        ROUND3(B, C, D, A, X[13], 15);
        ROUND3(A, B, C, D, X[3], 3);
        ROUND3(D, A, B, C, X[11], 9);
        ROUND3(C, D, A, B, X[7], 11);
        ROUND3(B, C, D, A, X[15], 15);

        A += A_;
        B += B_;
        C += C_;
        D += D_;
    }

    digest[0] = A;
    digest[1] = B;
    digest[2] = C;
    digest[3] = D;
    // After completion of one block reset the registers.
    resetMD4Registers();
    return digest;
}

uint32_t changeEndianness(uint32_t x)
{
    return ((x & 0xFF) << 24) | ((x & 0xFF00) << 8) | ((x & 0xFF0000) >> 8) | ((x & 0xFF000000) >> 24);
}

void setMD4Registers(uint32_t A_, uint32_t B_, uint32_t C_, uint32_t D_)
{
    A = A_;
    B = B_;
    C = C_;
    D = D_;
}

void resetMD4Registers(void)
{
    setMD4Registers(0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476);
}

// Reading file for input
char *readFileMD4Input(FILE *fd, int *len)
{
    char *content = (char *)malloc(BUF_SIZE * sizeof(char));
    int i = 0;
    do
    {
        char c = fgetc(fd);
        if (feof(fd))
            break;

        // printf("%c", c);
        content[i++] = c;
    } while (1);

    content[i] = '\0';
    *len = i;
    return content;
}

