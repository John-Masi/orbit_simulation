#ifndef CONSTANTS_H
#define CONSTANTS_H

static const double G = 6.6743e-20;

struct Planet{
	double mass_;	
	double u;

	Planet(double m) : mass_(m){
		u = G * mass_; 
	}
};

struct OrbitalParameters{
	double major_axis;
	double eccentrity;
	double inc;
	double omega;
	double perapsis;
	double v_anomaly;
	double dt;
	int period = static_cast<int>(((2* M_PI) * std::sqrt((major_axis * major_axis * major_axis) / 398600.4418)) / dt);
};


namespace g_constants {
	 constexpr double earth = 398600.0;
	 constexpr double mars = 4.282e13;
	 constexpr double mercury = 2.203e13;
	 constexpr double venus = 3.248e14;
	 constexpr double sun = 1.327e20;
	 constexpr double moon = 4.92e12;
	 constexpr double jupiter = 1.266e17;
	 constexpr double saturn = 3.793e16;
	 constexpr double uranus = 5.793e15;
	 constexpr double neptune = 6.836e16;
	 constexpr double pluto = 8.71e11;

};

namespace orbits {
	 constexpr OrbitalParameters loe = {6778.0,0.001,51.6,0.0,0.0,0.0,10.0};
	 constexpr OrbitalParameters molniya = {26560.0,0.72,63.4,270.0,0.0,0.0,10.0};
	 constexpr OrbitalParameters polar_circut = {7000,0.001,90.0,0.0,0.0,270.0,10.0};
}

#endif 
