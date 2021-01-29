#pragma once

#include <stdio.h>
#include <string>

bool doesFileExist(const std::string& filename);
void readFromFileUpToChar(FILE *fptr, const char searchedChar = '^');



