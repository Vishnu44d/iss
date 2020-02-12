#include<stdio.h>

int add_inv(int x, int m)
{
	x=x%m;
	return m-x;
}

int mult_inv(int x, int m)
{
	int i;
	x=x%m;
	for(i=1; i<m; i++)
	{
		if((x*i)%m==1)
		{
			//printf("ABCD\n");
			return i;
			
		}
	}
	return 0;
}



void main(void)
{
	printf("%d \n", add_inv(3, 12));
	if(mult_inv(3, 11)==0)
	{
		printf("No mult_inv\n");
	} 
	else{
		printf("%d\n", mult_inv(3, 11));
	}
}
