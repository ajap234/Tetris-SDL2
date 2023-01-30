#ifndef LINESYSTEM_HPP
#define LINESYSTEM_HPP

class Grid;

constexpr unsigned fullLine = 0x1FFFF;
unsigned checkLineCompletion(Grid& grid);
void vanishLines(Grid& grid, unsigned const& pattern);
void dropLines(Grid& grid, unsigned const& pattern);

#endif // LINESYSTEM_HPP

