#include <stdio.h>

int mult_inv(int x, int m)
{
    int i;
    x = x % m;
    for (i = 1; i < m; i++)
    {
        if ((x * i) % m == 1)
            return i;
    }
    return 0;
}

int mult(int *a, int n)
{
    int i, prod = 1;
    for (i = 0; i < n; i++)
        prod *= a[i];
    return prod;
}

int chRemainderTh(int n, int *B, int *M)
{
    int i, x = 0;
    int N = mult(M, n);
    for (i = 0; i < n; i++)
    {
        int ni, invi;
        ni = N / M[i];
        invi = mult_inv(ni, M[i]);
        x += B[i] * ni * invi;
    }
    return x % N;
}

void test1()
{
    int n = 3;
    printf("No of congruent equations %d\n", n);
    int B[] = {3, 1, 6};
    int M[] = {5, 7, 8};
    int i;
    printf("Set of equations are:\n");
    for (i = 0; i < n; i++)
    {
        printf("x = %d mod(%d)\n", B[i], M[i]);
    }
    printf("Value of X: %d\n", chRemainderTh(n, B, M));
}

// Compute of the given assignment result
void test2()
{
    int n = 4;
    printf("No of congruent equations %d\n", n);
    int B[] = {6, 13, 9, 19};
    int M[] = {11, 16, 21, 25};
    int i;
    printf("Set of equations are:\n");
    for (i = 0; i < n; i++)
    {
        printf("x = %d mod(%d)\n", B[i], M[i]);
    }
    printf("Value of X: %d\n", chRemainderTh(n, B, M));
}

int main(int argc, char const *argv[])
{
    test1();
    printf("-----------------------------------------\n");
    test2();

    return 0;
}
