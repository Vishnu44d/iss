#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

#define ACCURACY 20

#define FACTOR_DIGITS 100
#define EXPONENT_MAX RAND_MAX
#define BUF_SIZE 1024

#define BIGNUM_CAPACITY 20

#define RADIX 4294967296UL
#define HALFRADIX 2147483648UL

#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef unsigned int word;

typedef struct i128_
{
    int length;
    int capacity;
    word *data;
} bignum;

// function to add in place
// i.e. source += add
void inplace_addition(bignum *source, bignum *add);
// function to add
// i.e. result = b1 + b2
void addition(bignum *result, bignum *b1, bignum *b2);
// function to subtract in place
// i.e. source -= add
void inplace_subtraction(bignum *source, bignum *add);
// function to subtract
// i.e. result = b1 - b2
void subtraction(bignum *result, bignum *b1, bignum *b2);
// function to multiply in place
// i.e. source *= add
void inplace_multiplication(bignum *source, bignum *add);
// function to multiply
// i.e. result = b1 * b2
void multiplication(bignum *result, bignum *b1, bignum *b2);
// inplace divide
// i.e. source /= div
void inplave_divide(bignum *source, bignum *div);
// Perform an in place divide of source and also producing a remainder.
// source = source/div and remainder = source - source/div.
void inplace_divider(bignum *source, bignum *div, bignum *remainder);
// Calculate the remainder when source is divided by div.
void remainder_op(bignum *source, bignum *div, bignum *remainder);
// Modulate the source by the modulus. source = source % modulus
void modular_op(bignum *source, bignum *modulus);
// Divide two numbers and produc both a quotient and remainder.
// quotient = floor(b1/b2), remainder = b1 - quotient * b2. If b1 < b2 the quotient is
// trivially 0 and remainder is b2.
void division(bignum *quotient, bignum *remainder, bignum *b1, bignum *b2);

word DATA0[1] = {0};
word DATA1[1] = {1};
word DATA2[1] = {2};
word DATA3[1] = {3};
word DATA4[1] = {4};
word DATA5[1] = {5};
word DATA6[1] = {6};
word DATA7[1] = {7};
word DATA8[1] = {8};
word DATA9[1] = {9};
word DATA10[1] = {10};
word DATA[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
bignum NUMS[11] = {
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

bignum *i128_init()
{
    bignum *b = malloc(sizeof(bignum));
    b->length = 0;
    b->capacity = BIGNUM_CAPACITY;
    b->data = calloc(BIGNUM_CAPACITY, sizeof(word));
    return b;
}

void i128_free(bignum *b)
{
    free(b->data);
    free(b);
}

int isZero(bignum *b)
{
    return b->length == 0 || (b->length == 1 && b->data[0] == 0);
}

int isNonZero(bignum *b)
{
    return !isZero(b);
}

void cpy(bignum *source, bignum *dest)
{
    dest->length = source->length;
    if (source->capacity > dest->capacity)
    {
        dest->capacity = source->capacity;
        dest->data = realloc(dest->data, dest->capacity * sizeof(word));
    }
    memcpy(dest->data, source->data, dest->length * sizeof(word));
}

void from_str_to_i128(bignum *b, char *string)
{
    int i, len = 0;
    while (string[len] != '\0')
        len++; /* Find string length */
    for (i = 0; i < len; i++)
    {
        if (i != 0)
            inplace_multiplication(b, &NUMS[10]);    /* Base 10 multiply */
        inplace_addition(b, &NUMS[string[i] - '0']); /* Add */
    }
}

void from_int_to_i128(bignum *b, unsigned int num)
{
    b->length = 1;
    if (b->capacity < b->length)
    {
        b->capacity = b->length;
        b->data = realloc(b->data, b->capacity * sizeof(word));
    }
    b->data[0] = num;
}

void print_i128(bignum *b)
{
    int cap = 100, len = 0, i;
    char *buffer = malloc(cap * sizeof(char));
    bignum *copy = i128_init(), *remainder = i128_init();
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

int isEqual(bignum *b1, bignum *b2)
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

int isGreater(bignum *b1, bignum *b2)
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

int isLess(bignum *b1, bignum *b2)
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

int isGEq(bignum *b1, bignum *b2)
{
    return !isLess(b1, b2);
}

int isLEq(bignum *b1, bignum *b2)
{
    return !isGreater(b1, b2);
}

void inplace_addition(bignum *source, bignum *add)
{
    bignum *temp = i128_init();
    addition(temp, source, add);
    cpy(temp, source);
    i128_free(temp);
}

void addition(bignum *result, bignum *b1, bignum *b2)
{
    word sum, carry = 0;
    int i, n = MAX(b1->length, b2->length);
    if (n + 1 > result->capacity)
    {
        result->capacity = n + 1;
        result->data = realloc(result->data, result->capacity * sizeof(word));
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

void inplace_subtraction(bignum *source, bignum *sub)
{
    bignum *temp = i128_init();
    subtraction(temp, source, sub);
    cpy(temp, source);
    i128_free(temp);
}

void subtraction(bignum *result, bignum *b1, bignum *b2)
{
    int length = 0, i;
    word carry = 0, diff, temp;
    if (b1->length > result->capacity)
    {
        result->capacity = b1->length;
        result->data = realloc(result->data, result->capacity * sizeof(word));
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

void inplace_multiplication(bignum *source, bignum *mult)
{
    bignum *temp = i128_init();
    multiplication(temp, source, mult);
    cpy(temp, source);
    i128_free(temp);
}

void multiplication(bignum *result, bignum *b1, bignum *b2)
{
    int i, j, k;
    word carry, temp;
    unsigned long long int prod; /* Long for intermediate product... this is not portable and should probably be changed */
    if (b1->length + b2->length > result->capacity)
    {
        result->capacity = b1->length + b2->length;
        result->data = realloc(result->data, result->capacity * sizeof(word));
    }
    for (i = 0; i < b1->length + b2->length; i++)
        result->data[i] = 0;

    for (i = 0; i < b1->length; i++)
    {
        for (j = 0; j < b2->length; j++)
        {
            prod = (b1->data[i] * (unsigned long long int)b2->data[j]) + (unsigned long long int)(result->data[i + j]); /* This should not overflow... */
            carry = (word)(prod / RADIX);

            /* Add carry to the next word over, but this may cause further overflow.. propogate */
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

void inplave_divide(bignum *source, bignum *div)
{
    bignum *q = i128_init(), *r = i128_init();
    division(q, r, source, div);
    cpy(q, source);
    i128_free(q);
    i128_free(r);
}

void inplace_divider(bignum *source, bignum *div, bignum *remainder)
{
    bignum *q = i128_init(), *r = i128_init();
    division(q, r, source, div);
    cpy(q, source);
    cpy(r, remainder);
    i128_free(q);
    i128_free(r);
}

void remainder_op(bignum *source, bignum *div, bignum *remainder)
{
    bignum *q = i128_init();
    division(q, remainder, source, div);
    i128_free(q);
}

void modular_op(bignum *source, bignum *modulus)
{
    bignum *q = i128_init(), *r = i128_init();
    division(q, r, source, modulus);
    cpy(r, source);
    i128_free(q);
    i128_free(r);
}

void division(bignum *quotient, bignum *remainder, bignum *b1, bignum *b2)
{
    bignum *b2copy = i128_init(), *b1copy = i128_init();
    bignum *temp = i128_init(), *temp2 = i128_init(), *temp3 = i128_init();
    bignum *quottemp = i128_init();
    word carry = 0;
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
            quotient->data = realloc(quotient->data, quotient->capacity * sizeof(word));
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
            quotient->data = realloc(quotient->data, (n - m) * sizeof(word));
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
		 * we introduce a dummy zero word to artificially inflate it. */
        if (b1copy->length != n)
        {
            b1copy->length++;
            if (b1copy->length > b1copy->capacity)
            {
                b1copy->capacity = b1copy->length;
                b1copy->data = realloc(b1copy->data, b1copy->capacity * sizeof(word));
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
                temp3->data = realloc(temp3->data, temp3->capacity * sizeof(word));
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

void mod_pow(bignum *base, bignum *exponent, bignum *modulus, bignum *result)
{
    bignum *a = i128_init(), *b = i128_init(), *c = i128_init();
    bignum *discard = i128_init(), *remainder = i128_init();
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

void compute_gcd(bignum *b1, bignum *b2, bignum *result)
{
    bignum *a = i128_init(), *b = i128_init(), *remainder = i128_init();
    bignum *temp = i128_init(), *discard = i128_init();
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

void compute_inverse(bignum *a, bignum *m, bignum *result)
{
    bignum *remprev = i128_init(), *rem = i128_init();
    bignum *auxprev = i128_init(), *aux = i128_init();
    bignum *rcur = i128_init(), *qcur = i128_init(), *acur = i128_init();

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

int jacobi_c(bignum *ac, bignum *nc)
{
    bignum *remainder = i128_init(), *twos = i128_init();
    bignum *temp = i128_init(), *a = i128_init(), *n = i128_init();
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

int solovayPrime(int a, bignum *n)
{
    bignum *ab = i128_init(), *res = i128_init(), *pow = i128_init();
    bignum *modpow = i128_init();
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

int probablePrime(bignum *n, int k)
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

void randPrime(int numDigits, bignum *result)
{
    char *string = malloc((numDigits + 1) * sizeof(char));
    int i;
    string[0] = (rand() % 9) + '1';                 /* No leading zeros */
    string[numDigits - 1] = (rand() % 5) * 2 + '1'; /* Last digit is odd */
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

void randExponent(bignum *phi, int n, bignum *result)
{
    bignum *gcd = i128_init();
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

void encode(bignum *m, bignum *e, bignum *n, bignum *result)
{
    mod_pow(m, e, n, result);
}

void decode(bignum *c, bignum *d, bignum *n, bignum *result)
{
    mod_pow(c, d, n, result);
}

bignum *encodeMessage(int len, int bytes, char *message, bignum *exponent, bignum *modulus)
{
    /* Calloc works here because capacity = 0 forces a realloc by callees but we should really
	 * i128_init() all of these */
    int i, j;
    bignum *encoded = calloc(len / bytes, sizeof(bignum));
    bignum *num128 = i128_init(), *num128pow = i128_init();
    bignum *x = i128_init(), *current = i128_init();
    from_int_to_i128(num128, 128);
    from_int_to_i128(num128pow, 1);
    for (i = 0; i < len; i += bytes)
    {
        from_int_to_i128(x, 0);
        from_int_to_i128(num128pow, 1);
        /* Compute buffer[0] + buffer[1]*128 + buffer[2]*128^2 etc (base 128 representation for characters->int encoding)*/
        for (j = 0; j < bytes; j++)
        {
            from_int_to_i128(current, message[i + j]);
            inplace_multiplication(current, num128pow);
            inplace_addition(x, current); /*x += buffer[i + j] * (1 << (7 * j)) */
            inplace_multiplication(num128pow, num128);
        }
        encode(x, exponent, modulus, &encoded[i / bytes]);
#ifndef NOPRINT
        print_i128(&encoded[i / bytes]);
        printf(" ");
#endif
    }
    return encoded;
}

int *decodeMessage(int len, int bytes, bignum *cryptogram, bignum *exponent, bignum *modulus)
{
    int *decoded = malloc(len * bytes * sizeof(int));
    int i, j;
    bignum *x = i128_init(), *remainder = i128_init();
    bignum *num128 = i128_init();
    from_int_to_i128(num128, 128);
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
#ifndef NOPRINT
            printf("%c", (char)(decoded[i * bytes + j]));
#endif
        }
    }
    return decoded;
}

int main(void)
{
    int i, bytes, len;
    bignum *p = i128_init(), *q = i128_init(), *n = i128_init();
    bignum *phi = i128_init(), *e = i128_init(), *d = i128_init();
    bignum *bbytes = i128_init(), *shift = i128_init();
    bignum *temp1 = i128_init(), *temp2 = i128_init();

    bignum *encoded;
    int *decoded;
    char *buffer;
    FILE *f;

    srand(time(NULL));

    randPrime(FACTOR_DIGITS, p);
    printf("Got first prime factor, p = ");
    print_i128(p);
    printf(" ... ");
    getchar();

    randPrime(FACTOR_DIGITS, q);
    printf("Got second prime factor, q = ");
    print_i128(q);
    printf(" ... ");
    getchar();

    multiplication(n, p, q);
    printf("Got modulus, n = pq = ");
    print_i128(n);
    printf(" ... ");
    getchar();

    subtraction(temp1, p, &NUMS[1]);
    subtraction(temp2, q, &NUMS[1]);
    multiplication(phi, temp1, temp2); /* phi = (p - 1) * (q - 1) */
    printf("Got totient, phi = ");
    print_i128(phi);
    printf(" ... ");
    getchar();

    randExponent(phi, EXPONENT_MAX, e);
    printf("Chose public exponent, e = ");
    print_i128(e);
    printf("\nPublic key is (");
    print_i128(e);
    printf(", ");
    print_i128(n);
    printf(") ... ");
    getchar();

    compute_inverse(e, phi, d);
    printf("Calculated private exponent, d = ");
    print_i128(d);
    printf("\nPrivate key is (");
    print_i128(d);
    printf(", ");
    print_i128(n);
    printf(") ... ");
    getchar();

    /* Compute maximum number of bytes that can be encoded in one encryption */
    bytes = -1;
    from_int_to_i128(shift, 1 << 7); /* 7 bits per char */
    from_int_to_i128(bbytes, 1);
    while (isLess(bbytes, n))
    {
        inplace_multiplication(bbytes, shift); /* Shift by one byte, NB: we use bitmask representative so this can actually be a shift... */
        bytes++;
    }

    printf("Opening file \"text.txt\" for reading\n");
    f = fopen("text.txt", "r");
    if (f == NULL)
    {
        printf("Failed to open file \"text.txt\". Does it exist?\n");
        return EXIT_FAILURE;
    }
    len = readFile(f, &buffer, bytes); /* len will be a multiple of bytes, to send whole chunks */

    printf("File \"text.txt\" read successfully, %d bytes read. Encoding byte stream in chunks of %d bytes ... ", len, bytes);
    getchar();
    printf("\n");
    encoded = encodeMessage(len, bytes, buffer, e, n);
    printf("\n\nEncoding finished successfully ... ");
    getchar();

    printf("Decoding encoded message ... ");
    getchar();
    printf("\n");
    decoded = decodeMessage(len / bytes, bytes, encoded, d, n);

    /* Eek! This is why we shouldn't of calloc'd those! */
    for (i = 0; i < len / bytes; i++)
        free(encoded[i].data);
    free(encoded);
    free(decoded);
    free(buffer);
    i128_free(p);
    i128_free(q);
    i128_free(n);
    i128_free(phi);
    i128_free(e);
    i128_free(d);
    i128_free(bbytes);
    i128_free(shift);
    i128_free(temp1);
    i128_free(temp2);
    fclose(f);

    return EXIT_SUCCESS;
}