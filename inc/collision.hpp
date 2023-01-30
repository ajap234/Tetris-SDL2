#ifndef COLLISION_HPP
#define COLLISION_HPP

class Grid;

bool checkTetrominoTetrominoCollision(Grid& grid, unsigned pattern, int xBlock, int yBlock);
bool checkTetrominoGridCollision(int oriDim, int xBlock, int yBlock, int xDirection, int yDirection, bool rotate);

#endif // COLLISION_HPP
