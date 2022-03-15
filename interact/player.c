#include "../vx_header.h"

int playeractions(char world[2 * renderdist][worldheight][2 * renderdist], double pp[8], char * input) {
	*input = fgetc(stdin);
	if (*input == 27) return 1;
	for (int i = 0; i < 2; i++) pp[2 * i + 5] *= 0.4;
	if (*input == 'i') pp[4] -= 0.03;
	if (*input == 'k') pp[4] += 0.03;
	if (*input == 'j') pp[3] -= 0.03;
	if (*input == 'l') pp[3] += 0.03;
	if (pp[4] < 0) pp[4] = 0;
	if (pp[4] > pi) pp[4] = pi;
	pp[3] = modd(pp[3], 2 * pi);
	double dir[3] = {0};
	if (*input == 'w') {
		dir[0] = cos(pp[3]);
		dir[2] = sin(pp[3]);
	}
	if (*input == 's') {
		dir[0] = -cos(pp[3]);
		dir[2] = -sin(pp[3]);
	}
	if (*input == 'a') {
		dir[0] = sin(pp[3]);
		dir[2] = -cos(pp[3]);
	}
	if (*input == 'd') {
		dir[0] = -sin(pp[3]);
		dir[2] = cos(pp[3]);
	}
	if (*input == ' ') pp[1] += 0.2;
	if (*input == '<') pp[1] -= 0.2;
	for (int i = 0; i < 3; i++) {
		pp[i+5] += dir[i];
		pp[i] += 0.2 * pp[i+5];
	}
	double dirfacing[3] = {cos(pp[3]) * sin(pp[4]), cos(pp[4]), sin(pp[3]) * sin(pp[4])};
	int worldoffset[2] = {((int) pp[0] - renderdist) / chunksize, ((int) pp[2] - renderdist) / chunksize};
	double pointedloc[3] = {-1};
	double pp0[3] = {pp[0] - worldoffset[0] * chunksize, pp[1], pp[2] - worldoffset[1] * chunksize};
	raytrace(world, pp0, dirfacing, 5.0, pointedloc);
	int pointedblock[3] = {(int) pointedloc[0], (int) pointedloc[1], (int) pointedloc[2]};
	int normal[3] = {0};
	if (pointedblock[0] != -1) {
		for (int i = 0; i < 3; i++) {
			double dfc = modd(pointedloc[i], 1.0) - 0.5;
			if (dfc > 0) dfc *= -1;
			dfc += 0.5;
			if (dfc < 0.01) {
				normal[i] = 1;
				if (dirfacing[i] > 0) normal[i] *= -1;
				break;
			}
		}
		if (*input == 'u') {
			world[pointedblock[0]][pointedblock[1]][pointedblock[2]] = 32;
		}
		if (*input == 'o') {
			for (int i = 0; i < 3; i++) pointedblock[i] += normal[i];
			world[pointedblock[0]][pointedblock[1]][pointedblock[2]] = 35;
		}
	}
	printf("\033[1;0m\n\rLooking at block: (%d, %d, %d)\n\r", pointedblock[0], pointedblock[1], pointedblock[2]);
	printf("Normal: (%d, %d, %d)\n\r", normal[0], normal[1], normal[2]);
	printf("Location: (%f, %f, %f)\n\r", pp[0], pp[1], pp[2]);
	printf("Angle: (%f, %f)\n\r", pp[3], pp[4]);
	return 0;
}
