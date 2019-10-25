#ifndef __MarvelHeroClass_H_
#define __MarvelHeroClass_H_
#include <string>
//using namespace std::string;
class MarvelHero {

    std::string m_name;
    int    m_age;
    std::string m_weapon;
public:
    MarvelHero(const std::string &name, const int &age, const std::string &weapon);

    void heroAttr();
    void modifyWeapon(const std::string & weapon);
};
#endif
