#include <iostream>
#include <cstring>
using namespace std;
class MyString {
    char *buffer;
    public:
    MyString(const char* str = ""):buffer(strcpy(new char[strlen(str) + 1],str))
    {
        //buffer = new char [strlen(str) + 1];
        //strcpy(buffer,str);
        cout << "MyString constructor:"  << endl;

    }
    MyString(const MyString& copy)
    {
      buffer = new char [strlen(copy.buffer) + 1];
      strcpy(buffer, copy.buffer);
      cout << " MyString copy from"   << endl;


    }
    int get_size()
    {
        return strlen(buffer);
    }
    char *get_str()
    {
        return buffer;
    }
    ~MyString()
    {
        delete[] buffer;
        cout << " MyString distructor" << endl;
    }
};
int main(void)
{
    MyString str1("it is a test");
    cout <<"str1 size:"<<  str1.get_size() << endl;
    cout <<" str1:" << str1.get_str() << endl;
    MyString str2 = str1;
    cout <<"str2 size:"<<  str2.get_size() << endl;
    cout <<" str2:" << str2.get_str() << endl;
    return 0;
}
