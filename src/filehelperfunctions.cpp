#include "filehelperfunctions.h"

#include <io.h>

bool doesFileExist(const std::string & filename) {
	return (access(filename.c_str(), 0) == 0);
}

void readFromFileUpToChar(FILE *fptr, const char searchedChar) {
	char c;
	do {
		fscanf(fptr, "%c", &c);
	}
	while (c != searchedChar && !feof(fptr));
}
