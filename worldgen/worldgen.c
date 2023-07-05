#include "../vx_header.h"

int genchunk(char world[2 * renderdist][worldheight][2 * renderdist], int chunkID[2], int chunkpos[2]) {
	int cworldpos[2] = {chunkID[0] * chunksize, chunkID[1] * chunksize};
	for (int x = 0; x < chunksize; x++) {
		for (int y = 0; y < worldheight; y++) {
			for (int z = 0; z < chunksize; z++) {
				int worldpos[2] = {cworldpos[0] + x, cworldpos[1] + z};
				int height = terrainheight(worldpos);
				if (y > height) {
					world[chunkpos[0] + x][y][chunkpos[1] + z] = 32;
				} else if (y > height - 3) {
					world[chunkpos[0] + x][y][chunkpos[1] + z] = 34;
				} else {
					world[chunkpos[0] + x][y][chunkpos[1] + z] = 33;
				}
			}
		}
	}
	return 0;
}

int terrainheight(int coords[2]) {
return 10 + (int) (2 * sin(0.1 * coords[0]) - 4 * sin(0.05 * coords[1]));
}

int placefeature(char world[2 * renderdist][worldheight][2 * renderdist], char feature[], int coords[3]) {
	FILE* featurefile = fopen(feature, "r");
	if (featurefile == NULL) return -1;
	int i = 0;
	int n = 0;
	int fsize[3] = {0};
	char c = '\0';
	while (c != '\n') {
		c = fgetc(featurefile);
		if ((c == ' ' || c == '\n') && i < 3) {
			fsize[i] = n;
			n = 0;
			i++;
		} else if (c > 47 && c < 58) {
			n = 10 * n + c - 48;
		} else if (c != 0) return -2;
	}
	for (int x = 0; x < fsize[0]; x++) {
		for (int y = 0; y < fsize[1]; y++) {
			for (int z = 0; z < fsize[2]; z++) {
				c = fgetc(featurefile);
				if (c == EOF) return -2;
				if (c == '\n') break;
				int nc[3] = {coords[0] + x, coords[1] + y, coords[2] + z};
				if (c != 32 && nc[0] >= 0 && nc[0] < 2 * renderdist && nc[1] >= 0 && nc[1] < worldheight && nc[2] >= 0 && nc[2] < 2 * renderdist) {
					world[nc[0]][nc[1]][nc[2]] = c;
				}
			}
			if (c != '\n') fgetc(featurefile);
		}
		fgetc(featurefile);
	}
	return 0;
}
