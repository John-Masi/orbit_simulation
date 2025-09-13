# orbit_simulation
A C++ project for simulating orbital motion using numerical integration methods. The simulator models a satellite in Earth orbit and outputs trajectory data for analysis and visualization.
--------------------------------------------------------------------
Features:
Runge-Kutta 4 integrator for orbital motion ,
Outputs position and velocity for every timestep ,
Loggs the position and velocity ,
3D Visualization using the logged data in python , 
---------------------------------------------------------------------
Get started:
clone the repo,
cd orbit_simulation ,
mkdir build,
cd build,
cmake .. , 
cmake --build . ,
./orbit_simulation
----------------------------------------------------------------------
Draw the orbit:
poetry run python main.py
----------------------------------------------------------------------
<img width="800" height="660" alt="Figure_1" src="https://github.com/user-attachments/assets/e9181369-7197-4d06-b3ae-09393b6cecc6" />
