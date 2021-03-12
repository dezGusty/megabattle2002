#pragma once

#include "src/coord.h"

class Screen2D {
public:
	static Coord GetGameHexFromScreenCoords(const Coord& screen_coords);
    static bool CanMapScreenCoordsToMap(const Coord& screen_coords);
};
