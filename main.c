#include "vx_header.h"
	int renderdist = 64;
	int worldheight = 32;
	int chunksize = 8;
int main() {
	system("stty raw");
//	system("stty -echo");
	char world[2 * renderdist][worldheight][2 * renderdist];
	int chunkrenderdist = 2 * renderdist / chunksize;
	char loadedchunks[chunkrenderdist][chunkrenderdist];
	for (int i = 0; i < chunkrenderdist; i++) for (int j = 0; j < chunkrenderdist; j++) loadedchunks[i][j] = 0;
	double playerpos[8] = {402.6, 12.5, -82.4};
	loadworld(world, loadedchunks, playerpos);
	char playerinput = 0;
	while (1) {
		double previousplayerpos[8];
		for (int i = 0; i < 8; i++) previousplayerpos[i] = playerpos[i];
		playeractions(world, playerpos, &playerinput);
		if (playerinput == 27) break;
		updateworld(world, loadedchunks, playerpos, previousplayerpos);
		renderframe(world, playerpos);
		//playerinput = 27;
	}
	saveworld(world, loadedchunks, playerpos);
//	system("stty echo");
	system("stty cooked");
	printf("\033[1;0m\n");
	return 0;
}
