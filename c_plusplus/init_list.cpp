#include <iostream>
using namespace std;
class object {
    public:
    object( string &name, int age):m_name(name),m_age(age){}
    void print(){
        cout << "name:" << m_name << endl;
        cout << "age:" << m_age << endl;
    }
    void default_value() {
        m_name = "jarvis";
        cout << "default:" << m_name << endl;
    }
    private:
     string &m_name;
    int m_age;
};
int main(void)
{
    string name = "you";
    object test(name, 20);
    test.print();
    test.default_value();
    cout << "raw:" << name << endl;
    return 0;
}
