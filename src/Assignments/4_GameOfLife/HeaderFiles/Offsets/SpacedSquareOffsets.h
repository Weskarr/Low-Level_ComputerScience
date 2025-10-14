#pragma once

#include "../CellOffsets.h"

class SpacedSquareOffsets : public CellOffsets {
public:
    std::vector<std::pair<int, int>> getOffsets() override 
    {
        static const std::vector<std::pair<int, int>> offsets =
        {
            {-2,-2}, {-1,-2}, { 0,-2}, { 1,-2}, { 2,-2},
            {-2,-1},                            { 2,-1},
            {-2, 0},                            { 2, 0},
            {-2, 1},                            { 2, 1},
            {-2, 2}, {-1, 2}, { 0, 2}, { 1, 2}, { 2, 2}
        };

        return offsets;
    }
};
