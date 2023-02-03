#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#include <SDL.h>

#include <array>

class Grid;

constexpr int tetrominosNumber = 7;
constexpr int rotationsNumber = 4;
constexpr int maxSize = 4;
constexpr int IDLE = 0;
enum XDIRECTION {LEFT = -1, RIGHT = 1};
enum YDIRECTION {UP = -1, DOWN = 1};
enum ROTATION : bool {NOROTATE, ROTATE};
constexpr Uint32 defaultFallingThreshold = 2500;
constexpr Uint32 timeReductionByLevel = 100;

constexpr std::array<std::array<unsigned const, rotationsNumber>, tetrominosNumber> tetrominos = {
        {
                {0x0F00, 0x2222, 0x00F0, 0x4444}, // i
                {0x8E00, 0x6440, 0x0E20, 0x44C0}, // j
                {0x2E00, 0x4460, 0x0E80, 0xC440}, // l
                {0xCC00, 0xCC00, 0xCC00, 0xCC00}, // o
                {0x6C00, 0x4620, 0x06C0, 0x8C40}, // s
                {0x4E00, 0x4640, 0x0E40, 0x4C40}, // t
                {0xC600, 0x2640, 0x0C60, 0x4C80}  // z
        }
};

constexpr std::array<SDL_Color const, tetrominosNumber> colors = {
        {
                {0x00, 0xFF, 0xFF, 0xA0}, // cyan
                {0x00, 0x00, 0xFF, 0xA0}, // blue
                {0xFF, 0xAA, 0x00, 0xA0}, // orange
                {0xFF, 0xFF, 0x00, 0xA0}, // yellow
                {0x00, 0xFF, 0x00, 0xA0}, // green
                {0x99, 0x00, 0xFF, 0xA0}, // purple
                {0xFF, 0x00, 0x00, 0xA0}  // red
        }
};

constexpr std::array<int const, tetrominosNumber> sizes = { { 4, 3, 3, 2, 3, 3, 3 } };

class Tetromino
{
        public:
                Tetromino() = default;
                Tetromino(Grid& grid);
                ~Tetromino() = default;

        public:
                int getTetromino() const;
                int getOriginDimensions(unsigned const& pattern);
                int getX() const;
                int getY() const;
                int getPattern() const;
                SDL_Color const& getColor(int index) const;
                bool getBlockedState() const;
                void setBlockedState(bool set);
                Uint32 getFallingThreshold() const;
                void handleUnlatchThreshold(Grid& grid);
                static void reduceUnlatchThreshold();
                void moveLeft(Grid& grid);
                void moveRight(Grid& grid);
                void fallFaster(Grid& grid);
                void instantFall(Grid& grid);
                void rotateLeft(Grid& grid);
                void rotateRight(Grid& grid);
                void updateTetromino();
                void displayTetromino(SDL_Renderer* renderer);
                void castTetrominoShadow(Grid& grid, SDL_Renderer* renderer);
                void resetFallingThreshold();
                bool isMaxLevel() const;
                
        private:
                int getRandomNumber();
                
        private:
                int m_tetromino = 0;
                int m_rotation = 0;
                int m_xBlock = 0;
                int m_yBlock = 0;
                int m_originDimensions = 0;
                int m_xOrigin = 0;
                int m_yOrigin = 0;
                int m_wBlock = 0;
                int m_hBlock = 0;
                int m_pattern = 0;
                Uint32 m_previousUnlatchThreshold = 0;
                static Uint32 m_fallingThreshold;
                bool m_blocked = 0;
};

#endif // TETROMINO_HPP

