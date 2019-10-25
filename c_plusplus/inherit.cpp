#include <iostream>
using namespace std;
class Human {
    string m_name;
    int m_age;

public:
    Human(const string &name ,int age):m_name(name),m_age(age){}
    void move()
    {
        cout << "walk" << endl;
    }
    void show(){
        cout << "name:" << m_name << endl;
        cout << "age:" << m_age << endl;
    }
    ~Human(){}
};

class Teacher: public Human
{
    int m_salary;
public:
    Teacher(const string &name,int age,int salary) : m_salary(salary), Human(name,age){}
    void show()
    {
        cout << "teacher salary:" << m_salary << endl;
    }
};

class Student: public Human
{
   int m_id;
public:

    Student(const string &name,int age,int id) : m_id(id), Human(name,age){}
    void show()
    {
        cout << "student id:" << m_id << endl;
    }
};

int main ()
{
    Teacher math("math",50,10000);
    Student xiaoming("xiaoming",18,9527);
    math.show();
    xiaoming.show();
    math.move();
    return 0;
}
