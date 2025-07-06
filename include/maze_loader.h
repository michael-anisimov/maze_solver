#ifndef MAZE_LOADER_H
#define MAZE_LOADER_H

#include "types.h"
#include <string>

Matrix readMaze(const std::string& filename, Coordinates& start, Coordinates& end);

#endif // MAZE_LOADER_H 