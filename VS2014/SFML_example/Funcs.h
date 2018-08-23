#pragma once
#include <cstdlib>

// ��

#ifndef PI
#define PI 3.141592653589793238462643383
#endif

// ���������

#define ASTEROIDS 1
#define GRAVITYS 0

// ������� �������� ��������� ������� ����� �� 0 �� 1

double random() {
	return double(rand()) / 32768.0;
}

// ���� �����

short sgn(double x) {
	return x > 0 ? 1 : (x < 0 ? -1 : 0);
}

short sgn(int x) {
	return x > 0 ? 1 : (x < 0 ? -1 : 0);
}

// ������� �����

double sq(double x) {
	return x * x;
}

uint64_t sq(int x) {
	return x * x;
}