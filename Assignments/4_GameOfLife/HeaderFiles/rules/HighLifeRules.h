#pragma once


#include "../CellRules.h"

/*
 * http://www.conwaylife.com/wiki/HighLife
 */

class HighLifeRules : public CellRules {
public:
    bool getNewState(bool hasCell, int currentNeighbourCount) override {
        if (hasCell) return !(currentNeighbourCount < 2 || currentNeighbourCount > 3);
        return currentNeighbourCount == 3 || currentNeighbourCount == 6;
    }
};
