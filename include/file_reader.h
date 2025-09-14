#include <fstream>
#include <iostream>
#include <string>
#include "orbital_mechanics.h"
#include "telemetry.h"

#ifndef file_reader
#define file_reader

static std::string file_path = "../orbit.csv";
static std::string telemetry_path = "../orbit.bin";


// Converts 
void addTelemetry(const OrbitData& od) {
	std::ofstream file(telemetry_path, std::ios::binary | std::ios::app);
	file.write(reinterpret_cast<const char *>(&od),sizeof(od));

}

void addData(const OrbitData& od) {
	std::ofstream file(file_path, std::ios::app);

	static bool header{false};

	if(!header) {
		file << "t, rx, ry, rz, vx, vy, vz\n";
		header = true;
	}

	file << od.t_step << ", " << od.rx << ", " << od.ry << ", "
	<< od.rz << ", " << od.vx << ", " << od.vy << ", " << od.vz << "\n";

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