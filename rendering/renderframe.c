#include "../vx_header.h"
#include <sys/ioctl.h>

/*
colours:
black:	0
red:	1
green:	2
yellow:	3
blue:	4
magenta:5
cyan:	6
white:	7
*/

char brightnessencode[] = " -*aH$@#";

int printpix(char col, int brightness, char* frame, int* loc) {
	if (brightness >= 16 || brightness < 0) {
		return -1;
	}
	char printstring[] = "\033[1;30m\033[1;40m_";
	printstring[5 + 7 * (brightness >= 8)] = col + 48;
	if (brightness >= 8) brightness = 15 - brightness;
	printstring[14] = brightnessencode[brightness];
	for (int k = 0; k < 15; k++) frame[15 * (*loc) + k] = printstring[k];
	(*loc)++;
	return 0;
}

int rendersky(double pos[3], double dir[3], char* frame, int* loc) {
	printpix(4, 14, frame, loc);
	return 0;
}

int renderframe(char world[2 * renderdist][worldheight][2 * renderdist], double pos[8]) {
	struct winsize s;
	ioctl(0, TIOCGWINSZ, &s);
	int ws[2] = {s.ws_row, s.ws_col};
	char frame[ws[0] * (ws[1] + 1) * 18 + 1];
	int K = 0;
	for (int k = 0; k < ws[0] * (ws[1] + 1) * 18; k++) frame[k] = 0;
	double up[3] = {0, 1, 0};
	double dir0[3];
	double xdir[3];
	double ydir[3];
	dir0[0] = cos(pos[3]) * sin(pos[4]);
	dir0[1] = cos(pos[4]);
	dir0[2] = sin(pos[3]) * sin(pos[4]);
	printf("\033[1;0m\n\rPosition: (%f, %f, %f)", pos[0], pos[1], pos[2]);
	cross(dir0, up, xdir);
	cross(dir0, xdir, ydir);
	normalize(xdir);
	normalize(ydir);
	int worldoffset[2] = {((int) (pos[0] + 500 * chunksize - renderdist)) / chunksize - 500, ((int) (pos[2] + 500 * chunksize - renderdist)) / chunksize - 500};
	double pos0[3] = {pos[0] - worldoffset[0] * chunksize, pos[1], pos[2] - worldoffset[1] * chunksize};
	for (int y = 0; y < ws[0]; y++) {
		frame[15*K] = '\n';
		for (int k = 1; k < 15; k++) frame[15*K + k] = '\r';
		K++;
		double yoffset[3];
		for (int i = 0; i < 3; i++) yoffset[i] = 4 * ydir[i] * ((double) y / ws[1] - 0.5 * ws[0] / ws[1]);
		for (int x = 0; x < ws[1]; x++) {
			double dir[3] = {dir0[0], dir0[1], dir0[2]};
			double xoffset[3];
			for (int i = 0; i < 3; i++) xoffset[i] = 2 * xdir[i] * ((double) x / ws[1] - 0.5);
			for (int i = 0; i < 3; i++) dir[i] += xoffset[i] + yoffset[i];
			normalize(dir);
			double hitblock[3] = {-1};
			raytrace(world, pos0, dir, renderdist, hitblock);
			if (hitblock[0] == -1) {
				rendersky(pos, dir, frame, &K);
			} else {
				int normal = 0;
				for (int i = 0; i < 3; i++) {
					double dfc = modd(hitblock[i], 1.0) - 0.5;
					if (dfc > 0) dfc *= -1;
					dfc += 0.5;
					if (dfc < 0.01) {
						normal = i + 1;
						if (dir[i] > 0) normal *= -1;
						break;
					}
				}
				int block[3];
				for (int i = 0; i < 3; i++) block[i] = (int) hitblock[i];
				double sundir[3] = {0.5, 0.6, 0.3};
				double sunhitpos[3] = {-1};
				for (int i = 0; i < 3; i++) hitblock[i] -= 0.001 * dir[i];
				if (world[block[0]][block[1]][block[2]] == 37) for (int i = 0; i < 3; i++) hitblock[i] += 0.3 * sundir[i];
				raytrace(world, hitblock, sundir, renderdist, sunhitpos);
				int sunhit = 2 - (sunhitpos[0] == -1);
				switch (world[block[0]][block[1]][block[2]]) {
				case 33: printpix(7, (8 + normal) / sunhit, frame, &K); break;
				case 34: printpix(2, (5 + normal / 2) / sunhit, frame, &K); break;
				case 35: printpix(1, (10 + normal) / sunhit, frame, &K); break;
					case 36: {
						int col[2] = {7, 9};
						if (modd(hitblock[0] + hitblock[1] + hitblock[2], 0.7) < 0.2 && modd(hitblock[0] - hitblock[1] - 0.5 * hitblock[2], 0.6) < 0.2) {
							col[0] = 3;
							col[1] = 11;
						}
						if (modd(hitblock[0] + hitblock[1] + hitblock[2] + 0.4, 0.7) < 0.2 && modd(hitblock[0] - 0.2 * hitblock[1] - 1.2 * hitblock[2] + 0.3, 0.6) < 0.2) {
							col[0] = 1;
							col[1] = 7;
						}
						printpix(col[0], (col[1] + normal) / sunhit, frame, &K);
						break;
					}
				case 37: printpix(2, (3 + normal / 2) / sunhit, frame, &K); break;
				case 48: printpix(0, (12 + normal) / sunhit, frame, &K); break;
				case 49: printpix(1, (12 + normal) / sunhit, frame, &K); break;
				case 50: printpix(2, (12 + normal) / sunhit, frame, &K); break;
				case 51: printpix(3, (12 + normal) / sunhit, frame, &K); break;
				case 52: printpix(4, (12 + normal) / sunhit, frame, &K); break;
				case 53: printpix(5, (12 + normal) / sunhit, frame, &K); break;
				case 54: printpix(6, (12 + normal) / sunhit, frame, &K); break;
				case 55: printpix(7, (12 + normal) / sunhit, frame, &K); break;
				default: printpix(5, 15, frame, &K); break;
				}
			}
		}
	}
	frame[ws[0] * (ws[1] + 1) * 18] = 0;
	frame[ws[0] * (ws[1] + 1) * 18 - 1] = '\r';
	printf(frame);
	return 0;
}
