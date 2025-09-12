#pragma once 
#include "orbital_mechanics.h"

#pragma

#ifndef constants
#define constants 

namespace g_constants {
	inline constexpr double earth = 398600.0;
	inline constexpr double mars = 4.282e13;
	inline constexpr double mercury = 2.203e13;
	inline constexpr double venus = 3.248e14;
	inline constexpr double sun = 1.327e20;
	inline constexpr double moon = 4.92e12;
	inline constexpr double jupiter = 1.266e17;
	inline constexpr double saturn = 3.793e16;
	inline constexpr double uranus = 5.793e15;
	inline constexpr double neptune = 6.836e16;
	inline constexpr double pluto = 8.71e11;

};

namespace orbits {

	inline constexpr OrbitalParameters loe = {6778.0,0.001,51.6,0.0,0.0,0.0,10.0};
	inline constexpr OrbitalParameters molniya = {26560.0,0.72,63.4,270.0,0.0,0.0,10.0};
	inline constexpr OrbitalParameters polar_circut = {7000,0.001,90.0,0.0,0.0,270.0,10.0};
}

#endif 