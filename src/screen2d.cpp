#include "screen2d.h"

Coord Screen2D::RootSize = Coord {800, 600};
Coord Screen2D::StretchSize = Coord {800, 600};

Coord Screen2D::GetGameHexFromScreenCoords(const Coord& screen_coords){
	Coord result;

	result.x = screen_coords.x - 20;
	result.y = screen_coords.y - 110;
	result.y /= 60;
	if (result.y % 2 == 1) {
		result.x -= 40;
	}

	result.x /= 80;
	result.x = result.x;

	return result;
}


// ---------------------------------------------------------------------------
bool Screen2D::CanMapScreenCoordsToMap(const Coord& screen_coords) {
	if (screen_coords.x <= 20 || screen_coords.x >= 780 || screen_coords.y <= 110 || screen_coords.y >= 530) {
		return false;
	}

	int fx = screen_coords.x - 20;
	int fy = screen_coords.y - 110;
	fy /= 60;
	if ((fy % 2 == 1 && screen_coords.x < 60) || (fy % 2 == 0 && screen_coords.x > 740)) {
		return false;
	}

	return true;
}
