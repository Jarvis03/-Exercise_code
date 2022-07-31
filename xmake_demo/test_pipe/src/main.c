#include <stdio.h>
#include "test_pipe.h"

int main(int argc, char** argv)
{
    printf("test pipe!\n");
    test_pipe(0);
    while(1){
      sleep(2);
    }
    return 0;
}
