#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
char buf[3] = {0, 0, 0};
int main(void)
{
    int fd = 0;
    int ret = 0;
    unsigned int adc_raw = 0;
    float value = 0.0;
    if (0 > (fd = open("/dev/adc_dev",O_RDWR))) {
        perror("open error\n");
        return 0;
    }
    while (1) {

       ret = read(fd,&adc_raw, 4);
       if(ret < 0) {
           perror("read adc error\n");

       }
       //adc_raw = buf[0] | buf[1] << 8 | buf[2] << 16;
       printf("adc:%x\n",adc_raw);
       value = adc_raw * 1.8 / 4096;
       printf("adc value :%f\n",value);
//       sleep(1);
       memset(buf, 0, 3);
    }
    close(fd);
    return 0;

}
