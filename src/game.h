#pragma once

#include <string>

#include "src/terrain_type.h"
#include "src/hero.h"

enum HexDirection {
	Invalid = -1, TopLeft = 0, Left = 1, BottomLeft = 2, BottomRight = 3,
	Right = 4, TopRight = 5
};

#define STGSUS 0
#define STG    1
#define STGJOS 2
#define DRPJOS 3
#define DRP    4
#define DRPSUS 5

#define MAP_WIDTH 10
#define MAP_HEIGHT 8

class Game {
public:
	Game();
	~Game();

	// TODO: move functionality from UnitMain; refactor into single function.
	int GetX(int x, int y, int dir);
	int GetY(int x, int y, int dir);

	std::string left_player_cfg_file;
	std::string right_player_cfg_file;

	bool ShowHexes;

	// Second player control (Human or CPU)
	int control2;

	TerrainType felteren;

	bool LoadBattleIniFile();

	int FazaJoc;
	bool WaitingForOrder;

	Hero *Player[2];

	int selected[MAP_WIDTH - 1][MAP_HEIGHT - 1]; // matr selectarilor
	int teren[MAP_WIDTH - 1][MAP_HEIGHT - 1]; // matricea sistem

	void InitializeTerrainMatrix();
    void PlaceAllArmiesOnTerrainMatrix();
};
