#pragma once 

#pragma pack(push, 1)
struct OrbitData{
	double vx ,vy, vz;
	double rx, ry, rz;
	double t_step;
};
#pragma pack(pop)