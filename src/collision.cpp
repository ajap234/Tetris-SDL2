#include <iostream>

#include "collision.hpp"
#include "grid.hpp"
#include "tetromino.hpp"

bool checkTetrominoTetrominoCollision(Grid& grid, unsigned pattern, int xBlock, int yBlock)
{    
    unsigned lineCollision = 0;

    std::array<unsigned, maxSize> lines;

    for (int i = 0; i < maxSize; i++)
    {
        if (yBlock + i < linesNumber) // are we still in the grid ?
        {
            lines[i] = grid.getGrid()[yBlock + i]; // if so then we get the line at index (yBlock + i) 
        }

        else
        {
            lines[i] = 0; // otherwise there is nothing in the line so no collision for this line
        }

        // std::cout << std::hex << "lines[" << i << "] = " << lines[i] << std::dec << std::endl;
    }

    for (int block = 0; block < maxSize; block++)
    {
         lineCollision |= (pattern >> (maxSize * ((maxSize - 1) - block))) & (unsigned) 0x000F; // get tetromino line
         
         lineCollision <<= (columnsNumber - xBlock); // place it on its x position
         lineCollision >>= maxSize; // then add tetromino size to get the correct x position
        
         // std::cout << std::hex << "lines[" << block << "] = " << lines[block] << std::dec << std::endl;

         lineCollision &= lines[block]; // check overlapping blocks
         
         if (lineCollision != 0) // if there are one or more...
         {
            return true; // then there is a collision
         }
         
         lineCollision = 0; // otherwise we continue
    }
    
    return false;
}

bool checkTetrominoGridCollision(int oriDim, int xBlock, int yBlock, int xDirection, int yDirection, bool rotate)
{
    int xOrigin = (oriDim >> 9) & 0x007;
    int yOrigin = (oriDim >> 6) & 0x007;
    int wBlock = (oriDim >> 3) & 0x007;
    int hBlock = oriDim & 0x007;
    
    if ((xDirection == LEFT && (xBlock + xOrigin + LEFT) < 0) || // block tetromino from going through the left wall
        (xDirection == RIGHT && (xBlock + xOrigin + wBlock + RIGHT) > columnsNumber) || // block tetromino from going through the right wall
        (yDirection == DOWN && (yBlock + yOrigin + hBlock + DOWN) > linesNumber) || // block tetromino from going through the ground
        (rotate && (xBlock + xOrigin) < 0) || // block tetromino from rotaing through the left wall
        (rotate && (xBlock + xOrigin + wBlock) > columnsNumber) || // block tetromino from rotating through the right wall
        (rotate && (yBlock + yOrigin + hBlock) > linesNumber)) // block tetromino from rotating through the ground
    {
        return true;
    }
    
    return false;
}
