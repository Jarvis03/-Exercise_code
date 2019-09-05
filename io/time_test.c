#include<stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

FILE * fp = NULL;

void stop(int arg)
{
	fclose(fp);
    printf("is done\n");
	_exit(0);
}
int main()
{
	char data[200];
    time_t now ;
    struct tm *mytm;
	int num = 0;
	fp = fopen("time.txt","a+");
//fseek(fp,0,SEEK_END);
    if(fp == NULL) {
	    printf("file open faild\n");
		return 0;
	}
	//signal(SIGINT,stop);
   // rewind(fp);
   // fputs("you",fp);
    printf("is:%d\n",ftell(fp));

    while(fgets(data,200,fp) != NULL)
	{
	   num++;
       break;
	}
    printf("line:%d\n",num);

	//fseek(fp,0,SEEK_END);

    printf("is:%d\n",ftell(fp));
	while (1) {
	  num++;
	  now = time(NULL);
      mytm = localtime(&now);
	  fprintf(fp,"%d,%d-%d-%d %d:%d:%d\n",num,mytm->tm_year+1900,mytm->tm_mon+1,\
			  mytm->tm_mday,mytm->tm_hour,mytm->tm_min,mytm->tm_sec);
	  printf("%d,h:%d,m:%d,s:%d\n",num,mytm->tm_hour,mytm->tm_min,mytm->tm_sec);
      fflush(fp);
	  sleep(1);
	}
	fclose(fp);
	return 0;
}
