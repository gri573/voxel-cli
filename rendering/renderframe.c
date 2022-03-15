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

int printpix(char col, char brightness) {
	char printstring[] = "\033[1;30m\033[1;40m_";
	printstring[5 + 7 * (brightness >= 8)] = col + 48;
	if (brightness >= 8) brightness = 15 - brightness;
	printstring[14] = brightnessencode[brightness];
	printf("%s", printstring);
	return 0;
}

int rendersky(double pos[3], double dir[3]) {
	printpix(4, 14);
	return 0;
}

int renderframe(char world[2 * renderdist][worldheight][2 * renderdist], double pos[8]) {
	struct winsize s;
	ioctl(0, TIOCGWINSZ, &s);
	int ws[2] = {s.ws_row - 5, s.ws_col};
	double up[3] = {0, 1, 0};
	double dir0[3];
	double xdir[3];
	double ydir[3];
	dir0[0] = cos(pos[3]) * sin(pos[4]);
	dir0[1] = cos(pos[4]);
	dir0[2] = sin(pos[3]) * sin(pos[4]);
	printf("Look direction: (%f, %f, %f) with length %f", dir0[0], dir0[1], dir0[2], sqrt(dot(dir0, dir0)));
	cross(dir0, up, xdir);
	cross(dir0, xdir, ydir);
	normalize(xdir);
	normalize(ydir);
	int worldoffset[2] = {((int) pos[0] - renderdist) / chunksize, ((int) pos[2] - renderdist) / chunksize};
	double pos0[3] = {pos[0] - worldoffset[0] * chunksize, pos[1], pos[2] - worldoffset[1] * chunksize};
	for (int y = 0; y < ws[0]; y++) {
		printf("\n\r");
		double yoffset[3];
		for (int i = 0; i < 3; i++) yoffset[i] = 4 * ydir[i] * ((double) y / ws[1] - 0.5 * ws[0] / ws[1]);
		if (y < 10) printf("\033[1;0m %d", y);
		else printf("\033[1;0m%d", y);
		for (int x = 2; x < ws[1]; x++) {
			double dir[3] = {dir0[0], dir0[1], dir0[2]};
			double xoffset[3];
			for (int i = 0; i < 3; i++) xoffset[i] = 2 * xdir[i] * ((double) x / ws[1] - 0.5);
			for (int i = 0; i < 3; i++) dir[i] += xoffset[i] + yoffset[i];
			normalize(dir);
			double hitblock[3] = {-1};
			raytrace(world, pos0, dir, renderdist, hitblock);
			if (hitblock[0] == -1) {
				rendersky(pos, dir);
			} else {
				int normal;
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
				switch (world[block[0]][block[1]][block[2]]) {
					case 33: printpix(7, 8 + normal); break;
					case 34: printpix(2, 7 + normal); break;
					case 35: printpix(1, 10 + normal); break;
					default: printpix(6, 15); break;
				}
			}
		}
	}
	return 0;
}
