#ifndef CONNECTION_H
#define CONNECTION_H

#include <utility>
#include <cstring>

enum create_type {
	PLAYER,
	BULLET
};

void shoveData(char*& buf, void* data, int size);
void pullData(char*& buf, void* data, int size);

#endif
