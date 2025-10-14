#pragma once


#include "../CellRules.h"

class DayNight : public CellRules {
public:
    bool getNewState(bool hasCell, int currentNeighbourCount) override {
        if (hasCell)
            return currentNeighbourCount == 3 || currentNeighbourCount == 6 ||
                   currentNeighbourCount == 7 || currentNeighbourCount == 8;
        return currentNeighbourCount >= 3 && currentNeighbourCount <= 8;
    }
};
