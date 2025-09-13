#include <fstream>
#include <iostream>
#include <string>
#include "orbital_mechanics.h"

#ifndef file_reader
#define file_reader

struct OrbitData{
	VectorRV v;
	VectorRV r;
	double t_step;
};

static std::string file_path = "../orbit.csv";

void addData(const OrbitData& od) {
	std::ofstream file(file_path, std::ios::app);

	static bool header{false};

	if(!header) {
		file << "t, rx, ry, rz, vx, vy, vz\n";
		header = true;
	}

	file << od.t_step << ", " << od.v.v[0] << ", " << od.v.v[1] << ", "
	<< od.v.v[2] << ", " << od.r.v[0] << ", " << od.r.v[1] << ", " << od.r.v[2] << "\n";

	file.close();

}

int ClearData() {
	std::ofstream file(file_path,std::ios::out | std::ios::trunc);

	if (file.is_open()){
		return 1;
	}

	file.close();
	return 0;
}

#endif