#include <stdio.h>
int main(void)
{
    int i = 0;
	FILE *fp = NULL;
	do {
       fp = fopen("text.txt","w+");
	
	   i++;
	}while(fp != NULL);
	printf("max :%d\n",i);
}
