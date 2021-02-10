#pragma once

#include <string>

#include "src/terrain_type.h"

class Game{
	public:
	Game();
	~Game();

  	std::string left_player_cfg_file;
	std::string right_player_cfg_file;
	bool ShowHexes;

	// Second player control (Human or CPU)
	int control2;

	TerrainType felteren;

	bool LoadBattleIniFile();

};