#pragma once

#include "hero.h"

class HeroFactory {
private:
public:
    virtual Hero* CreateHero(const std::string& input) = 0;
};
