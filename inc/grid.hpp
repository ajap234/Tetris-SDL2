#ifndef GRID_HPP
#define GRID_HPP

#include <array>

#include <SDL.h>

class Tetromino;

constexpr int linesNumber = 21;
constexpr int columnsNumber = 17;
constexpr int blockSize = 36;
constexpr int emptyBlock = 0;
constexpr int blankBlock = 7;

class Grid
{
	template <int L, int C>
	using ColorLookup = std::array<std::array<int, C>, L>;
	
	public:
		Grid();
		~Grid() = default;
	
	public:
		std::array<unsigned, linesNumber>& getGrid();
		std::array<std::array<int, columnsNumber>, linesNumber>& getColorLookup();
		void insertTetromino(Tetromino const& tetromino);
		void displayGrid(SDL_Renderer* renderer, Tetromino const& tetromino) const;
	
	private:
		std::array<unsigned, linesNumber> m_grid = {};
		std::array<std::array<int, columnsNumber>, linesNumber> m_blocksColor = {};
};

#endif // GRID_HPP

