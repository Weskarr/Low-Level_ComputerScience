#pragma once

#include "../CellOffsets.h"

class VonNeumannOffsets : public CellOffsets {
public:
    std::vector<std::pair<int, int>> getOffsets() override 
    {
        static const std::vector<std::pair<int, int>> offsets =
        {
                     { 0, -1},
            {-1, 0},           { 1, 0},
                     { 0,  1},
        };

        return offsets;
    }
};
