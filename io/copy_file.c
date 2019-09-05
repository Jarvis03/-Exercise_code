#include <stdio.h>
int main(void)
{
    FILE *fp = NULL, *fw = NULL;
    int ret = 0;
	char buf[50];
	char *rev = NULL;
	fp = fopen("raw.txt","r");
    fw = fopen("copy.txt","w");
	if (fp == NULL) {
	    printf("file open error\n");

	}
	printf("fp is %d\n",*fp);
	if (fw == NULL) {
	    printf("copy file error\n");
	}
#if 0
    ret = fgetc(fp);
    while(ret != EOF){

	    fputc(ret,fw);
		ret = fgetc(fp);
	}
#else
    while((rev = fgets(buf,6,fp)) != NULL) {
	    fputs(buf,fw);
	//	printf("line\n");
	}
#endif
	printf("it is done\n");
	fclose(fp);
	fclose(fw);

}
