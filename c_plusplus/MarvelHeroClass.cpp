#include <iostream>
#include "MarvelHeroClass.h"
using namespace std;
MarvelHero::MarvelHero(const string &name, const int &age, const string &weapon)
{
    cout << "constructor" << endl;
    m_age = age;
    m_name = name;
    m_weapon = weapon;
 }

void MarvelHero::heroAttr()
{
    cout << "name:" <<m_name << endl;
    cout << "age:"  << m_age << endl;
    cout << "weapon:" << m_weapon << endl;
}
void MarvelHero::modifyWeapon(const string & weapon) {
    m_weapon = weapon;
}


