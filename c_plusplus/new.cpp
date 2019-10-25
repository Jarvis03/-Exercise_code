#include <iostream>

using namespace std;
int main ()
{
    int *p = new int[10]{1,3,4};
    int *q = new int(2345);
    cout << "new:" << *q << endl;
    for (int i= 0; i < 10; i++){
        cout << i <<":" <<p[i] << endl;
    }
    delete[] p;
    p= NULL;
    delete p;
    p = NULL;
    return 0;

}
