#include <iostream>
#include <random>
    
#include "tetromino.hpp"
#include "grid.hpp"
#include "collision.hpp"

Tetromino::Tetromino(Grid& grid)
{
	m_tetromino = getRandomNumber();
	m_rotation = 0;
    m_pattern = tetrominos[m_tetromino][m_rotation];
	m_originDimensions = getOriginDimensions(tetrominos[m_tetromino][m_rotation]);
    
    m_xBlock = (columnsNumber / 2) - sizes[m_tetromino] / 2;
	m_yBlock = 0;
    
    if (checkTetrominoTetrominoCollision(grid, m_pattern, m_xBlock, m_yBlock))
    {
        m_blocked = true;
    }
    
	m_xOrigin = (m_originDimensions >> 9) & 0x007;
	m_yOrigin = (m_originDimensions >> 6) & 0x007;
	m_wBlock = (m_originDimensions >> 3) & 0x007;
	m_hBlock = m_originDimensions & 0x007;
}

Uint32 Tetromino::m_fallingThreshold = defaultFallingThreshold;

int Tetromino::getTetromino() const
{
    return m_tetromino;
}

int Tetromino::getX() const
{
    return m_xBlock;
}

int Tetromino::getY() const
{
    return m_yBlock;
}

int Tetromino::getPattern() const
{
    return tetrominos[m_tetromino][m_rotation];
}

SDL_Color const& Tetromino::getColor(int index) const
{
    return colors[index];
}

bool Tetromino::getBlockedState() const
{
    return m_blocked;
}

void Tetromino::setBlockedState(bool set)
{
    m_blocked = set;
}

int Tetromino::getOriginDimensions(unsigned const& pattern)
{    
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    
    int originDimensions = 0;
    
    for (int i = 0; i < maxSize; i++)
    {
        unsigned column = (pattern & (0x8888 >> i));
        if (x >= 0 && column != 0)
        {
            break;
        }
        
        if (column == 0)
        {
            ++x;
        }
    }
    
    for (int j = 0; j < maxSize; j++)
    {
        unsigned line = (pattern & (0xF000 >> (maxSize * j)));
        
        if (y >= 0 && line != 0)
        {
            break;
        }
        
        if (line == 0)
        {
            ++y;
        }
    }
    
    for (int k = 0; k < maxSize; k++)
    {
        unsigned line = (pattern & (0x8888 >> k));
        if (line != 0)
        {
            ++w;
        }
        
        unsigned column = (pattern & (0xF000 >> (maxSize * k)));
        if (column != 0)
        {
            ++h;
        }
    }
    
    originDimensions |= (h & 0x007);
    originDimensions |= (w << 3) & 0x038;
    originDimensions |= (y << 6) & 0x1C0;
    originDimensions |= (x << 9) & 0xE00;
    
    return originDimensions;
}

Uint32 Tetromino::getFallingThreshold() const
{
    return m_fallingThreshold;
}

void Tetromino::handleUnlatchThreshold(Grid& grid)
{
    if (!m_blocked && SDL_GetTicks() - m_previousUnlatchThreshold > m_fallingThreshold)
    {        
        if (!(checkTetrominoGridCollision(m_originDimensions, m_xBlock, m_yBlock, IDLE, DOWN, NOROTATE)) &&
            !(checkTetrominoTetrominoCollision(grid, tetrominos[m_tetromino][m_rotation], m_xBlock, m_yBlock + DOWN)))
        {
            m_previousUnlatchThreshold = SDL_GetTicks();
            m_yBlock++;
        }
            
        else
        {
            m_blocked = true;
        }
    }
}

void Tetromino::reduceUnlatchThreshold()
{
    m_fallingThreshold -= timeReductionByLevel;
}

void Tetromino::moveLeft(Grid& grid)
{
    if (m_blocked) return;

    m_originDimensions = getOriginDimensions(tetrominos[m_tetromino][m_rotation]);
                
    if (!(checkTetrominoGridCollision(m_originDimensions, m_xBlock, m_yBlock, LEFT, IDLE, NOROTATE)) &&
        !(checkTetrominoTetrominoCollision(grid, tetrominos[m_tetromino][m_rotation], m_xBlock + LEFT, m_yBlock)))
    {
        --m_xBlock;
    }
}

void Tetromino::moveRight(Grid& grid)
{
    if (m_blocked) return;

    m_originDimensions = getOriginDimensions(tetrominos[m_tetromino][m_rotation]);
                
    if (!(checkTetrominoGridCollision(m_originDimensions, m_xBlock, m_yBlock, RIGHT, IDLE, NOROTATE)) &&
        !(checkTetrominoTetrominoCollision(grid, tetrominos[m_tetromino][m_rotation], m_xBlock + RIGHT, m_yBlock)))
    {
        ++m_xBlock;
    }
}

void Tetromino::fallFaster(Grid& grid)
{
    if (m_blocked) return;

    m_originDimensions = getOriginDimensions(tetrominos[m_tetromino][m_rotation]);
                
    if (!(checkTetrominoGridCollision(m_originDimensions, m_xBlock, m_yBlock, IDLE, DOWN, NOROTATE)) &&
        !(checkTetrominoTetrominoCollision(grid, tetrominos[m_tetromino][m_rotation], m_xBlock, m_yBlock + DOWN)))
    {
        ++m_yBlock;
    }
                
    else
    {
        m_blocked = true;
    }
}

void Tetromino::instantFall(Grid& grid)
{
    if (m_blocked) return;

    m_originDimensions = getOriginDimensions(tetrominos[m_tetromino][m_rotation]);
    
    while (!(checkTetrominoGridCollision(m_originDimensions, m_xBlock, m_yBlock, IDLE, DOWN, NOROTATE)) &&
           !(checkTetrominoTetrominoCollision(grid, tetrominos[m_tetromino][m_rotation], m_xBlock, m_yBlock + DOWN)))
    {
        ++m_yBlock;
    }
    
    m_blocked = true;
}

void Tetromino::rotateLeft(Grid& grid)
{
    if (m_blocked) return;

    m_originDimensions = getOriginDimensions(tetrominos[m_tetromino][(m_rotation + LEFT >= 0) ? (m_rotation + LEFT) : (maxSize - 1)]);
                
    if (!(checkTetrominoGridCollision(m_originDimensions, m_xBlock, m_yBlock, IDLE, IDLE, ROTATE)) && 
        !(checkTetrominoTetrominoCollision(grid, tetrominos[m_tetromino][(m_rotation + LEFT >= 0) ? (m_rotation + LEFT) : (maxSize - 1)], m_xBlock, m_yBlock)))
    {
        m_rotation = (m_rotation + LEFT >= 0) ? (m_rotation + LEFT) : (maxSize - 1);
    }
}

void Tetromino::rotateRight(Grid& grid)
{
    if (m_blocked) return;

    m_originDimensions = getOriginDimensions(tetrominos[m_tetromino][(m_rotation + RIGHT) % maxSize]);
                
    if (!(checkTetrominoGridCollision(m_originDimensions, m_xBlock, m_yBlock, IDLE, IDLE, ROTATE)) &&
        !(checkTetrominoTetrominoCollision(grid, tetrominos[m_tetromino][(m_rotation + RIGHT) % maxSize], m_xBlock, m_yBlock)))
    {
        (++m_rotation) %= maxSize;
    }
}

void Tetromino::updateTetromino()
{
    m_pattern = tetrominos[m_tetromino][m_rotation];
}

void Tetromino::displayTetromino(SDL_Renderer* renderer)
{
    for (int i = 0; i < sizes[m_tetromino]; i++)
        {
            for (int j = 0; j < sizes[m_tetromino]; j++)
            {
                SDL_Rect block = {};
                block.x = m_xBlock * blockSize + j * blockSize;
                block.y = m_yBlock * blockSize + i * blockSize;
                block.w = blockSize;
                block.h = blockSize;
                
                SDL_Rect outline = {};
                
                if ((m_pattern & 0x8000) == 0x8000)
                {
                    SDL_SetRenderDrawColor(renderer, colors[m_tetromino].r, colors[m_tetromino].g, colors[m_tetromino].b, 0xFF); // tetromino color
                    outline.x = m_xBlock * blockSize + j * blockSize;
                    outline.y = m_yBlock * blockSize + i * blockSize;
                    outline.w = blockSize;
                    outline.h = blockSize;
                }
                
                else
                {
                    SDL_SetRenderDrawColor(renderer, colors[m_tetromino].r, colors[m_tetromino].g, colors[m_tetromino].b, 0x30); // adding tansparency for the rotation box
                    outline.x = 0;
                    outline.y = 0;
                    outline.w = 0;
                    outline.h = 0;
                }
            
                SDL_RenderFillRect(renderer, &block);
                
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); // outline color
                SDL_RenderDrawRect(renderer, &outline);
            
                m_pattern <<= 1;
            }
            
            m_pattern <<= (maxSize - sizes[m_tetromino]);
        }
}

int Tetromino::getRandomNumber()
{
    std::random_device random;
    std::default_random_engine engine(random());
    std::uniform_int_distribution<int> uniform(0,6);
    return uniform(engine);
}

void Tetromino::resetFallingThreshold()
{
    m_fallingThreshold = defaultFallingThreshold;
}