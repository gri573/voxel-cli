#include "../vx_header.h"

int terrainheight(int coords[2]) {
return worldheight / 2 + (int) (5 * sin(coords[0]) - 3 * sin(0.6 * coords[1]));
}
