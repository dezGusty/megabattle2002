#include "game.h"

#include <numeric>
#include <vector>

#include "src/thirdparty/mini/ini.h"

Game::Game(): hero_factory_(nullptr) {

}

Game::~Game() {

}

void Game::AssignHeroFactory(HeroFactory* hero_factory){
	this->hero_factory_ = hero_factory;
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
		if (sourceCell.y % 2 == 0) {
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
	for (auto count = 0; count < hero.army_slots.size(); ++count) {
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

void Game::ResetSelectionMatrix() {
	for (int i = 0; i < MAP_WIDTH; i++) {
		for (int j = 0; j < MAP_HEIGHT; j++) {
			selected[i][j] = 0;
		}
	}
}


// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
std::vector<Coord> Game::SearchPathOnMap(Coord source, Coord target, int max_moves, bool& path_found) {
	std::vector<Coord> result;

	this->SearchPathOnCachedMap(source, target, source, max_moves, 0, path_found, result);
	std::reverse(begin(result),end(result));
	return result;
}

void Game::SearchPathOnCachedMap(Coord source, Coord target, Coord current_pos, int max_moves, int step, bool& path_found, std::vector<Coord> &path) {

	if (path_found) {
		return;
	}

	if (step > max_moves) {
		return;
	}

	// default directions (and order in which to try them)
	std::vector<int>directions_to_try = {TOPLEFT, LEFT, BOTLEFT, TOPRIGHT, RIGHT, BOTRIGHT};

	if (target.x == current_pos.x && target.y == current_pos.y) {
        path.clear();
		path.emplace_back(current_pos);
		path_found = true;
		return;
	}

	// if the terrain is occupied, you can't use it.
	if (this->teren[current_pos.x][current_pos.y] != 0) {
		// unless you are the one occupying it
		if (current_pos.x != source.x || current_pos.y != source.y) {
			return;
		}
	}

	// Check how far we are from each target (x and y).
	// The more sizeable difference can be used with priority.
	int diff_x = abs(current_pos.x - target.x);
	int diff_y = abs(current_pos.y - target.y);

	// prioritize going up
	if (current_pos.y > target.y) {
		// prioritize going up/left
		if (current_pos.x > target.x) {
			if (diff_x > diff_y) {
				directions_to_try = {
					TOPLEFT, LEFT, BOTLEFT, TOPRIGHT, RIGHT, BOTRIGHT};
			}
			else {
				directions_to_try = {
					TOPLEFT, TOPRIGHT, LEFT, RIGHT, BOTLEFT, BOTRIGHT};
			}
		}
		// prioritize going up/right
		if (current_pos.x < target.x) {
			if (diff_x > diff_y) {
				directions_to_try = {
					TOPRIGHT, RIGHT, BOTRIGHT, TOPLEFT, LEFT, BOTLEFT};
			}
			else {
				directions_to_try = {
					TOPRIGHT, TOPLEFT, RIGHT, LEFT, BOTRIGHT, BOTLEFT};
			}
		}
		// prioritize going up
		if (current_pos.x == target.x) {
			if (current_pos.y % 2 == 0) {
				// TOPRIGHT
				directions_to_try = {
					TOPRIGHT, RIGHT, BOTRIGHT, TOPLEFT, LEFT, BOTLEFT};
			}
			else {
				// TOPLEFT
				directions_to_try = {
					TOPLEFT, LEFT, BOTLEFT, TOPRIGHT, RIGHT, BOTRIGHT};
			}
		}
	}

	// prioritize left/right
	if (current_pos.y == target.y) {
		// prioritize left
		if (current_pos.x > target.x) {
			directions_to_try = {
				LEFT, TOPLEFT, BOTLEFT, RIGHT, BOTRIGHT, TOPRIGHT};
		}

		// prioritize right
		if (current_pos.x < target.x) {
			directions_to_try = {
				RIGHT, TOPRIGHT, BOTRIGHT, LEFT, BOTLEFT, TOPLEFT};

		}
	}

	// prioritize down
	if (current_pos.y < target.y) {
		// prioritize down/left
		if (current_pos.x > target.x) {
			if (diff_y >= diff_x) {
				directions_to_try = {
					BOTLEFT, BOTRIGHT, LEFT, RIGHT, TOPLEFT, TOPRIGHT};
			}
			else {
				directions_to_try = {
					BOTLEFT, LEFT, TOPLEFT, BOTRIGHT, RIGHT, TOPRIGHT};

			}
		}

		// prior drp
		if (current_pos.x <= target.x) {
			// prioritize bottom over right
			if (diff_y >= diff_x) {
				directions_to_try = {
					BOTRIGHT, BOTLEFT, RIGHT, LEFT, TOPRIGHT, TOPLEFT};
			}
			else {
				directions_to_try = {
					BOTRIGHT, RIGHT, TOPRIGHT, BOTLEFT, LEFT, TOPLEFT};

			}
		}
	}

	for (int direction : directions_to_try) {
		if (this->DoesNeighbourExist(current_pos, direction)) {
			Coord nextCell = this->GetNeighbourCell(current_pos, direction);
			this->SearchPathOnCachedMap(source, target, nextCell, max_moves, step + 1,
				path_found, path);
		}
	}

   	path.emplace_back(current_pos);
}

void Game::MarkSelectionOnCachedMap(Coord target, int max_moves, int selected_player_index) {
	MarkSelectionOnCachedMap(target, max_moves, -1, selected_player_index);
}

/**
	Updates the (cached) selection map, and propagates from the given position.

	@param pos Position to mark. Can propagate from it in all directions.
	@param max_moves How far can the move propagate
	@param from_direction A direction from which this call was expanded.
			Will not propagate in that direction.
	@param selected_player_index The selected player for which the selection is created.
*/
void Game::MarkSelectionOnCachedMap(Coord pos, int max_moves, int from_direction, int selected_player_index) {
	if (max_moves <= 0) {
		return;
	}

	for (int direction = TOPLEFT; direction <= TOPRIGHT; direction++) {
		if (from_direction >=0 && direction == (from_direction + 3) % 6) {
			// skip processing
			continue;
		}

		if (false == this->DoesNeighbourExist(pos, direction)) {
			continue;
		}

		Coord hex_cell = this->GetNeighbourCell(pos, direction);
		if (this->teren[hex_cell.x][hex_cell.y]) {
			if (this->teren[hex_cell.x][hex_cell.y] / 20 != selected_player_index) {
				this->selected[hex_cell.x][hex_cell.y] = 2;
			}
			else {
				this->selected[hex_cell.x][hex_cell.y] = 0;
			}
		}
		else {
			this->selected[hex_cell.x][hex_cell.y] = 1;
			MarkSelectionOnCachedMap(hex_cell, max_moves - 1, direction, selected_player_index);
		}
	}
}

std::vector<Coord> Game::GetSelectedCellsOnCachedMap() {
	std::vector<Coord>results;
	for (int j = 0; j < MAP_HEIGHT; j++) {
		for (int i = 0; i < MAP_WIDTH; i++) {
			if (selected[i][j]) {
				results.emplace_back(Coord {i, j});
			}
		}
	}

	return results;
}

/**
	Checks the victory status for the game.
	Typically, when one of the players has all units destroyed => the other wins.

	@return 0: Nobody won yet; the battle is still fought
			1: The first player won (battle finished).
			2: The second player won (battle finished).
*/
int Game::GetVictoryStatus() {
	if (this->players_.size() <= 1) {
		return 1;
	}

	auto count_alive = [](int sum, Soldier* soldier){
        return sum + soldier->alive;
	};

    // count the amount of units alive
	int alive_count_0 = std::accumulate(
		players_[0]->army_slots.begin(),
		players_[0]->army_slots.end(), 0, count_alive);

	int alive_count_1 = std::accumulate(
		players_[1]->army_slots.begin(),
		players_[1]->army_slots.end(), 0, count_alive);

	if (0 == alive_count_0 && 0 == alive_count_1) {
		// both players have 0 units.
		// how is this even possible?
		// some spell or delayed ability that can kill both attacking and the defending unit?
		// for now, just return the first player as a winner.
		return 1;
	}

	if (0 == alive_count_0) {
		return 2;
	}

	if (0 == alive_count_1) {
		return 1;
	}

	// default: no player won.
	return 0;
}
