#pragma once


class CellRules {
public:
    virtual ~CellRules() = default;
    CellRules() = default;

    virtual bool getNewState(bool hasCell, int currentNeighbourCount) = 0;
};

