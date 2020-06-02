/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RSAsignature.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Vishnu <vishnu44d@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/02 12:55:40 by Vishnu            #+#    #+#             */
/*   Updated: 2020/06/02 19:01:35 by Vishnu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

#define ACCURACY 20

#define FACTOR_DIGITS 100
#define EXPONENT_MAX RAND_MAX
#define BUF_SIZE 1024

#define i128_CAPACITY 20

#define RADIX 4294967296UL
#define HALFRADIX 2147483648UL

#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct i128_
{
    int length;
    int capacity;
    unsigned int *data;
} i128;

// function to add in place
// i.e. source += add
void inplace_addition(i128 *source, i128 *add);
// function to add
// i.e. result = b1 + b2
void addition(i128 *result, i128 *b1, i128 *b2);
// function to subtract in place
// i.e. source -= add
void inplace_subtraction(i128 *source, i128 *add);
// function to subtract
// i.e. result = b1 - b2
void subtraction(i128 *result, i128 *b1, i128 *b2);
// function to multiply in place
// i.e. source *= add
void inplace_multiplication(i128 *source, i128 *add);
// function to multiply
// i.e. result = b1 * b2
void multiplication(i128 *result, i128 *b1, i128 *b2);
// inplace divide
// i.e. source /= div
void inplave_divide(i128 *source, i128 *div);
// Perform an in place divide of source and also producing a remainder.
// source = source/div and remainder = source - source/div.
void inplace_divider(i128 *source, i128 *div, i128 *remainder);
// Calculate the remainder when source is divided by div.
void remainder_op(i128 *source, i128 *div, i128 *remainder);
// Modulate the source by the modulus. source = source % modulus
void modular_op(i128 *source, i128 *modulus);
// Divide two numbers and produc both a quotient and remainder.
// quotient = floor(b1/b2), remainder = b1 - quotient * b2. If b1 < b2 the quotient is
// trivially 0 and remainder is b2.
void division(i128 *quotient, i128 *remainder, i128 *b1, i128 *b2);

unsigned int DATA0[1] = {0};
unsigned int DATA1[1] = {1};
unsigned int DATA2[1] = {2};
unsigned int DATA3[1] = {3};
unsigned int DATA4[1] = {4};
unsigned int DATA5[1] = {5};
unsigned int DATA6[1] = {6};
unsigned int DATA7[1] = {7};
unsigned int DATA8[1] = {8};
unsigned int DATA9[1] = {9};
unsigned int DATA10[1] = {10};
unsigned int DATA[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
i128 NUMS[11] = {
    {1, 1, DATA0},
    {1, 1, DATA1},
    {1, 1, DATA2},
    {1, 1, DATA3},
    {1, 1, DATA4},
    {1, 1, DATA5},
    {1, 1, DATA6},
    {1, 1, DATA7},
    {1, 1, DATA8},
    {1, 1, DATA9},
    {1, 1, DATA10}};

// initialize i128
i128 *i128_init()
{
    i128 *b = malloc(sizeof(i128));
    b->length = 0;
    b->capacity = i128_CAPACITY;
    b->data = calloc(i128_CAPACITY, sizeof(unsigned int));
    return b;
}
// free memory. Very crucial
void i128_free(i128 *b)
{
    free(b->data);
    free(b);
}
// check if b is zero
int isZero(i128 *b)
{
    return b->length == 0 || (b->length == 1 && b->data[0] == 0);
}
// check if not zero
int isNonZero(i128 *b)
{
    return !isZero(b);
}
// copy i128 variavle
// i.e. dest=source
void cpy(i128 *source, i128 *dest)
{
    dest->length = source->length;
    // if dest capacity is low realloc the dest data
    if (source->capacity > dest->capacity)
    {
        dest->capacity = source->capacity;
        dest->data = realloc(dest->data, dest->capacity * sizeof(unsigned int));
    }
    memcpy(dest->data, source->data, dest->length * sizeof(unsigned int));
}
// make i128 number from a string.
// only take string with number in it. Err is not hanlded here for special char in string
void from_str_to_i128(i128 *b, char *string)
{
    int i, len = 0;
    // find string length
    while (string[len] != '\0')
        len++;
    for (i = 0; i < len; i++)
    {
        if (i != 0)
            inplace_multiplication(b, &NUMS[10]);    // b *= 10;
        inplace_addition(b, &NUMS[string[i] - '0']); // b += NUMS[string[i]-'0'];
    }
}

void from_int_to_i128(i128 *b, unsigned int num)
{
    b->length = 1;
    if (b->capacity < b->length)
    {
        b->capacity = b->length;
        b->data = realloc(b->data, b->capacity * sizeof(unsigned int));
    }
    b->data[0] = num;
}

// to print i128 just by repetative division by 10
void print_i128(i128 *b)
{
    int cap = 100, len = 0, i;
    char *buffer = malloc(cap * sizeof(char));
    i128 *copy = i128_init(), *remainder = i128_init();
    if (b->length == 0 || isZero(b))
        printf("0");
    else
    {
        cpy(b, copy);
        while (isNonZero(copy))
        {
            inplace_divider(copy, &NUMS[10], remainder);
            buffer[len++] = remainder->data[0];
            if (len >= cap)
            {
                cap *= 2;
                buffer = realloc(buffer, cap * sizeof(char));
            }
        }
        for (i = len - 1; i >= 0; i--)
            printf("%d", buffer[i]);
    }
    i128_free(copy);
    i128_free(remainder);
    free(buffer);
}

// to print i128 just by repetative division by 10
void fprint_i128(FILE *f, i128 *b)
{
    int cap = 100, len = 0, i;
    char *buffer = malloc(cap * sizeof(char));
    i128 *copy = i128_init(), *remainder = i128_init();
    // FILE *f;
    // f = fopen("test.txt", "a+");
    if (b->length == 0 || isZero(b))
        fputc('0', f);
    // printf("0");
    else
    {
        cpy(b, copy);
        while (isNonZero(copy))
        {
            inplace_divider(copy, &NUMS[10], remainder);
            buffer[len++] = remainder->data[0];
            if (len >= cap)
            {
                cap *= 2;
                buffer = realloc(buffer, cap * sizeof(char));
            }
        }
        for (i = len - 1; i >= 0; i--)
            fprintf(f, "%d", buffer[i]);
        // printf("%d", buffer[i]);
    }
    i128_free(copy);
    i128_free(remainder);
    free(buffer);
    // fclose(f);
}

// check if b1==b2
int isEqual(i128 *b1, i128 *b2)
{
    int i;
    if (isZero(b1) && isZero(b2))
        return 1;
    else if (isZero(b1))
        return 0;
    else if (isZero(b2))
        return 0;
    else if (b1->length != b2->length)
        return 0;
    for (i = b1->length - 1; i >= 0; i--)
    {
        if (b1->data[i] != b2->data[i])
            return 0;
    }
    return 1;
}

// check if b1>b2
int isGreater(i128 *b1, i128 *b2)
{
    int i;
    if (isZero(b1) && isZero(b2))
        return 0;
    else if (isZero(b1))
        return 0;
    else if (isZero(b2))
        return 1;
    else if (b1->length != b2->length)
        return b1->length > b2->length;
    for (i = b1->length - 1; i >= 0; i--)
    {
        if (b1->data[i] != b2->data[i])
            return b1->data[i] > b2->data[i];
    }
    return 0;
}
// check if b1<b2
int isLess(i128 *b1, i128 *b2)
{
    int i;
    if (isZero(b1) && isZero(b2))
        return 0;
    else if (isZero(b1))
        return 1;
    else if (isZero(b2))
        return 0;
    else if (b1->length != b2->length)
        return b1->length < b2->length;
    for (i = b1->length - 1; i >= 0; i--)
    {
        if (b1->data[i] != b2->data[i])
            return b1->data[i] < b2->data[i];
    }
    return 0;
}
// check if b1>=b2
int isGEq(i128 *b1, i128 *b2)
{
    return !isLess(b1, b2);
}
// check if b1<=b2
int isLEq(i128 *b1, i128 *b2)
{
    return !isGreater(b1, b2);
}

void inplace_addition(i128 *source, i128 *add)
{
    i128 *temp = i128_init();
    addition(temp, source, add);
    cpy(temp, source);
    i128_free(temp);
}

void addition(i128 *result, i128 *b1, i128 *b2)
{
    unsigned int sum, carry = 0;
    int i, n = MAX(b1->length, b2->length);
    if (n + 1 > result->capacity)
    {
        result->capacity = n + 1;
        result->data = realloc(result->data, result->capacity * sizeof(unsigned int));
    }
    for (i = 0; i < n; i++)
    {
        sum = carry;
        if (i < b1->length)
            sum += b1->data[i];
        if (i < b2->length)
            sum += b2->data[i];
        result->data[i] = sum; /* Already taken mod 2^32 by unsigned wrap around */

        if (i < b1->length)
        {
            if (sum < b1->data[i])
                carry = 1; /* Result must have wrapped 2^32 so carry bit is 1 */
            else
                carry = 0;
        }
        else
        {
            if (sum < b2->data[i])
                carry = 1; /* Result must have wrapped 2^32 so carry bit is 1 */
            else
                carry = 0;
        }
    }
    if (carry == 1)
    {
        result->length = n + 1;
        result->data[n] = 1;
    }
    else
    {
        result->length = n;
    }
}

void inplace_subtraction(i128 *source, i128 *sub)
{
    i128 *temp = i128_init();
    subtraction(temp, source, sub);
    cpy(temp, source);
    i128_free(temp);
}

void subtraction(i128 *result, i128 *b1, i128 *b2)
{
    int length = 0, i;
    unsigned int carry = 0, diff, temp;
    if (b1->length > result->capacity)
    {
        result->capacity = b1->length;
        result->data = realloc(result->data, result->capacity * sizeof(unsigned int));
    }
    for (i = 0; i < b1->length; i++)
    {
        temp = carry;
        if (i < b2->length)
            temp = temp + b2->data[i]; /* Auto wrapped mod RADIX */
        diff = b1->data[i] - temp;
        if (temp > b1->data[i])
            carry = 1;
        else
        {
            if (temp == 0 && b2->data[i] == 0xffffffff)
            {
                carry = 1;
            }
            else
            {
                carry = 0;
            }
        }
        result->data[i] = diff;
        if (result->data[i] != 0)
            length = i + 1;
    }
    result->length = length;
}

void inplace_multiplication(i128 *source, i128 *mult)
{
    i128 *temp = i128_init();
    multiplication(temp, source, mult);
    cpy(temp, source);
    i128_free(temp);
}

void multiplication(i128 *result, i128 *b1, i128 *b2)
{
    int i, j, k;
    unsigned int carry, temp;
    unsigned long long int prod; /* Long for intermediate product... this is not portable and should probably be changed */
    if (b1->length + b2->length > result->capacity)
    {
        result->capacity = b1->length + b2->length;
        result->data = realloc(result->data, result->capacity * sizeof(unsigned int));
    }
    for (i = 0; i < b1->length + b2->length; i++)
        result->data[i] = 0;

    for (i = 0; i < b1->length; i++)
    {
        for (j = 0; j < b2->length; j++)
        {
            prod = (b1->data[i] * (unsigned long long int)b2->data[j]) + (unsigned long long int)(result->data[i + j]); /* This should not overflow... */
            carry = (unsigned int)(prod / RADIX);

            /* Add carry to the next unsigned int over, but this may cause further overflow.. propogate */
            k = 1;
            while (carry > 0)
            {
                temp = result->data[i + j + k] + carry;
                if (temp < result->data[i + j + k])
                    carry = 1;
                else
                    carry = 0;
                result->data[i + j + k] = temp; /* Already wrapped in unsigned arithmetic */
                k++;
            }

            prod = (result->data[i + j] + b1->data[i] * (unsigned long long int)b2->data[j]) % RADIX; /* Again, should not overflow... */
            result->data[i + j] = prod;                                                               /* Add */
        }
    }
    if (b1->length + b2->length > 0 && result->data[b1->length + b2->length - 1] == 0)
        result->length = b1->length + b2->length - 1;
    else
        result->length = b1->length + b2->length;
}

void inplave_divide(i128 *source, i128 *div)
{
    i128 *q = i128_init(), *r = i128_init();
    division(q, r, source, div);
    cpy(q, source);
    i128_free(q);
    i128_free(r);
}

void inplace_divider(i128 *source, i128 *div, i128 *remainder)
{
    i128 *q = i128_init(), *r = i128_init();
    division(q, r, source, div);
    cpy(q, source);
    cpy(r, remainder);
    i128_free(q);
    i128_free(r);
}

void remainder_op(i128 *source, i128 *div, i128 *remainder)
{
    i128 *q = i128_init();
    division(q, remainder, source, div);
    i128_free(q);
}

void modular_op(i128 *source, i128 *modulus)
{
    i128 *q = i128_init(), *r = i128_init();
    division(q, r, source, modulus);
    cpy(r, source);
    i128_free(q);
    i128_free(r);
}

void division(i128 *quotient, i128 *remainder, i128 *b1, i128 *b2)
{
    i128 *b2copy = i128_init(), *b1copy = i128_init();
    i128 *temp = i128_init(), *temp2 = i128_init(), *temp3 = i128_init();
    i128 *quottemp = i128_init();
    unsigned int carry = 0;
    int n, m, i, j, length = 0;
    unsigned long long factor = 1;
    unsigned long long gquot, gtemp, grem;
    if (isLess(b1, b2))
    { /* Trivial case, b1/b2 = 0 iff b1 < b2. */
        quotient->length = 0;
        cpy(b1, remainder);
    }
    else if (isZero(b1))
    { /* 0/x = 0.. assuming b2 is nonzero */
        quotient->length = 0;
        from_int_to_i128(remainder, 0);
    }
    else if (b2->length == 1)
    { /* Division by a single limb means we can do simple division */
        if (quotient->capacity < b1->length)
        {
            quotient->capacity = b1->length;
            quotient->data = realloc(quotient->data, quotient->capacity * sizeof(unsigned int));
        }
        for (i = b1->length - 1; i >= 0; i--)
        {
            gtemp = carry * RADIX + b1->data[i];
            gquot = gtemp / b2->data[0];
            quotient->data[i] = gquot;
            if (quotient->data[i] != 0 && length == 0)
                length = i + 1;
            carry = gtemp % b2->data[0];
        }
        from_int_to_i128(remainder, carry);
        quotient->length = length;
    }
    else
    {
        n = b1->length + 1;
        m = b2->length;
        if (quotient->capacity < n - m)
        {
            quotient->capacity = n - m;
            quotient->data = realloc(quotient->data, (n - m) * sizeof(unsigned int));
        }
        cpy(b1, b1copy);
        cpy(b2, b2copy);
        while (b2copy->data[b2copy->length - 1] < HALFRADIX)
        {
            factor *= 2;
            inplace_multiplication(b2copy, &NUMS[2]);
        }
        if (factor > 1)
        {
            from_int_to_i128(temp, factor);
            inplace_multiplication(b1copy, temp);
        }
        /* Ensure the dividend is longer than the original (pre-normalized) divisor. If it is not
		 * we introduce a dummy zero unsigned int to artificially inflate it. */
        if (b1copy->length != n)
        {
            b1copy->length++;
            if (b1copy->length > b1copy->capacity)
            {
                b1copy->capacity = b1copy->length;
                b1copy->data = realloc(b1copy->data, b1copy->capacity * sizeof(unsigned int));
            }
            b1copy->data[n - 1] = 0;
        }

        for (i = n - m - 1; i >= 0; i--)
        {
            gtemp = RADIX * b1copy->data[i + m] + b1copy->data[i + m - 1];
            gquot = gtemp / b2copy->data[m - 1];
            if (gquot >= RADIX)
                gquot = UINT_MAX;
            grem = gtemp % b2copy->data[m - 1];
            while (grem < RADIX && gquot * b2copy->data[m - 2] > RADIX * grem + b1copy->data[i + m - 2])
            { /* Should not overflow... ? */
                gquot--;
                grem += b2copy->data[m - 1];
            }
            quottemp->data[0] = gquot % RADIX;
            quottemp->data[1] = (gquot / RADIX);
            if (quottemp->data[1] != 0)
                quottemp->length = 2;
            else
                quottemp->length = 1;
            multiplication(temp2, b2copy, quottemp);
            if (m + 1 > temp3->capacity)
            {
                temp3->capacity = m + 1;
                temp3->data = realloc(temp3->data, temp3->capacity * sizeof(unsigned int));
            }
            temp3->length = 0;
            for (j = 0; j <= m; j++)
            {
                temp3->data[j] = b1copy->data[i + j];
                if (temp3->data[j] != 0)
                    temp3->length = j + 1;
            }
            if (isLess(temp3, temp2))
            {
                inplace_addition(temp3, b2copy);
                gquot--;
            }
            inplace_subtraction(temp3, temp2);
            for (j = 0; j < temp3->length; j++)
                b1copy->data[i + j] = temp3->data[j];
            for (j = temp3->length; j <= m; j++)
                b1copy->data[i + j] = 0;
            quotient->data[i] = gquot;
            if (quotient->data[i] != 0)
                quotient->length = i;
        }

        if (quotient->data[b1->length - b2->length] == 0)
            quotient->length = b1->length - b2->length;
        else
            quotient->length = b1->length - b2->length + 1;

        carry = 0;
        for (i = b1copy->length - 1; i >= 0; i--)
        {
            gtemp = carry * RADIX + b1copy->data[i];
            b1copy->data[i] = gtemp / factor;
            if (b1copy->data[i] != 0 && length == 0)
                length = i + 1;
            carry = gtemp % factor;
        }
        b1copy->length = length;
        cpy(b1copy, remainder);
    }
    i128_free(temp);
    i128_free(temp2);
    i128_free(temp3);
    i128_free(b1copy);
    i128_free(b2copy);
    i128_free(quottemp);
}
// modulus exponent operartion
// i.e. result = base^exponent mod modulus
void mod_pow(i128 *base, i128 *exponent, i128 *modulus, i128 *result)
{
    i128 *a = i128_init(), *b = i128_init(), *c = i128_init();
    i128 *discard = i128_init(), *remainder = i128_init();
    cpy(base, a);
    cpy(exponent, b);
    cpy(modulus, c);
    from_int_to_i128(result, 1);
    while (isGreater(b, &NUMS[0]))
    {
        if (b->data[0] & 1)
        {
            inplace_multiplication(result, a);
            modular_op(result, c);
        }
        inplave_divide(b, &NUMS[2]);
        cpy(a, discard);
        inplace_multiplication(a, discard);
        modular_op(a, c);
    }
    i128_free(a);
    i128_free(b);
    i128_free(c);
    i128_free(discard);
    i128_free(remainder);
}
// compute gcd.
// i.e. result = gcd(b1, b2);
void compute_gcd(i128 *b1, i128 *b2, i128 *result)
{
    i128 *a = i128_init(), *b = i128_init(), *remainder = i128_init();
    i128 *temp = i128_init(), *discard = i128_init();
    cpy(b1, a);
    cpy(b2, b);
    while (!isEqual(b, &NUMS[0]))
    {
        cpy(b, temp);
        modular_op(a, b);
        cpy(a, b);
        cpy(temp, a);
    }
    cpy(a, result);
    i128_free(a);
    i128_free(b);
    i128_free(remainder);
    i128_free(temp);
    i128_free(discard);
}
// compute inverse a mod(m)
// i.e. result = a^-1 mod m.
void compute_inverse(i128 *a, i128 *m, i128 *result)
{
    i128 *remprev = i128_init(), *rem = i128_init();
    i128 *auxprev = i128_init(), *aux = i128_init();
    i128 *rcur = i128_init(), *qcur = i128_init(), *acur = i128_init();

    cpy(m, remprev);
    cpy(a, rem);
    from_int_to_i128(auxprev, 0);
    from_int_to_i128(aux, 1);
    while (isGreater(rem, &NUMS[1]))
    {
        division(qcur, rcur, remprev, rem);
        subtraction(acur, m, qcur);
        inplace_multiplication(acur, aux);
        inplace_addition(acur, auxprev);
        modular_op(acur, m);

        cpy(rem, remprev);
        cpy(aux, auxprev);
        cpy(rcur, rem);
        cpy(acur, aux);
    }

    cpy(acur, result);

    i128_free(remprev);
    i128_free(rem);
    i128_free(auxprev);
    i128_free(aux);
    i128_free(rcur);
    i128_free(qcur);
    i128_free(acur);
}

// Compute the jacobi symbol, J(ac, nc)
// https://en.wikipedia.org/wiki/Solovay%E2%80%93Strassen_primality_test
int jacobi_c(i128 *ac, i128 *nc)
{
    i128 *remainder = i128_init(), *twos = i128_init();
    i128 *temp = i128_init(), *a = i128_init(), *n = i128_init();
    int mult = 1, result = 0;
    cpy(ac, a);
    cpy(nc, n);
    while (isGreater(a, &NUMS[1]) && !isEqual(a, n))
    {
        modular_op(a, n);
        if (isLEq(a, &NUMS[1]) || isEqual(a, n))
            break;
        from_int_to_i128(twos, 0);
        while (a->data[0] % 2 == 0)
        {
            inplace_addition(twos, &NUMS[1]);
            inplave_divide(a, &NUMS[2]);
        }
        if (isGreater(twos, &NUMS[0]) && twos->data[0] % 2 == 1)
        {
            remainder_op(n, &NUMS[8], remainder);
            if (!isEqual(remainder, &NUMS[1]) && !isEqual(remainder, &NUMS[7]))
            {
                mult *= -1;
            }
        }
        if (isLEq(a, &NUMS[1]) || isEqual(a, n))
            break;
        remainder_op(n, &NUMS[4], remainder);
        remainder_op(a, &NUMS[4], temp);
        if (!isEqual(remainder, &NUMS[1]) && !isEqual(temp, &NUMS[1]))
            mult *= -1;
        cpy(a, temp);
        cpy(n, a);
        cpy(temp, n);
    }
    if (isEqual(a, &NUMS[1]))
        result = mult;
    else
        result = 0;
    i128_free(remainder);
    i128_free(twos);
    i128_free(temp);
    i128_free(a);
    i128_free(n);
    return result;
}
// Check whether a is a Euler witness for n. That is, if a^(n - 1)/2 != Ja(a, n) mod n
// https://en.wikipedia.org/wiki/Solovay%E2%80%93Strassen_primality_test
int solovayPrime(int a, i128 *n)
{
    i128 *ab = i128_init(), *res = i128_init(), *pow = i128_init();
    i128 *modpow = i128_init();
    int x, result;

    from_int_to_i128(ab, a);
    x = jacobi_c(ab, n);
    if (x == -1)
        subtraction(res, n, &NUMS[1]);
    else
        from_int_to_i128(res, x);
    cpy(n, pow);
    inplace_subtraction(pow, &NUMS[1]);
    inplave_divide(pow, &NUMS[2]);
    mod_pow(ab, pow, n, modpow);

    result = !isEqual(res, &NUMS[0]) && isEqual(modpow, res);
    i128_free(ab);
    i128_free(res);
    i128_free(pow);
    i128_free(modpow);
    return result;
}
// Test if n is probably prime, by repeatedly using the Solovay-Strassen primality test.
// https://en.wikipedia.org/wiki/Solovay%E2%80%93Strassen_primality_test
int probablePrime(i128 *n, int k)
{
    if (isEqual(n, &NUMS[2]))
        return 1;
    else if (n->data[0] % 2 == 0 || isEqual(n, &NUMS[1]))
        return 0;
    while (k-- > 0)
    {
        if (n->length <= 1)
        { /* Prevent a > n */
            if (!solovayPrime(rand() % (n->data[0] - 2) + 2, n))
                return 0;
        }
        else
        {
            int wit = rand() % (RAND_MAX - 2) + 2;
            if (!solovayPrime(wit, n))
                return 0;
        }
    }
    return 1;
}
// Generate a random prime number, with a specified number of digits.
void randPrime(int numDigits, i128 *result)
{
    char *string = malloc((numDigits + 1) * sizeof(char));
    int i;
    string[0] = (rand() % 9) + '1';
    string[numDigits - 1] = (rand() % 5) * 2 + '1';
    for (i = 1; i < numDigits - 1; i++)
        string[i] = (rand() % 10) + '0';
    string[numDigits] = '\0';
    from_str_to_i128(result, string);
    while (1)
    {
        if (probablePrime(result, ACCURACY))
        {
            free(string);
            return;
        }
        inplace_addition(result, &NUMS[2]); /* result += 2 */
    }
}

void randExponent(i128 *phi, int n, i128 *result)
{
    i128 *gcd = i128_init();
    int e = rand() % n;
    while (1)
    {
        from_int_to_i128(result, e);
        compute_gcd(result, phi, gcd);
        if (isEqual(gcd, &NUMS[1]))
        {
            i128_free(gcd);
            return;
        }
        e = (e + 1) % n;
        if (e <= 2)
            e = 3;
    }
}
// Reading file for input
int readFile(FILE *fd, char **buffer, int bytes)
{
    int len = 0, cap = BUF_SIZE, r;
    char buf[BUF_SIZE];
    *buffer = malloc(BUF_SIZE * sizeof(char));
    while ((r = fread(buf, sizeof(char), BUF_SIZE, fd)) > 0)
    {
        if (len + r >= cap)
        {
            cap *= 2;
            *buffer = realloc(*buffer, cap);
        }
        memcpy(&(*buffer)[len], buf, r);
        len += r;
    }
    /* Pad the last block with zeros to signal end of cryptogram. An additional block is added if there is no room */
    if (len + bytes - len % bytes > cap)
        *buffer = realloc(*buffer, len + bytes - len % bytes);
    do
    {
        (*buffer)[len] = '\0';
        len++;
    } while (len % bytes != 0);
    return len;
}

// encode a message m
// i.e. result = m^e mod n
void encode(i128 *m, i128 *e, i128 *n, i128 *result)
{
    mod_pow(m, e, n, result);
}
// decode a cipher
// i.e. result = c^d mod n
void decode(i128 *c, i128 *d, i128 *n, i128 *result)
{
    mod_pow(c, d, n, result);
}

// encode the entire message
i128 *encodeMessage(int len, int bytes, char *message, i128 *exponent, i128 *modulus)
{

    int i, j;
    i128 *encoded = calloc(len / bytes, sizeof(i128));
    i128 *num128 = i128_init(), *num128pow = i128_init();
    i128 *x = i128_init(), *current = i128_init();
    from_int_to_i128(num128, 128);
    from_int_to_i128(num128pow, 1);
    // printf("Cipher text is: ");
    for (i = 0; i < len; i += bytes)
    {
        from_int_to_i128(x, 0);
        from_int_to_i128(num128pow, 1);
        // Compute buffer[0] + buffer[1]*128 + buffer[2]*128^2 etc (base 128 representation for characters->int encoding)
        for (j = 0; j < bytes; j++)
        {
            from_int_to_i128(current, message[i + j]);
            inplace_multiplication(current, num128pow);
            inplace_addition(x, current); //x += buffer[i + j] * (1 << (7 * j))
            inplace_multiplication(num128pow, num128);
        }
        encode(x, exponent, modulus, &encoded[i / bytes]);
        // print_i128(&encoded[i / bytes]);
        // printf(" ");
    }
    return encoded;
}

// decode the entire message
char *decodeMessage(int len, int bytes, i128 *cryptogram, i128 *exponent, i128 *modulus)
{
    int *decoded = malloc(len * bytes * sizeof(int));
    char *msg = (char *)malloc(len * bytes * sizeof(char));
    int i, j, k;
    i128 *x = i128_init(), *remainder = i128_init();
    i128 *num128 = i128_init();
    // printf("Decoded message is: ");
    from_int_to_i128(num128, 128);
    k = 0;
    for (i = 0; i < len; i++)
    {
        decode(&cryptogram[i], exponent, modulus, x);
        for (j = 0; j < bytes; j++)
        {
            inplace_divider(x, num128, remainder);
            if (remainder->length == 0)
                decoded[i * bytes + j] = (char)0;
            else
                decoded[i * bytes + j] = (char)(remainder->data[0]);
            // printf("%c", (char)(decoded[i * bytes + j]));
            msg[k++] = (char)(decoded[i * bytes + j]);
        }
    }
    // printf("\n");
    msg[k] = '\0';
    // printf("\n\nmsg is: %s", msg);
    return msg;
}

void read_key_file(FILE *f, char **e_or_d, char **n)
{
    char *line = NULL;
    char *str_e_or_d = (char *)malloc(400 * sizeof(char));
    char *str_n = (char *)malloc(400 * sizeof(char));
    size_t len = 0;
    int i = 0;
    while ((getline(&line, &len, f) != -1))
    {
        if (i == 1)
        {
            int k = 2;
            while (line[k] != ' ')
            {
                str_e_or_d[k - 2] = line[k];
                k++;
            }
            str_e_or_d[k - 2] = '\0';
            strcpy(*e_or_d, str_e_or_d);
            free(str_e_or_d);
            int j = k + 3;
            k = 0;
            while (line[j] != ' ')
            {
                str_n[k] = line[j];
                j++;
                k++;
            }
            str_n[k] = '\0';
            // printf("\n%s", str_n);
            strcpy(*n, str_n);
            free(str_n);
        }
        i++;
    }
}

int main(int argc, char const *argv[])
{
    FILE *fp;
    // char *str_d = "", *str_n = "";
    char *str_d = (char *)malloc(200 * sizeof(char));
    char *str_e = (char *)malloc(200 * sizeof(char));
    char *str_n = (char *)malloc(200 * sizeof(char));
    size_t len = 0;
    int byte = 94;

    fp = fopen("privatekey.txt", "r");
    read_key_file(fp, &str_d, &str_n);
    fclose(fp);

    fp = fopen("publickey.txt", "r");
    read_key_file(fp, &str_e, &str_n);
    fclose(fp);

    i128 *e = i128_init();
    i128 *d = i128_init();
    i128 *n = i128_init();

    from_str_to_i128(e, str_e);
    from_str_to_i128(d, str_d);
    from_str_to_i128(n, str_n);
    printf("e is: ");
    print_i128(e);
    printf("\n");
    printf("d is: ");
    print_i128(d);
    printf("\n");
    printf("n is: ");
    print_i128(n);
    printf("\n");

    int in_len;
    char *buffer;
    int bytes = 94;

    // fp = fopen("cipher.txt", "r");
    // in_len = readFile(fp, &buffer, bytes);
    // fclose(fp);
    // i128 *cipher = i128_init();
    // from_str_to_i128(cipher, buffer);
    // char *decoded;
    // decoded = decodeMessage(in_len / bytes, bytes, cipher, d, n);
    // printf("\n\n%s\n", decoded);

    fp = fopen("cipher.txt", "r");
    in_len = readFile(fp, &buffer, bytes);
    fclose(fp);
    printf("\nMessage is (M): %s\n", buffer);
    // signing the message, that is already encrypted using RSA.
    // with my private key (d, n).
    // I am Alice Here. who wants to send the message along with it's signature.
    i128 *sign = i128_init();
    sign = encodeMessage(in_len, bytes, buffer, d, n);
    printf("Signature is: ");
    print_i128(sign);
    printf(" \n");
    // writing this signature to a signature file.
    fp = fopen("signature.txt", "w+");
    fprint_i128(fp, sign);
    fclose(fp);

    // This signature.txt and cipher.txt is sent to BOB.
    // Here I am bob.
    // I will decrypt the signature using Alice's Public key (e, n)
    // and if this message is equals to the message I will accept otherwise reject.
    char *sign_buffer;
    int s_len;
    fp = fopen("signature.txt", "r");
    s_len = readFile(fp, &sign_buffer, bytes);
    fclose(fp);
    i128 *signature = i128_init();
    from_str_to_i128(signature, sign_buffer);
    printf("File se read signature: ");
    print_i128(signature);
    printf("\n");
    char *m_dash;
    m_dash = decodeMessage(in_len / bytes, bytes, sign, e, n);
    printf("Decrypted Message is (M`): %s\n", m_dash);

    // comparing m` with actual message stored in buffer
    if (!strcmp(buffer, m_dash))
    {
        printf("M and M` is same. Accepting the message\n");
    }
    else
    {
        printf("M and M` is different. Rejecting the message.");
    }

    i128_free(e);
    i128_free(d);
    i128_free(n);
    i128_free(sign);
    i128_free(signature);

    // free(str_n);
    // free(str_d);
    str_e = NULL;

    return 0;
}
