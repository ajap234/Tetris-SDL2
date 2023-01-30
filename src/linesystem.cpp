#include "linesystem.hpp"
#include "grid.hpp"

unsigned checkLineCompletion(Grid& grid)
{    
    unsigned completedLines = 0;
    
    for (int line = linesNumber - 1; line >= 0; line--)
    {
        if ((grid.getGrid()[line] & fullLine) == fullLine)
        {
            completedLines |= (1 << line);
        }
    }
    
    return completedLines;
}

void vanishLines(Grid& grid, unsigned const& completedLines)
{
    if (completedLines == 0)
    {
        return;
    }
    
    for (int line = linesNumber - 1; line >= 0; line--)
    {
        if ((int) (completedLines & (1 << line)) == (int) (1 << line))
        {
            grid.getGrid()[line] = 0;
            
            for (auto& color : grid.getColorLookup()[line])
            {
		color = blankBlock;
	    }
        }
    }
}

void dropLines(Grid& grid, unsigned const& vanishedLines)
{
    for (int line = 0; line < linesNumber; line++)
    {		
	if ((vanishedLines << ((linesNumber - 1) - line)) & (1 << (linesNumber - 1)))
	{
	    for (int i = line; i >= 0; i--)
	    {				
		if(i == 0)
		{
		    grid.getGrid()[i] = 0;
		    for (int& color : grid.getColorLookup()[i])
		    {
			color = blankBlock;
		    }
		}
				
		else
		{
		    grid.getGrid()[i] = grid.getGrid()[i - 1];
		    grid.getColorLookup()[i] = grid.getColorLookup()[i - 1];
		}
	    }
	}
    }
}
