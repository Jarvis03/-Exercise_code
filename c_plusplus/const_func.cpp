#include <iostream>
#include <cstring>
using namespace std;
class Human {
    char *buffer;
    int m_age;
    string m_name;
    public:
    Human(const char* str = "",int age = 18,string name = "nobody"):
        buffer(strcpy(new char[strlen(str) + 1],str)),m_age(age),m_name(name)
    {
        cout << "Human constructor:"  << endl;

    }
    Human(const Human& copy)
    {
      buffer = new char [strlen(copy.buffer) + 1];
      strcpy(buffer, copy.buffer);
      m_age = copy.m_age;
      m_name = copy.m_name; //todo
      cout << " Human copy"   << endl;


    }
    int get_age() const
    {
        // do not  age++
        return m_age;
    }
    char *get_str()
    {
        return buffer;
    }
    ~Human()
    {
        delete[] buffer;
        cout << " Human distructor" << endl;
    }
};
int main(void)
{
    const Human man("it is a man");
    cout <<"man age:"<<  man.get_age() << endl;
    //cout <<" man:" << man.get_str() << endl;
    Human woman;
    cout <<"woman age:"<<  woman.get_age() << endl;
    cout <<" woman:" << woman.get_str() << endl;
    return 0;
}
