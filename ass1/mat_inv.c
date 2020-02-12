#include<stdio.h>  
#define N 3
  
void getCofactor(int A[N][N], int temp[N][N], int p, int q, int n) 
{ 
    int i = 0, j = 0, row, col; 

    for (row = 0; row < n; row++) 
    { 
        for (col = 0; col < n; col++) 
        { 
            if (row != p && col != q) 
            { 
                temp[i][j++] = A[row][col]; 
                if (j == n - 1) 
                { 
                    j = 0; 
                    i++; 
                } 
            } 
        } 
    } 
} 

int determinant(int A[N][N], int n) 
{ 
    int D = 0; 

    if (n == 1) 
        return A[0][0]; 
  
    int temp[N][N];
  
    int sign = 1, f;  
    for (f = 0; f < n; f++) 
    {  
        getCofactor(A, temp, 0, f, n); 
        D += sign * A[0][f] * determinant(temp, n - 1);  
        sign = -sign; 
    } 
  
    return D; 
} 
  
void adjoint(int A[N][N],int adj[N][N]) 
{ 
    if (N == 1) 
    { 
        adj[0][0] = 1; 
        return; 
    } 
    int sign = 1, temp[N][N]; 
    int i, j;
    for (i=0; i<N; i++) 
    { 
        for (j=0; j<N; j++) 
        { 
            getCofactor(A, temp, i, j, N); 
            sign = ((i+j)%2==0)? 1: -1; 
            adj[j][i] = (sign)*(determinant(temp, N-1)); 
        } 
    } 
} 
  
int inverse(int A[N][N], float inverse[N][N]) 
{ 
    int det = determinant(A, N); 
    if (det == 0) 
    { 
        printf("Inverse does not exist\n"); 
        return 0; 
    } 

    int adj[N][N]; 
    adjoint(A, adj); 
    int i, j;
    for (i=0; i<N; i++) 
        for (j=0; j<N; j++) 
            inverse[i][j] = adj[i][j]/(float)det; 
  
    return 1; 
} 
  

void displayINT(int A[N][N]) 
{ 
    int i, j;
    for (i=0; i<N; i++) 
    { 
        for (j=0; j<N; j++) 
            printf("%d ", A[i][j]); 
        printf("\n"); 
    } 
} 

void displayFLOAT(float A[N][N]) 
{ 
    int i, j;
    for (i=0; i<N; i++) 
    { 
        for (j=0; j<N; j++) 
            printf("%f ", A[i][j]); 
        printf("\n"); 
    } 
}

int main() 
{ 

    int A[N][N] = { {1, 2, 3}, 
                    {4, 5, 6}, 
                    {7, 8, 9}
                }; 
  
    int adj[N][N];  // To store adjoint of A[][] 
  
    float inv[N][N]; // To store inverse of A[][] 
  
    printf("Input matrix is :\n"); 
    displayINT(A); 
  
    printf("\nThe Adjoint is :\n"); 
    adjoint(A, adj); 
    displayINT(adj); 
  
    printf("\nThe Inverse is :\n"); 
    if (inverse(A, inv)) 
        displayFLOAT(inv); 
  
    return 0; 
} 