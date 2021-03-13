#pragma once

#include "src/coord.h"

class Screen2D {
public:

	// Game root surface size (E.g. 800x600).
	// Can be stretched/upscaled to higher resolutions.
	static Coord RootSize;

    static Coord StretchSize;

	static Coord GetGameHexFromScreenCoords(const Coord& screen_coords);
    static bool CanMapScreenCoordsToMap(const Coord& screen_coords);
};
