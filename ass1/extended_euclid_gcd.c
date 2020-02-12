#include<stdio.h>

int calculateGCD(int r0, int r1, int s0, int s1, int t0, int t1){
	int q = r0/r1;
	int r = r0%r1;
	int s = (s0-s1*q);
	int t = (t0-t1*q);
	printf("%d\t%d\t%d\t%d\t\n", q, r, s, t);
	if(r==0) return r1;
	else calculateGCD(r1, r, s1, s, t1, t);
}

void main(){
	int n1, n2;
	scanf("%d", &n1);
	scanf("%d", &n2);
	printf("q\tr\ts\tt\t\n");
	int gcd = calculateGCD(n1, n2, 0, 1, 1, 0);
	printf("GCD of %d and %d is %d\n", n1, n2, gcd);
}
