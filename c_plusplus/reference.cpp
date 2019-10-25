#include <iostream>
using namespace std;

int main(void)
{

    int a = 100;
        int c = 200;
    const int &b = a;
    cout << "ref:" << b << endl;
    cout << "refaddr" << hex << &b << endl;
    cout << "addr" << hex << &a << endl;
    return 0;
}
