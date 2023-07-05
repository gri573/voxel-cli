#include <stdlib.h>
#include <stdio.h>
// global variables
extern int renderdist;
extern int worldheight;
extern int chunksize;
// mathematical functions:

// maximum
double max(double x, double y);
// minimum
double min(double x, double y);
// modulo
double modd(double x, double m);
// whole-numbered power (positive)
double ipow(double x, int e);
// sine
double sin(double x);
// cosine
double cos(double x);
// square root
double sqrt(double x);
// normalize a vector
void normalize(double x[3]);
// calculate the dot product between two vectors
double dot(double x[3], double y[3]);
// calculate the cross product between two vectors
void cross(double x[3], double y[3], double res[3]);

// Terrain generation and noise functions:

// chunk generation
int genchunk(char world[2 * renderdist][worldheight][2 * renderdist], int chunkID[2], int chunkpos[2]);
// height generation
int terrainheight(int coords[2]);
// feature placement
int placefeature(char world[2 * renderdist][worldheight][2 * renderdist], char feature[], int coords[3]);
// load world on startup
int loadworld(char world[2 * renderdist][worldheight][2 * renderdist], char loadedchunks[2 * renderdist / chunksize][2 * renderdist / chunksize], double pp[8]);
// save world on exit
int saveworld(char world[2 * renderdist][worldheight][2 * renderdist], char loadedchunks[2 * renderdist / chunksize][2 * renderdist / chunksize], double pp[8]);
// update world when stuff happens
int updateworld(char world[2 * renderdist][worldheight][2 * renderdist], char loadedchunks[2 * renderdist / chunksize][2 * renderdist / chunksize], double pp[8], double ppp[8]);
// construct chunk file name from location
void getchunkname(const int chunkID[2], char * chunkname);
// read chunk data from file
int readchunk(char world[2 * renderdist][worldheight][2 * renderdist], const int chunkID[2], const int chunkpos[2]);
// write chunk data to file
int writechunk(char world[2 * renderdist][worldheight][2 * renderdist], const int chunkID[2], const int chunkpos[2]);
// move chunk in map to compensate for player movement
int copychunk(char world[2 * renderdist][worldheight][2 * renderdist], const int chunkpos0[2], const int chunkpos1[2]);
// Player-world interaction:
int playeractions(char world[2 * renderdist][worldheight][2 * renderdist], double pp[8], char * input, char * playerdata);

// Rendering:

//print pixel (brightness goes from 0 to 15)
int printpix(char col, int brightness, char* frame, int* loc);
// voxel ray tracer
int raytrace(char world[2 * renderdist][worldheight][2 * renderdist], double pos[3], double dir[3], double dist, double hitpos[3]);
// render entire frame
int renderframe(char world[2 * renderdist][worldheight][2 * renderdist], double pos[8]);
// render sky
int rendersky(double pos[3], double dir[3], char* frame, int* loc);


#define pi 3.141592654
