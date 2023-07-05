#include "../vx_header.h"

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
	int chunkID0[2] = {(int) (pp[0] + 500 * chunksize) / chunksize - chunkrd, (int) (pp[2] + 500 * chunksize) / chunksize - chunkrd};
	int pchunkID0[2] = {(int) (ppp[0] + 500 * chunksize) / chunksize - chunkrd, (int) (ppp[2] + 500 * chunksize) / chunksize - chunkrd};
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
		int startpos[2] = {
			(2 * chunkrd - 1) * (offset[0] < 0),
			(2 * chunkrd - 1) * (offset[1] < 0)
		};
		int endpos[2] = {
			offset[0] < 0 ? -1 : 2 * chunkrd,
			offset[1] < 0 ? -1 : 2 * chunkrd
		};
		for (int i = startpos[0]; i != endpos[0]; i += (2 * (offset[0] >= 0) - 1)) {
			for (int j = startpos[1]; j != endpos[1]; j += (2 * (offset[1] >= 0) - 1)) {
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
