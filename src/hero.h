#pragma once

#include <string>
#include <vector>

#include "soldier.h"

class Hero {
private:
public:
	std::string nume;
	int portret;

	std::vector<Soldier*> army_slots;

	int control; // 1=human; 2=computer;
	int angajati; // nr de mercenari

	// THero(char numefisier[35]) {
	// IncarcaErou(numefisier);
	// }

	bool IncarcaErou(const std::string& numefisier);
};
