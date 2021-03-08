#include "game.h"

#include "src/thirdparty/mini/ini.h"

Game::Game() {

}

Game::~Game() {

}

bool Game::LoadBattleIniFile() {
	// first, create a file instance
	mINI::INIFile file("battle.ini");

	// next, create a structure that will hold data
	mINI::INIStructure ini;

	// now we can read the file
	file.read(ini);

	std::string::size_type sz;

	// Load the settings

	//# Only for esthetics for now
	//# Terrain type impacts the picture drawn for the background
	//# values
	//#   1: Grass
	//#   2: Dirt
	std::string str_terrain_type = ini.get("battle").get("terrain");
	int num_terrain_type = std::stoi (str_terrain_type,&sz);
	felteren = static_cast<TerrainType>(num_terrain_type);

	//# Shows or hides the hexes
	//# values
	//#   0: don't show hexes
	//#   1: show hexes
	std::string str_show_hexes = ini.get("battle").get("show_hexes");
	ShowHexes = std::stoi (str_show_hexes,&sz);

	//# Opponent type (right side player can be either human of CPU controlled; left side player is always HUMAN controlled)
	//# values
	//#   0: HUMAN
	//#   1: CPU
	std::string str_control2 = ini.get("battle").get("right_player_control");
	control2 = std::stoi (str_control2,&sz);

	//# Config file for left side hero and army
	left_player_cfg_file = ini.get("battle").get("left_side_player_config");

	//# Config file for right side hero and army
	right_player_cfg_file = ini.get("battle").get("right_side_player_config");

	return true;
}



//---------------------------------------------------------------------------
int GetX(int x, int y, int dir) {
	int ret = 0;
	switch (dir) {
	case STGSUS:
		if (y % 2 == 1)
			ret = x;
		else
			ret = x - 1;
		break;
	case STG:
		ret = x - 1;
		break;
	case STGJOS:
		if (y % 2 == 1)
			ret = x;
		else
			ret = x - 1;
		break;
	case DRPJOS:
		if (y % 2 == 0)
			ret = x;
		else
			ret = x + 1;
		break;
	case DRP:
		ret = x + 1;
		break;
	case DRPSUS:
		if (y % 2 == 0)
			ret = x;
		else
			ret = x + 1;
		break;
	}
	return ret;
}

//---------------------------------------------------------------------------
int GetY(int x, int y, int dir) {
	int ret = 0;
	switch (dir) {
	case STGSUS:
	case DRPSUS:
		ret = y - 1;
		break;
	case DRP:
	case STG:
		ret = y;
		break;
	case STGJOS:
	case DRPJOS:
		ret = y + 1;
		break;
	}
	return ret;
}


void Game::InitializeTerrainMatrix() {
	for (size_t i = 0; i < MAP_WIDTH - 1; i++) {
		for (size_t j = 0; j < MAP_HEIGHT - 1; j++) {
			teren[i][j] = 0;
		}
	}
}

void Game::PlaceAllArmiesOnTerrainMatrix() {
	// Layout the armies of the two players on game map/matrix
	for (size_t player_idx = 0; player_idx < 2; ++player_idx) {
		for (size_t j = 0; j <= Player[player_idx]->angajati; j++) {
			teren[Player[player_idx]->army_slots[j]->x]
				[Player[player_idx]->army_slots[j]->y] =
				10 + player_idx * 10 + j;
		}
	}
}
