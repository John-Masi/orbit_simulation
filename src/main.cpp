#include <utility>
#include "orbital_mechanics.h"
#include "file_reader.h"
#include "constants.h"
#include "telemetry.h"
#include <map>
#include <iomanip>

void orbitSimulation(OrbitalParameters op) {

	SatelliteOrbit so(5.972e24,op);

	ClearData();

	VectorRV update_r = so.v_r;
	VectorRV update_v = so.v_v;

	so.rotate(update_r,update_v);

	for(int i = 1; i <= so.op.period; i++){
		so.RK4_Integration(10.0,update_r,update_v);

		update_r = so.getFinal_r();
		update_v = so.getFinal_v();

		OrbitData od = {so.getFinal_v().v[0],so.getFinal_v().v[1],so.getFinal_v().v[2],
					so.getFinal_r().v[0],so.getFinal_r().v[1],so.getFinal_r().v[2],static_cast<double>(i)};
		addData(od);
		//addTelemetry(od);
	}
}

int main() {
	std::map<OrbitalParameters, int> o_map;

	std::thread main_t(orbitSimulation, orbits::molniya);
	main_t.join();

	return 0;
}
