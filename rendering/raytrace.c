#include "../vx_header.h"
int raytrace(char world[2 * renderdist][worldheight][2 * renderdist], double pos0[3], double dir[3], double dist, double hitpos[3]) {
	double start[3];
	start[0] = pos0[0];
	start[1] = pos0[1];
	start[2] = pos0[2];
	double distmult = sqrt(dot(dir, dir));
	double stp[3];
	double len[3];
	
	for (int i = 0; i < 3; i++) {
		dir[i] /= distmult;
		if (dir[i] != 0) stp[i] = 1 / dir[i];
		else stp[i] = 100;
		if (stp[i] < 0) stp[i] *= -1;
		if (dir[i] < 0) len[i] = 0;
		else len[i] = 1;
		len[i] = (len[i] - modd(start[i], 1.0)) / dir[i];
	}
	len[0] -= stp[0];
	double d = 0;
	int found = 0;
	for (int k = 0; k < 4 * dist && d < dist && !found; k++) {
		double pos[3];
		int blockpos[3];
		for (int i = 0; i < 3; i++) {
			pos[i] = start[i] + dir[i] * (d + 0.001);
			blockpos[i] = (int) pos[i];
		}
		if (pos[1] < 0 || pos[1] > worldheight || min(pos[0], pos[2]) < 0 || max(pos[0], pos[2]) >= 2 * renderdist) break;
		
		if (world[blockpos[0]][blockpos[1]][blockpos[2]] != 32 && world[blockpos[0]][blockpos[1]][blockpos[2]] != 0) {
			found = 1;
			for (int i = 0; i < 3; i++) hitpos[i] = pos[i];
			break;
		}
		double minlen = len[0];
		for (int i = 1; i < 3; i++) if (len[i] < minlen) minlen = len[i];
		for (int i = 0; i < 3; i++) if (len[i] == minlen) {
			len[i] += stp[i];
			break;
		}
		d = len[0];
		for (int i = 1; i < 3; i++) if (len[i] < d) d = len[i];
		if (d < 0) d = 0;
	}
//	dist *= distmult;
	return 0;
}
