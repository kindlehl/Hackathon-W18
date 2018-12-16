#include "../inc/connection.h"

void shoveData(char*& buf, void* data, int size) {
	memcpy(buf, data, size);
	buf += size;
}

void pullData(char*& buf, void* data, int size) {
	memcpy(data, buf, size);
	buf += size;
}
