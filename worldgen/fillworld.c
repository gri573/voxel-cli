#include "../vx_header.h"

int genchunk(char world[2 * renderdist][worldheight][2 * renderdist], int chunkID[2], int chunkpos[2]) {
	int cworldpos[2] = {chunkID[0] * chunksize, chunkID[1] * chunksize};
	for (int x = 0; x < chunksize; x++) {
		for (int y = 0; y < worldheight; y++) {
			for (int z = 0; z < chunksize; z++) {
				int worldpos[2] = {cworldpos[0] + x, cworldpos[1] + z};
				int height = terrainheight(worldpos);
				if (y > 10) {
					world[chunkpos[0] + x][y][chunkpos[1] + z] = 32;
				} else if (y > 7) {
					world[chunkpos[0] + x][y][chunkpos[1] + z] = 34;
				} else {
					world[chunkpos[0] + x][y][chunkpos[1] + z] = 33;
				}
			}
		}
	}
	return 0;
}

int loadworld(char world[2 * renderdist][worldheight][2 * renderdist], char loadedchunks[2 * renderdist / chunksize][2 * renderdist / chunksize], double pp[8]) {
	int control = 0;
	int chunkrd = renderdist / chunksize;
	int chunkID0[2] = {(int) pp[0] / chunksize + 500 - chunkrd, (int) pp[2] / chunksize + 500 - chunkrd};
	for (int i = 0; i < chunkrd * 2; i++) {
		for (int j = 0; j < chunkrd * 2; j++) {
			if (!loadedchunks[i][j]) {
				int chunkID[2] = {chunkID0[0] + i, chunkID0[1] + j};
				int chunkpos[2] = {i * chunksize, j * chunksize};
				control = readchunk(world, chunkID, chunkpos);
				if (control == -1) {
					putchar('.');
					genchunk(world, chunkID, chunkpos);
				}
				if (control != -2) loadedchunks[i][j] = 1;
			}
		}
	}
	return control;
}

int saveworld(char world[2 * renderdist][worldheight][2 * renderdist], char loadedchunks[2 * renderdist / chunksize][2 * renderdist / chunksize], double pp[8]) {
	int control = 0;
	int chunkrd = renderdist / chunksize;
	int chunkID0[2] = {(int) pp[0] / chunksize + 500 - chunkrd, (int) pp[2] / chunksize + 500 - chunkrd};
	for (int i = 0; i < chunkrd * 2; i++) {
		for (int j = 0; j < chunkrd * 2; j++) {
			if (loadedchunks[i][j]) {
				int chunkID[2] = {chunkID0[0] + i, chunkID0[1] + j};
				int chunkpos[2] = {i * chunksize, j * chunksize};
				control = writechunk(world, chunkID, chunkpos);
			}
		}
	}
	return control;
}

int copychunk(char world[2 * renderdist][worldheight][2 * renderdist], const int chunkpos0[2], const int chunkpos1[2]) {
	for (int x = 0; x < chunksize; x++) {
		for (int y = 0; y < worldheight; y++) {
			for (int z = 0; z < chunksize; z++) {
				world[chunkpos0[0] * chunksize + x][y][chunkpos0[1] * chunksize + z] = world[chunkpos1[0] * chunksize + x][y][chunkpos1[1] * chunksize + z];
			}
		}
	}
	return 0;
}

int updateworld(char world[2 * renderdist][worldheight][2 * renderdist], char loadedchunks[2 * renderdist / chunksize][2 * renderdist / chunksize], double pp[8], double ppp[8]) {
	int control = 0;
	int chunkrd = renderdist / chunksize;
	int chunkID0[2] = {(int) pp[0] / chunksize + 500 - chunkrd, (int) pp[2] / chunksize + 500 - chunkrd};
	int pchunkID0[2] = {(int) ppp[0] / chunksize + 500 - chunkrd, (int) ppp[2] / chunksize + 500 - chunkrd};
	int offset[2] = {chunkID0[0] - pchunkID0[0], chunkID0[1] - pchunkID0[1]};
	if (offset[0] || offset[1]) {
		for (int i = 0; i < chunkrd * 2; i++) {
			for (int j = 0; j < chunkrd * 2; j++) {
				if (loadedchunks[i][j]) {
					int pchunkID[2] = {pchunkID0[0] + i, pchunkID0[1] + j};
					int coords[2] = {pchunkID[0] + offset[0], pchunkID[1] + offset[1]};
					if (coords[0] < 0 || coords[0] >= 2 * chunkrd || coords[1] < 0 || coords[1] >= 2 * chunkrd) {
						int chunkpos[2] = {i * chunksize, j * chunksize};
						control = writechunk(world, pchunkID, chunkpos);
						loadedchunks[i][j] = 0;
					}
				}
			}
		}
		for (int i = (2 * chunkrd - 1) * (offset[0] < 0); (2 * chunkrd - 1) * (offset[0] < 0) + (2 * (offset[0] >= 0) - 1) * i < 2 * chunkrd; i += (2 * (offset[0] >= 0) - 1)) {
		for (int j = (2 * chunkrd - 1) * (offset[1] < 0); (2 * chunkrd - 1) * (offset[1] < 0) + (2 * (offset[1] >= 0) - 1) * j < 2 * chunkrd; j += (2 * (offset[1] >= 0) - 1)) {
				int cp0[2] = {i, j};
				int cp1[2] = {i + offset[0], j + offset[1]};
				if (cp1[0] >= 0 && cp1[0] < 2 * chunkrd && cp1[1] >= 0 && cp1[1] < 2 * chunkrd) copychunk(world, cp0, cp1);
			}
		}
		for (int i = 0; i < chunkrd * 2; i++) {
			for (int j = 0; j < chunkrd * 2; j++) {
				if (!loadedchunks[i][j]) {
					int chunkID[2] = {chunkID0[0] + i, chunkID0[1] + j};
					int pcoords[2] = {chunkID[0] - offset[0], chunkID[1] - offset[1]};
					if (pcoords[0] < 0 || pcoords[0] >= 2 * chunkrd || pcoords[1] < 0 || pcoords[1] >= 2 * chunkrd) {
						int chunkpos[2] = {i * chunksize, j * chunksize};
						int exists = readchunk(world, chunkID, chunkpos);
						if (exists == -1) genchunk(world, chunkID, chunkpos);
						loadedchunks[i][j] = 1;
						if (exists == -2) control = -2;
					}
				}
			}
		}
	}
	return control;
}
