#include <stdio.h>
#include "algo.h"
#include <unistd.h>
int main(int argc, char** argv)
{
     printf("hello world!\n");
    test_algo(0);
    while(1) {
        sleep(2);
    }
    return 0;
}
