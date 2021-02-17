#include "hero.h"
#include <stdio.h>
#include "filehelperfunctions.h"

// ---------------------------------------------------------------------------
bool Hero::IncarcaErou(const std::string& numefisier) {
	if (false == doesFileExist(numefisier)) {
		return false;
	}

	FILE *f = fopen(numefisier.c_str(), "r");
	readFromFileUpToChar(f);
	char old_c_name[1024];
	fscanf(f, "%s", old_c_name);
    nume = old_c_name;

	readFromFileUpToChar(f);
	fscanf(f, "%d", &angajati);

	if (angajati > 6) {
		angajati = 0;
	}

	for (int i = 0; i <= angajati; i++) {
		this->army_slots.push_back(new Soldier());
		int ranged_value = 0;
		readFromFileUpToChar(f);
		fscanf(f, "%d%d%d%d%d", &army_slots[i]->Level, &army_slots[i]->Xp, &army_slots[i]->XpMax,
			&army_slots[i]->Hp, &army_slots[i]->HpMax);
		fscanf(f, "%d%d%d%d%d", &army_slots[i]->MovesMax, &army_slots[i]->DamageMin,
			&army_slots[i]->DamageMax, &army_slots[i]->Armor, &army_slots[i]->Protection);
		fscanf(f, "%d%d%d%d%d", &army_slots[i]->ChancesToHit, &army_slots[i]->Upkeep,
			&army_slots[i]->ImagineAsoc, &army_slots[i]->PortretAsoc, &ranged_value);
        army_slots[i]->Ranged = ranged_value;
		fscanf(f, "%d%d%d", &army_slots[i]->Ammo, &army_slots[i]->RetalNum, &army_slots[i]->y);
		army_slots[i]->Retal = army_slots[i]->RetalNum;
		army_slots[i]->MovesLeft = army_slots[i]->MovesMax;
	}

	fclose(f);

	return true;
};
