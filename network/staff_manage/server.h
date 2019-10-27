
#include <stdint.h>
// Do not use magic number
// Data length define
#define PHONE_LEN     20
#define NAME_LEN      20
#define ADDR_LEN      30
#define PASSWORD_LEN  15

#define SERVER_HOST_ADDR  "192.168.199.110"
/*
 * staff information
 */
struct staff_info {
    uint32_t number;
    char     phone[PHONE_LEN];
    char     name[NAME_LEN];
    char     addr[ADDR_LEN];
    char     age;
    int      level;
    double   salary;
};

struct user {
    char  flag;
    char  name[NAME_LEN];
    char  psword[PASSWORD_LEN];
};

//interface



