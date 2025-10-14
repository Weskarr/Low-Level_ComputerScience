#pragma once


#include "../CellRules.h"

class ConwayRules : public CellRules {
public:
    bool getNewState(bool hasCell, int currentNeighbourCount) override {
        if (hasCell) return !(currentNeighbourCount < 2 || currentNeighbourCount > 3);
        return currentNeighbourCount == 3;
    }
};
