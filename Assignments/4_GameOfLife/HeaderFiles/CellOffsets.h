
#pragma once

#include <utility>  // for std::pair
#include <vector>

class CellOffsets {
public:
    virtual ~CellOffsets() = default;
    CellOffsets() = default;

    virtual std::vector<std::pair<int, int>> getOffsets() = 0;
};

