#include <iostream>

#include <SDL.h>

#include "grid.hpp"
#include "tetromino.hpp"
#include "linesystem.hpp"

Grid::Grid()
{	
	m_grid.fill(emptyBlock);
	for (auto& line : m_blocksColor)
	{
		line.fill(blankBlock);
	}
}

std::array<unsigned, linesNumber>& Grid::getGrid()
{
	return m_grid;
}

std::array<std::array<int, columnsNumber>, linesNumber>& Grid::getColorLookup()
{
	return m_blocksColor;
}

void Grid::insertTetromino(Tetromino const& tetromino)
{
	int xBlock = tetromino.getX();
	int yBlock = tetromino.getY();
	int pattern = tetromino.getPattern();
	
	unsigned insertedLine = 0;
	
	for (int block = 0; block < (int)maxSize; block++)
	{
		if (yBlock + block < linesNumber)
		{
			insertedLine |= (pattern >> (maxSize * ((maxSize - 1) - block))) & (unsigned)0x000F;
			insertedLine <<= (columnsNumber - xBlock);
			insertedLine >>= maxSize;
			insertedLine &= fullLine;
			m_grid[yBlock + block] |= insertedLine;
			insertedLine = 0;
		}
	}
    
	int insertedIndex = 0;
	int colorPresent = 0;
    
	for (int y = 0; y < maxSize; y++)
	{
		for (int x = 0; x < maxSize; x++)
		{
			if (xBlock + x < columnsNumber && yBlock + y < linesNumber)
			{
				colorPresent = (pattern >> (((maxSize * maxSize) - 1) - (maxSize * y + x))) & 1;

				if (xBlock + x < 0) continue;

				if (m_blocksColor[yBlock + y][xBlock + x] != blankBlock)
				{
					continue;
				}

				if (!colorPresent)
				{
					insertedIndex = blankBlock;
				}

				else
				{
					insertedIndex = tetromino.getTetromino();
				}

				m_blocksColor[yBlock + y][xBlock + x] = insertedIndex;
			}
		}
	}
}

void Grid::displayGrid(SDL_Renderer* renderer, Tetromino const& tetromino) const
{
	for (int i = 0; i < linesNumber; i++)
        {
		for (int j = 0; j < columnsNumber; j++)
		{
			SDL_Rect gridUnit = {j * blockSize, i * blockSize, blockSize, blockSize};
						
			if (m_blocksColor[i][j] < blankBlock)
			{
				SDL_SetRenderDrawColor(renderer, 
			                           tetromino.getColor(m_blocksColor[i][j]).r,
                                       tetromino.getColor(m_blocksColor[i][j]).g,
                                       tetromino.getColor(m_blocksColor[i][j]).b,
							           0xFF); // grid block color
				
				SDL_RenderFillRect(renderer, &gridUnit);
			}
			
			SDL_SetRenderDrawColor(renderer, 0x7F, 0x7F, 0x7F, 0x3F); // grid color
			SDL_RenderDrawRect(renderer, &gridUnit);
		}
    }
}

