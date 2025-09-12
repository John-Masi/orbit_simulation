import matplotlib
import matplotlib.pyplot as plt 
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.animation as animation
from matplotlib import image
from skimage.transform import resize 
matplotlib.use("Qt5Agg")
import numpy as np
import pandas as pd

path = "../orbit.csv"
earth_img = "../earth.jpg"
img = image.imread(earth_img)
earth = resize(img, (64,128))
file = pd.read_csv(path)

headers = ["t", " rx", " ry"," rz"," vx"," vy"," vz"]
rows = len(file)

fig = plt.figure(figsize=(8,8))
ax = fig.add_subplot(111,projection='3d')
ax.set_proj_type('ortho')
ax.view_init(elev=10, azim=50)
ax.set_box_aspect([1,1,1])

ax.set_xlabel("X (rx)")
ax.set_ylabel("Y (ry)")
ax.set_zlabel("Z (rz)")

#x = np.concatenate([earth_x.ravel(),file[headers[1]].iloc[0]])
#y = np.concatenate([earth_y.ravel(),file[headers[2]].iloc[0]])
#z = np.concatenate([earth_z.ravel(),file[headers[3]].iloc[0]])

ax.set_xlim(14000)
ax.set_zlim(14000)
ax.set_ylim(14000)

line, = ax.plot3D([], [], [], lw=1)
point, = ax.plot3D([], [], [], marker='H',ms=20,mec='black',color='red',ls='--')

def init():
    line.set_data([], [])
    line.set_3d_properties([])
    point.set_data([], [])
    point.set_3d_properties([])
    return point, line

def single_animate(frame):
	point.set_data([file[headers[1]].iloc[frame]],[file[headers[2]].iloc[frame]])
	point.set_3d_properties([file[headers[3]].iloc[frame]])

	return point,

def full_animate(frame):
	#line.set_data(file[headers[1]].iloc[:frame],file[headers[2]].iloc[:frame])
	#line.set_3d_properties(file[headers[3]].iloc[:frame])

	point.set_data(file[headers[1]].iloc[:frame],file[headers[2]].iloc[:frame])
	point.set_3d_properties(file[headers[3]].iloc[:frame])

	return point, line

ani = animation.FuncAnimation(fig = fig, func=single_animate,frames=rows - 1, interval=100,init_func=init)

earth = earth / 255.0 if earth.max() > 1 else earth
n_lon, n_lat = earth.shape[1], earth.shape[0]
u = np.linspace(0,2*np.pi,n_lon)
v = np.linspace(0, np.pi,n_lat)
u , v = np.meshgrid(u, v)
earth_x = 3000 * np.cos(u) * np.sin(v)
earth_y = 3000 * np.sin(u) * np.sin(v)
earth_z = 3000 * np.cos(v)
ax.plot_surface(earth_x, earth_y, earth_z, facecolors=earth, linewidth=0)

plt.show()