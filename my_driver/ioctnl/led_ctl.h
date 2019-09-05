#ifndef __LED_CTL_H_
#define __LED_CTL_H_
//#include <sys/ioctl.h>
#define LED_RED_ON   _IOW('A', 0, int)
#define LED_RED_OFF  _IOW('A', 1, int)
#define ACCESS_W     _IOW('B', 0, int)
#define ACCESS_R     _IOR('B', 0, int)

#endif
