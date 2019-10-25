#include <iostream>
using namespace std;
class MyString {
    string m_name;
    public:
    MyString(const string name):m_name(name)
    {
        cout << "MyString constructor:" << name << endl;

    }
    MyString(const MyString& copy)
    {
      cout << m_name << " MyString copy from" << copy.m_name  << endl;
      m_name = copy.m_name;

    }
    ~MyString()
    {
        cout << m_name << " MyString distructor" << endl;
    }
};
class MyObject {
    public:
     MyObject( string name, int age):m_name(name),m_age(age),a("inter"){

         cout << "MyObject constructor:" << m_name  << endl;
    }
    void print(){
        cout << "name:" << m_name << endl;
        cout << "age:" << m_age << endl;
    }
    void default_value() {
        m_name = "jarvis";
        cout << "default:" << m_name << endl;
    }
    MyObject(const MyObject & copy):m_name(copy.m_name),m_age(copy.m_age),a(copy.a)
    {
        cout << m_name <<" MyObject copy"  << endl;
    }
    ~MyObject()
    {
        cout << m_name <<  " MyObject distructor" << endl;
    }
    private:
    MyString a;
    string m_name;
    int m_age;
};
int main(void)
{
    string name = "ironman";
    MyObject obj1(name, 40);
    cout << "init done" << endl;
    MyObject test(obj1);
    //test.print();
    //test.default_value();
    //cout << "raw:" << name << endl;
    //MyObject objlast = obj1;
    return 0;
}
