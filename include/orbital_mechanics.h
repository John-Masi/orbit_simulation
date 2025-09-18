#include <cassert>
#include <math.h>
#include <iostream>
#include <limits>

#ifndef orbital_mechanics
#define orbital_mechanics

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

// Our rotation matrices to compute for our real r and v 
class Matrice {
	public:
		double m[3][3] {};

		static Matrice rotation_I(double inclination) {
			double new_inc = inclination * (M_PI/180);
			double a = std::cos(new_inc);
			double b = std::sin(new_inc);
			Matrice r;
			r.m[0][0] = 1.0;
			r.m[0][1] = 0.0;
			r.m[0][2] = 0.0;

			r.m[1][0] = 0.0;
			r.m[1][1] = a;
			r.m[1][2] = (-1.0) * b;

			r.m[2][0] = 0.0;
			r.m[2][1] = b;
			r.m[2][2] = a;
	
			return r;
		};

		static Matrice rotation_R(double perapsis) {
			double a = std::cos(perapsis);
			double b = std::sin(perapsis);
			Matrice r;

			r.m[0][0] = a;
			r.m[0][1] = (-1.0) * b;
			r.m[0][2] = 0.0;

			r.m[1][0] = b;
			r.m[1][1] = a;
			r.m[1][2] = 0.0;

			r.m[2][0] = 0.0;
			r.m[2][1] = 0.0;
			r.m[2][2] = 1.0;

			return r;
		};

		static Matrice rotation_O(double omega) {
			double a = std::cos(omega);
			double b = std::sin(omega);
			Matrice r;

			r.m[0][0] = a;
			r.m[0][1] = (-1.0) * b;
			r.m[0][2] = 0.0;

			r.m[1][0] = b;
			r.m[1][1] = a;
			r.m[1][2] = 0.0;

			r.m[2][0] = 0.0;
			r.m[2][1] = 0.0;
			r.m[2][2] = 1.0;

			return r;
		};

		Matrice operator*(const Matrice &mx) const {
			Matrice result;

			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 3; j++) {
					result.m[i][j] = 0.0;
					for(int k = 0; k < 3; k++){
						result.m[i][j] += m[i][k] * mx.m[k][j];
					}
				}
			}

			return result;
		}

		void Print() {
			for(int i = 0; i < 3; i++) {
				for(int k = 0; k < 3; k++) {
					std::cout << m[i][k] << '\n';
				}
			}
		}

	private:
};

// State vectors for r and v 
class VectorRV {
	public:
		double v[3];

		void m_update(double j) {
			for(int i = 0; i < 3; i++){
				v[i] = v[i] * j;
			}
		}

		double magnitude() {
			double m;
			for(int i = 0; i < 3; i++){
				m += v[i] * v[i];
			}
			m = std::sqrt(m);

			return m;
		}

		VectorRV mul_create(double j){
			VectorRV result;
			for(int i = 0; i < 3; i++){
				result.v[i] = v[i] * j;
			}

			return result;
		}

		// Multiplying by a 3x3 matrice for inital state vectors 
		VectorRV mul_3x3(const Matrice& m){
			VectorRV result;
			for(int i = 0; i < 3; i++){
				result.v[i] = 0.0;
				for(int j = 0; j < 3; j++){
					result.v[i] += m.m[i][j] * v[j];
				}
			}

			return result;
		}

		// TODO: Add operator+ for reduce reduncy 
		// Adding 1x1 vectors to each other
		VectorRV add_1x1(VectorRV rv){
			VectorRV result;
			for(int j = 0; j < 3; j++){
					result.v[j] = rv.v[j] + v[j];
				}

			return result; 
		}

		// half step required for calculating mid points in rk4 integration
		VectorRV half_step(VectorRV& rv,double dt) {
			VectorRV result;

			for(int i = 0; i < 3; i++){
				result.v[i] = rv.v[i] * 0.5 * dt;
			}

			return result;
		}

		void Print(){
			for(int i = 0; i < 3; i++){
				std::cout << v[i] << '\n';
			}
		}

	private:
};

class SatelliteOrbit{
	public:
		SatelliteOrbit() = default;
		SatelliteOrbit(double mass,OrbitalParameters o) :orbited_p(mass), op(o){
			orbited_p = {mass};
			op = o;
			p = op.major_axis* (1.0 - (op.eccentrity * op.eccentrity));
			r = (p / (1.0 + op.eccentrity) * std::cos(op.v_anomaly));
			orbited_p.u = std::round(orbited_p.u);

			// Calculating velocity components 
			V_p = (-1.0) * (std::sqrt(orbited_p.u/p) * std::sin(op.v_anomaly));
			V_q =  std::sqrt(orbited_p.u/p) * (op.eccentrity + std::cos(op.v_anomaly));

			// Setting the perofical vecloity vector 
			v_v.v[0] = V_p;
			v_v.v[1] = V_q;
			v_v.v[2] = 0.0;

			// Setting the perofical position vector 
			v_r.v[0] = r * std::cos(op.v_anomaly);
			v_r.v[1] = r * std::sin(op.v_anomaly);
			v_r.v[2] = 0.0; 

			// Rotating Matrices
			m_i = Matrice::rotation_I(op.inc);
			m_o = Matrice::rotation_O(op.omega);
			m_r = Matrice::rotation_R(op.perapsis);
		
		};

		VectorRV rotating_vec(const Matrice& m,VectorRV rv) {
			VectorRV rotated_Vec = rv.mul_3x3(m);

			return rotated_Vec;
		}

		Matrice rotation_Matrices() {
			Matrice r1 = m_i * m_r;
			Matrice r2 = r1 * m_o;

			return r2;
		}

		void rotate(VectorRV rev,VectorRV rev2) {
			// Turn Rotation in a struct?
			Matrice m = rotation_Matrices();
			rev = rotating_vec(m,rev);
			rev2 = rotating_vec(m,rev2);
		}

		void RK4_Integration(double dt, VectorRV& pos, VectorRV& vel) {
			// Turn Kvalues into structs


   		 	// Stage 1
    		VectorRV k1_r = vel;               
    		VectorRV k1_v = calculateAcceleration(pos); 

    		// Stage 2
   			VectorRV r2 = pos.add_1x1(k1_r.mul_create(0.5 * dt));
    		VectorRV v2 = vel.add_1x1(k1_v.mul_create(0.5 * dt));
    		VectorRV k2_r = v2;
    		VectorRV k2_v = calculateAcceleration(r2);

    		// Stage 3
    		VectorRV r3 = pos.add_1x1(k2_r.mul_create(0.5 * dt));
   		 	VectorRV v3 = vel.add_1x1(k2_v.mul_create(0.5 * dt));
   			VectorRV k3_r = v3;
    		VectorRV k3_v = calculateAcceleration(r3);

    		// Stage 4
   	 		VectorRV r4 = pos.add_1x1(k3_r.mul_create(dt));
    		VectorRV v4 = vel.add_1x1(k3_v.mul_create(dt));
    		VectorRV k4_r = v4;
    		VectorRV k4_v = calculateAcceleration(r4);

   			// Final stage 
    		VectorRV delta_r = k1_r
        		.add_1x1(k2_r.mul_create(2.0)).add_1x1(k3_r.mul_create(2.0)).add_1x1(k4_r);
    		delta_r.m_update(dt / 6.0);

    		VectorRV delta_v = k1_v
        		.add_1x1(k2_v.mul_create(2.0)).add_1x1(k3_v.mul_create(2.0)).add_1x1(k4_v);
    		delta_v.m_update(dt / 6.0);

    		pos = pos.add_1x1(delta_r);
    		vel = vel.add_1x1(delta_v);

    		Final_r = pos;
    		Final_v = vel;
		}

		VectorRV calculateAcceleration(VectorRV& r) {
			VectorRV accel = r.mul_create(-orbited_p.u /(r.magnitude() * r.magnitude() * r.magnitude()));

			return accel;
		}

		VectorRV getFinal_r() { return Final_r; }
		VectorRV getFinal_v() { return Final_v; }

		OrbitalParameters op;
		Planet orbited_p;

		VectorRV v_r;
		VectorRV v_v;

		VectorRV k_v;	
		VectorRV k_r;

		~SatelliteOrbit() = default; 
		
	private:

		VectorRV Final_v;
		VectorRV Final_r;

		Matrice m_i;
		Matrice m_o;
		Matrice m_r;

		double p;
		double r;

		double V_p;
		double V_q;
	
};

#endif 
