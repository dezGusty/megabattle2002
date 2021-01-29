#pragma once

#include <string>

class Soldier {
public:
	bool alive;
	int RetalNum;
	int Retal;
	int Level;
	int Xp;
	int XpMax;
	int Hp;
	int HpMax;
	int MovesLeft;
	int MovesMax;
	int DamageMin;
	int DamageMax;
	int Armor;
	int Protection;
	int ChancesToHit;
	int ImagineAsoc;
	int PortretAsoc;
	bool Ranged;
	int Ammo;
	int Upkeep;
	int Orientare;
	int x;
	int y;

	Soldier() {
		alive = 1;
		Level = 1;
		Xp = 0;
		XpMax = 1;
		Hp = 1;
		HpMax = 1;
		MovesLeft = 1;
		MovesMax = 1;
		DamageMin = 1;
		DamageMax = 2;
		Armor = 0;
		Protection = 0;
		ChancesToHit = 50;
		ImagineAsoc = 1;
		PortretAsoc = 0;
		Ranged = false;
		Ammo = -1;
		Upkeep = 0;
		RetalNum = 1;
		Retal = 1;
		Orientare = 0;
		x = y = 0;
	}

	std::string GetNume() const;
};
