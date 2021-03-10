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

	//# Shows or hides the debug indices
	//# values
	//#   0: don't show indices
	//#   1: show indices
	std::string str_show_dbg_indices = ini.get("battle").get("show_debug_indices");
	ShowDebugIndices = std::stoi (str_show_dbg_indices,&sz);

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


/**
	Get a neighbouring cell for a source cell, in a desired target direction.
	@param sourceCell The source to use.
	@param direction  The direction in which to get the neighbour. Should be one of:
	- TOPLEFT
	- LEFT
	- BOTLEFT
	- BOTRIGHT
	- RIGHT
	- TOPRIGHT
	@return The coordinates of the neighbour cell.
*/
Coord Game::GetNeighbourCell(Coord sourceCell, int direction){
	Coord result(sourceCell);

	switch (direction) {
	case TOPLEFT:
		result.y = sourceCell.y - 1;
		if (sourceCell.y % 2 == 1) {
			result.x = sourceCell.x;
		}
		else {
			result.x = sourceCell.x - 1;
		}
		break;

	case LEFT:
		result.y = sourceCell.y;
		result.x = sourceCell.x - 1;
		break;

	case BOTLEFT:
		result.y = sourceCell.y + 1;
		if (sourceCell.y % 2 == 1) {
			result.x = sourceCell.x;
		}
		else {
			result.x = sourceCell.x - 1;
		}
		break;

	case TOPRIGHT:
		result.y = sourceCell.y - 1;
		if (sourceCell.y % 2 == 0) {
			result.x = sourceCell.x;
		}
		else {
			result.x = sourceCell.x + 1;
		}
		break;

	case RIGHT:
		result.y = sourceCell.y;
		result.x = sourceCell.x + 1;
		break;

	case BOTRIGHT:
		result.y = sourceCell.y + 1;
		if (result.y % 2 == 0) {
			result.x = sourceCell.x;
		}
		else {
			result.x = sourceCell.x + 1;
		}
		break;

	default: ;
	}

	return result;
}

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
*/
bool Game::DoesNeighbourExist(Coord sourceCell, int direction) {

	// Input validation.
	if (sourceCell.x < 0 || sourceCell.y < 0) {
		return false;
	}

	if (sourceCell.x >= MAP_WIDTH || sourceCell.y >= MAP_HEIGHT) {
		return false;
	}

	// Check the various directions
	switch (direction) {
	case TOPLEFT:
		if (sourceCell.y <= 0) {
			return false;
		}

		if (sourceCell.y % 2 == 0) {
			if (sourceCell.x <= 0) {
				return false;
			}
		}

		break;

	case LEFT:
		if (sourceCell.x <= 0) {
			return false;
		}
		break;

	case BOTLEFT:
		if (sourceCell.y >= 6) {
			return false;
		}
		if (sourceCell.y % 2 == 0) {
			if (sourceCell.x <= 0) {
				return false;
			}
		}
		break;

	case TOPRIGHT:
		if (sourceCell.y <= 0) {
			return false;
		}

		if (sourceCell.y % 2 == 1) {
			if (sourceCell.x >= MAP_WIDTH - 1) {
				return false;
			}
		}
		break;

	case RIGHT:
		if (sourceCell.x >= MAP_WIDTH - 1) {
			return false;
		}
		break;

	case BOTRIGHT:
		if (sourceCell.y >= MAP_HEIGHT - 1) {
			return false;
		}

		if (sourceCell.y % 2 == 1) {
			if (sourceCell.x >= MAP_WIDTH - 1) {
				return false;
			}
		}

		break;

	default: ;
	}

	return true;
}

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
 */
bool Game::IsNeighbourTerrainAvailable(Coord sourceCell, int direction) {
	if (false == DoesNeighbourExist(sourceCell, direction)) {
		return false;
	}

	Coord cell = GetNeighbourCell(sourceCell, direction);
	if (teren[cell.x][cell.y]) {
		return false;
	}

	return true;
}

void Game::InitializeTerrainMatrix() {
	for (size_t i = 0; i < MAP_WIDTH; i++) {
		for (size_t j = 0; j < MAP_HEIGHT; j++) {
			teren[i][j] = 0;
		}
	}
}

void Game::PlacePlayerArmyOnTerrainMatrix(const Hero& hero) {
	// Layout the armies of the two players on game map/matrix
	for (auto count = 0; count < hero.angajati; ++count) {
		teren[hero.army_slots[count]->x][hero.army_slots[count]->y] =
			10 + hero.player_id * 10 + count;
	}
}

void Game::MarkCellForRangedAttack(unsigned cell_x, unsigned cell_y) {
	for (int i = 0; i < MAP_WIDTH; i++) {
		for (int j = 0; j < MAP_HEIGHT; j++) {
			if (!selected[i][j] && teren[i][j]) {
				if (teren[i][j] / 20 != teren[cell_x][cell_y] / 20) {
					selected[i][j] = 2;
				}
			}
		}
	}
}

void Game::ResetSelectionMatrix(){
                           int i, j;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 7; j++) {
			selected[i][j] = 0;
		}
	}
}


// ---------------------------------------------------------------------------
void Game::PathFinding(int x, int y, int mut, int pas) {
//	if (PathwasFound) {
//		return;
//	}
//
//	if (pas > mut) {
//		return;
//	}
//
//	path[pas].x = x;
//	path[pas].y = y;
//	if (TargetX == x && TargetY == y)
//		PathwasFound = true;
//	else if ((game.teren[x][y] == 0) ||
//		(Player[SelectedPlayer]->army_slots[SelectedSlot]->x == x && Player
//		[SelectedPlayer]->army_slots[SelectedSlot]->y == y)) {
//		// prioritise going up
//		int ordin[6] = {TOPLEFT, TOPRIGHT, LEFT, RIGHT, BOTLEFT, BOTRIGHT};
//		if (y > TargetY) {
//			// prioritize going left
//			if (x > TargetX) {
//				if (x - TargetX > y - TargetY) {
//					ordin[0] = TOPLEFT;
//					ordin[1] = LEFT;
//					ordin[2] = BOTLEFT;
//					ordin[3] = TOPRIGHT;
//					ordin[4] = RIGHT;
//					ordin[5] = BOTRIGHT;
//				}
//				else {
//					ordin[0] = TOPLEFT;
//					ordin[1] = TOPRIGHT;
//					ordin[2] = LEFT;
//					ordin[3] = RIGHT;
//					ordin[4] = BOTLEFT;
//					ordin[5] = BOTRIGHT;
//				}
//			}
//			// prior drp
//			if (x < TargetX) {
//				if (TargetX - x > y - TargetY) {
//					ordin[0] = TOPRIGHT;
//					ordin[1] = RIGHT;
//					ordin[2] = BOTRIGHT;
//					ordin[3] = TOPLEFT;
//					ordin[4] = LEFT;
//					ordin[5] = BOTLEFT;
//				}
//				else {
//					ordin[0] = TOPRIGHT;
//					ordin[1] = TOPLEFT;
//					ordin[2] = RIGHT;
//					ordin[3] = LEFT;
//					ordin[4] = BOTRIGHT;
//					ordin[5] = BOTLEFT;
//				}
//			}
//			// prior sus
//			if (x == TargetX) {
//				if (y % 2 == 0) {
//					// TOPRIGHT
//					ordin[0] = TOPRIGHT;
//					ordin[1] = RIGHT;
//					ordin[2] = BOTRIGHT;
//					ordin[3] = TOPLEFT;
//					ordin[4] = LEFT;
//					ordin[5] = BOTLEFT;
//				}
//				else {
//					// TOPLEFT
//					ordin[0] = TOPLEFT;
//					ordin[1] = LEFT;
//					ordin[2] = BOTLEFT;
//					ordin[3] = TOPRIGHT;
//					ordin[4] = RIGHT;
//					ordin[5] = BOTRIGHT;
//				}
//			}
//		}
//
//		if (y == TargetY) // prioritate oriz
//		{
//			if (x > TargetX) // prior stg
//			{
//				ordin[0] = LEFT;
//				ordin[1] = RIGHT;
//				ordin[2] = TOPLEFT;
//				ordin[3] = BOTLEFT;
//				ordin[4] = BOTRIGHT;
//				ordin[5] = TOPRIGHT;
//			}
//			if (x < TargetX) // prior drp
//			{
//				ordin[0] = RIGHT;
//				ordin[1] = LEFT;
//				ordin[2] = TOPRIGHT;
//				ordin[3] = BOTRIGHT;
//				ordin[4] = BOTLEFT;
//				ordin[5] = TOPLEFT;
//			}
//		}
//
//		if (y < TargetY) // prioritate jos
//		{
//			// prior stg
//			if (x > TargetX) {
//				if (TargetY - y >= x - TargetX) {
//					ordin[0] = BOTLEFT;
//					ordin[1] = BOTRIGHT;
//					ordin[2] = LEFT;
//					ordin[3] = RIGHT;
//					ordin[4] = TOPLEFT;
//					ordin[5] = TOPRIGHT;
//				}
//				else {
//					ordin[0] = BOTLEFT;
//					ordin[1] = LEFT;
//					ordin[2] = TOPLEFT;
//					ordin[3] = BOTRIGHT;
//					ordin[4] = RIGHT;
//					ordin[5] = TOPRIGHT;
//				}
//			}
//
//			// prior drp
//			if (x <= TargetX) {
//				if (TargetY - y >= TargetX - x) {
//					ordin[0] = BOTRIGHT;
//					ordin[1] = BOTLEFT;
//					ordin[2] = RIGHT;
//					ordin[3] = LEFT;
//					ordin[4] = BOTLEFT;
//					ordin[5] = TOPLEFT;
//				}
//				else {
//					ordin[0] = BOTRIGHT;
//					ordin[1] = RIGHT;
//					ordin[2] = TOPRIGHT;
//					ordin[3] = BOTLEFT;
//					ordin[4] = LEFT;
//					ordin[5] = TOPLEFT;
//				}
//			}
//
//			// BOTRIGHT
//			if (x == TargetX) {
//				if (y % 2 == 0) {
//					ordin[0] = BOTRIGHT;
//					ordin[1] = BOTLEFT;
//					ordin[2] = RIGHT;
//					ordin[3] = LEFT;
//					ordin[4] = BOTLEFT;
//					ordin[5] = TOPLEFT;
//				}
//				else // BOTLEFT
//				{
//					ordin[0] = BOTLEFT;
//					ordin[1] = BOTRIGHT;
//					ordin[2] = LEFT;
//					ordin[3] = RIGHT;
//					ordin[4] = TOPLEFT;
//					ordin[5] = TOPRIGHT;
//				}
//			}
//		}
//		for (int k = 0; k <= 5; k++) {
//			int i = ordin[k];
//			if (game.DoesNeighbourExist({x, y}, i)) {
//				Coord nextCell = game.GetNeighbourCell({x, y}, i);
//				PathFinding(nextCell.x, nextCell.y, mut, pas + 1);
//			}
//		}
//	}

}
