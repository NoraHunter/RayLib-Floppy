#pragma once
#include <iostream>

int random(int min, int max) {
	return rand() % (max - min) + min;
}
