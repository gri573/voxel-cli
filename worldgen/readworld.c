#include "../vx_header.h"

void getchunkname(const int chunkID[2], char * chunkname) {
	int digit = 100;
	#define worldnamelen 6
	for (int i = 0; i < 3; i++) {
		chunkname[i + 6 + worldnamelen] = (chunkID[0] % (10 * digit) - chunkID[0] % digit) / digit + 48;
		chunkname[i + 10 + worldnamelen] = (chunkID[1] % (10 * digit) - chunkID[1] % digit) / digit + 48;
		digit /= 10;
	}
}

int readchunk(char world[2 * renderdist][worldheight][2 * renderdist], const int chunkID[2], const int chunkpos[2]) {
	char chunkname[] = "world/chunk_000_000.vxc";
	getchunkname(chunkID, chunkname);
	FILE * chunkfile = fopen(chunkname, "r");
	if (chunkfile == NULL) return -1;
	for (int x = 0; x < chunksize; x++) {
		for (int y = 0; y < worldheight; y++) {
			for (int z = 0; z < chunksize; z++) {
				world[chunkpos[0] + x][y][chunkpos[1] + z] = fgetc(chunkfile);
				if (world[chunkpos[0] + x][y][chunkpos[1] + z] == EOF) {
					fclose(chunkfile);
					chunkfile = NULL;
				}
			}
			if (chunkfile == NULL) break;
			if (fgetc(chunkfile) == EOF) {
				fclose(chunkfile);
				chunkfile = NULL;
				break;
			}
		}
		if (chunkfile == NULL) break;
		if (fgetc(chunkfile) == EOF) {
			fclose(chunkfile);
			chunkfile = NULL;
			break;
		}
	}
	if (chunkfile == NULL) return -2;
	fclose(chunkfile);
	return 0;
}
int writechunk(char world[2 * renderdist][worldheight][2 * renderdist], const int chunkID[2], const int chunkpos[2]) {
	char chunkname[] = "world/chunk_000_000.vxc";
	getchunkname(chunkID, chunkname);
	FILE * chunkfile = fopen(chunkname, "w");
	if (chunkfile == NULL) return -1;
	for (int x = 0; x < chunksize; x++) {
		for (int y = 0; y < worldheight; y++) {
			for (int z = 0; z < chunksize; z++) {
				fputc(world[chunkpos[0] + x][y][chunkpos[1] + z], chunkfile);
			}
			fputc('\n', chunkfile);
		}
		fputc('\n', chunkfile);
	}
	fclose(chunkfile);
	return 0;
}
