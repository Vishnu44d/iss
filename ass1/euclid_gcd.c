#include<stdio.h>

int gcd(int m, int n)
{
	int t=m-n*(m/n);
	if(t==0)
		return n;
	else
		gcd(n, t);
}

int main(void)
{
	printf("%d\n", gcd(6, 4));
	return 0;
}
