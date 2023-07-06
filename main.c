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
	double playerpos[8] = {402.6, 12.5, -82.4, 0, 1.6};
	loadworld(world, loadedchunks, playerpos);
	char playerinput = 0;
	char playerdata = 35;
	printf("Welcome to voxel_cli!\n\rControls: WASD for translational, IJKL for rotational movement\n\rO for placing blocks, U for breaking them\n\rP for picking blocks, 0-7 for choosing blocks manually\n\rESC to exit the game\n\n\rpress any of those keys to start the game!\n\r");
	while (1) {
		double previousplayerpos[8];
		for (int i = 0; i < 8; i++) previousplayerpos[i] = playerpos[i];
		playeractions(world, playerpos, &playerinput, &playerdata);
		if (playerinput == 27) {
			printf("really quit? [y/N] ");
			int c = fgetc(stdin);
			if (c == 'y' || c == 'Y')
				break;
		}
		updateworld(world, loadedchunks, playerpos, previousplayerpos);
		renderframe(world, playerpos);
	}
	saveworld(world, loadedchunks, playerpos);
//	system("stty echo");
	system("stty cooked");
	printf("\033[1;0m\n");
	return 0;
}
