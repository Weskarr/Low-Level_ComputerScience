#pragma once

#include "../CellOffsets.h"

class RoundedLargeSquareOffsets : public CellOffsets {
public:
    std::vector<std::pair<int, int>> getOffsets() override 
    {
        static const std::vector<std::pair<int, int>> offsets =
        {
                     {-1,-2}, {0,-2}, {1,-2},
            {-2,-1}, {-1,-1}, {0,-1}, {1,-1}, {2,-1},
            {-2, 0}, {-1, 0},         {1, 0}, {2, 0},
            {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1},
                     {-1, 2}, {0, 2}, {1, 2}
        };

        return offsets;
    }
};
