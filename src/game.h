#pragma once

#include <string>

#include "src/coord.h"
#include "src/hero.h"
#include "src/terrain_type.h"

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

#define TOPLEFT  0
#define LEFT     1
#define BOTLEFT  2
#define BOTRIGHT 3
#define RIGHT    4
#define TOPRIGHT 5

#define MAP_WIDTH 9
#define MAP_HEIGHT 7

class Game {
public:
	Game();
	~Game();

	// TODO: move functionality from UnitMain; refactor into single function.
	int GetX(int x, int y, int dir);
	int GetY(int x, int y, int dir);

	/**
		Get a neighbouring cell for a source cell, in a desired target direction.
		@param sourceCell The source to use as starting point.
		@param direction  The direction in which to get the neighbour. Should be one of:
		- TOPLEFT
		- LEFT
		- BOTLEFT
		- BOTRIGHT
		- RIGHT
		- TOPRIGHT

		@return The coordinates of the neighbour cell.
		TODO: make static
	*/
	Coord GetNeighbourCell(Coord sourceCell, int direction);

	/**
		Check to see whether there is a neighbouring cell (for a source cell), in a desired target direction.
		@param sourceCell The source to use as starting point.
		@param direction  The direction in which to get the neighbour. Should be one of:
		- TOPLEFT
		- LEFT
		- BOTLEFT
		- BOTRIGHT
		- RIGHT
		- TOPRIGHT

		@return True if there is a neighbour in the desired direction.
				False otherwise.
		TODO: make static
	*/
	bool DoesNeighbourExist(Coord sourceCell, int direction);

	/**
		Check to see whether a neighbouring cell is available, in a desired target direction.

		@param sourceCell The source to use as starting point.
		@param direction  The direction in which to get the neighbour. Should be one of:
		- TOPLEFT
		- LEFT
		- BOTLEFT
		- BOTRIGHT
		- RIGHT
		- TOPRIGHT

		@return True if there is a neighbour in the desired direction.
				False otherwise.
		TODO: make static
	*/
	bool IsNeighbourTerrainAvailable(Coord sourceCell, int direction);

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

	int selected[MAP_WIDTH][MAP_HEIGHT]; // matr selectarilor
	int teren[MAP_WIDTH][MAP_HEIGHT]; // matricea sistem

	void InitializeTerrainMatrix();
	void PlacePlayerArmyOnTerrainMatrix(const Hero& hero);

    void ResetSelectionMatrix();
	void MarkCellForRangedAttack(unsigned cell_x, unsigned cell_y);
};
