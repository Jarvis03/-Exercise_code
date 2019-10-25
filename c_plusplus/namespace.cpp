#include <iostream>
namespace book {
    int ISDN = 1234;
    char name[100] = "linux kernel";
    int data = 2019;
}
union type {
    unsigned int a;
    unsigned char b;
};
using namespace std;
union type test;

int main(void)
{
    using namespace book;
    test.a = 0x12345678;
    char value = test.b;
    char hex_value = 64;
    cout <<"value:" << hex << int(value) << endl;
  //  cout << "name:" << name << endl << "isdn :" << ISDN << endl \
  //       << "data:" << data << endl;


    return 0;
}
