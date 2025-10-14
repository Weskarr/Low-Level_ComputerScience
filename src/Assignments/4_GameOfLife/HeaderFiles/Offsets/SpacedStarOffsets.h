#pragma once

#include "../CellOffsets.h"

class SpacedStarOffsets : public CellOffsets {
public:
    std::vector<std::pair<int, int>> getOffsets() override 
    {
        static const std::vector<std::pair<int, int>> offsets =
        {
                              { 0,-2},
                     {-1,-1},          { 1,-1},
            {-2, 0},                            { 2, 0},
                     {-1, 1},          { 1, 1},
                              { 0, 2}
        };

        return offsets;
    }
};
