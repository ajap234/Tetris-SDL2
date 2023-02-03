#include "linesystem.hpp"
#include "grid.hpp"

unsigned checkLineCompletion(Grid& grid)
{    
    auto& tmpGrid = grid.getGrid();
    unsigned completedLines = 0;
    
    for (int line = linesNumber - 1; line >= 0; line--)
    {
        if ((tmpGrid[line] & fullLine) == fullLine)
        {
            completedLines |= (1 << line);
        }
    }
    
    return completedLines;
}

void vanishLines(Grid& grid, unsigned const& completedLines)
{
    auto& tmpGrid = grid.getGrid();
    auto& tmpColorLookup = grid.getColorLookup();
    if (completedLines == 0)
    {
        return;
    }
    
    for (int line = linesNumber - 1; line >= 0; line--)
    {
        if ((int) (completedLines & (1 << line)) == (int) (1 << line))
        {
            tmpGrid[line] = 0;
            
            for (auto& color : tmpColorLookup[line])
            {
		        color = blankBlock;
	        }
        }
    }
}

void dropLines(Grid& grid, unsigned const& vanishedLines)
{
    auto& tmpGrid = grid.getGrid();
    auto& tmpColorLookup = grid.getColorLookup();
    for (int line = 0; line < linesNumber; line++)
    {		
	    if ((vanishedLines << ((linesNumber - 1) - line)) & (1 << (linesNumber - 1)))
	    {
	        for (int i = line; i >= 0; i--)
	        {				
	    	    if(i == 0)
	    	    {
		            tmpGrid[i] = 0;
	    	        for (int& color : tmpColorLookup[i])
		            {
		           	    color = blankBlock;
		            }
		        }
				
		        else
		        {
		            tmpGrid[i] = tmpGrid[i - 1];
		            tmpColorLookup[i] = tmpColorLookup[i - 1];
		        }
	        }
	    }
    }
}
