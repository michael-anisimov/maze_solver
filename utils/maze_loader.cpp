#include "../include/types.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>
#include <string>

Matrix readMaze(const std::string& filename, Coordinates& start, Coordinates& end) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return Matrix();
    }
    
    std::vector<std::string> lines;
    std::string line;
    
    // Read all lines from the file
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    
    file.close();
    
    if (lines.size() < 3) {
        std::cerr << "Error: Invalid maze file format in " << filename << std::endl;
        return Matrix();
    }
    
    // Find the maze lines (everything before "start" and "end" lines)
    std::vector<std::string> mazeLines;
    std::string startLine, endLine;
    
    for (const std::string& l : lines) {
        if (l.substr(0, 5) == "start") {
            startLine = l;
        } else if (l.substr(0, 3) == "end") {
            endLine = l;
        } else {
            mazeLines.push_back(l);
        }
    }
    
    if (mazeLines.empty() || startLine.empty() || endLine.empty()) {
        std::cerr << "Error: Invalid maze file format in " << filename << std::endl;
        return Matrix();
    }
    
    // Parse dimensions
    int height = mazeLines.size();
    int width = 0;
    for (const std::string& l : mazeLines) {
        width = std::max(width, static_cast<int>(l.length()));
    }
    
    if (width <= 0 || height <= 0) {
        std::cerr << "Error: Invalid maze dimensions in " << filename << std::endl;
        return Matrix();
    }
    
    // Initialize maze matrix
    Matrix maze(height, std::vector<int>(width, 0));
    
    // Parse maze data
    for (int y = 0; y < height; ++y) {
        const std::string& mazeLine = mazeLines[y];
        for (int x = 0; x < static_cast<int>(mazeLine.length()) && x < width; ++x) {
            if (mazeLine[x] == 'X') {
                maze[y][x] = 1;  // Wall
            } else if (mazeLine[x] == ' ') {
                maze[y][x] = 0;  // Path
            }
            // Default is 0 (path) for any other character
        }
    }
    
    // Parse start coordinates
    std::istringstream startStream(startLine.substr(6)); // Skip "start "
    char comma;
    if (!(startStream >> start.first >> comma >> start.second)) {
        std::cerr << "Error: Invalid start coordinates in " << filename << std::endl;
        return Matrix();
    }
    
    // Parse end coordinates
    std::istringstream endStream(endLine.substr(4)); // Skip "end "
    if (!(endStream >> end.first >> comma >> end.second)) {
        std::cerr << "Error: Invalid end coordinates in " << filename << std::endl;
        return Matrix();
    }
    
    // Validate coordinates
    if (start.first < 0 || start.first >= width || start.second < 0 || start.second >= height ||
        end.first < 0 || end.first >= width || end.second < 0 || end.second >= height) {
        std::cerr << "Error: Invalid coordinates in " << filename << std::endl;
        return Matrix();
    }
    
    // Mark start and end positions in the maze
    maze[start.second][start.first] = -1;
    maze[end.second][end.first] = -2;
    
    return maze;
} 