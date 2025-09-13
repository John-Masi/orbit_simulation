#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>
#include <chrono>
#include "orbital_mechanics.h"
#include "file_reader.h"
#include "constants.h"
#include <map>
#include <iomanip>

// Our queue to run said physics operations
// TODO MAKE THIS A CLASS
class Operation{
	public:
		Operation(){

		}
		~Operation() = default;
	private:

};

namespace operation_queue{
	using sample_size = int;
	std::queue<int> q;
	std::condition_variable cv;
	std::mutex mtx;
	sample_size s = 5;
	int stop = -1;
	// NOTE: Stop will be different for each operation

	int pop_p() {
		std::unique_lock<std::mutex> l(mtx);
		// Thread is blocked while we wait to check if the queue is empty 
		cv.wait(l,[&]{ return !q.empty(); });
		int v = std::move(q.front());
		q.pop();
		return v;
	}

	void push(int v) {
		{
			q.push(v);
		}
		// Block thread
		cv.notify_one();
	}

	void consumer() {
		while(true) {
			int h = pop_p();
			if(h == stop){
				break;
			}
			// Run simulation of what ever physicis calulation we do
			// h.run(sample_size)
		}
	}

	void producer() {
		for(int i = 0; i < 5; i++) {
			int v = i;
			q.push(v);
		}

		// Implement different stops for different physics types
		q.push(stop);
	}
};

// Clean this up for queue
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

		OrbitData od = {so.getFinal_r(), so.getFinal_v(),static_cast<double>(i)};
		addData(od);
	}
}

int main() {
	std::map<OrbitalParameters, int> o_map;

	std::thread main_t(orbitSimulation, orbits::loe);
	main_t.join();

	return 0;
}