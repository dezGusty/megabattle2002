#include "soldier.h"

// ---------------------------------------------------------------------------
std::string Soldier::GetNume() const {
	std::string ret;
	switch (PortretAsoc) {
	case 0:
		ret = "Peasant";
		break;
	case 1:
		ret = "Peasant Archer";
		break;
	case 2:
		ret = "Pikeman";
		break;
	case 3:
		ret = "Lizardman";
		break;
	case 4:
		ret = "Crossbowman";
		break;
	case 5:
		ret = "Halberdier";
		break;
	case 6:
		ret = "Paladin";
		break;
	case 7:
		ret = "Longbowman";
		break;
	case 8:
		ret = "Magic Sprite";
		break;
	case 9:
		ret = "Elemental";
		break;
	case 10:
		ret = "Dead Unit";
		break;

	default:
		ret = "Hero Unit";
	}
	return ret;
}
