#include "vx_header.h"

double max(double x, double y) {
	if (x > y) return x;
	else return y;
}

double min(double x, double y) {
	if (x < y) return x;
	else return y;
}

double modd(double x, double m) {
	int s = 1;
	if (x * m < 0) s = -1;
	if (x < 0) x *= -1;
	if (m < 0) m *= -1;
	double r = x / m;
	r -= (int) (x / m);
	if (s < 0) {
		r -= 1;
		r *= -1;
	}
	return r * m;
}

double ipow(double x, int e) {
	double p = 1;
	for (int i = 0; i < e; i++) p *= x;
	return p;
}

double sin(double x) {
	x = modd(x + pi * 0.5, 2.0 * pi) - pi * 0.5;
	if (x < pi * 0.5) return x - ipow(x, 3) / 6.0 + ipow(x, 5) / 120.0;
	else return -((x - pi) - ipow(x - pi, 3) / 6.0 + ipow(x - pi, 5) / 120.0);
}

double cos(double x) {
	return sin(x + pi * 0.5);
}

double sqrt(double x) {
	double r = 0.5;
	for (int i = 0; i < 5; i++) {
		r = (r + x / r) / 2;
	}
	return r;
}

void normalize(double x[3]) {
	double r = sqrt(dot(x, x));
	for (int i = 0; i < 3; i++) x[i] /= r;
	return;
}

double dot(double x[3], double y[3]) {
	double r = 0;
	for (int i = 0; i < 3; i++) r += x[i] * y[i];
	return r;
}

void cross(double x[3], double y[3], double res[3]) {
	for (int i = 0; i < 3; i++) res[i] = x[(i+1) % 3] * y[(i+2) % 3] - x[(i+2) % 3] * y[(i+1) % 3];
	return;
}
