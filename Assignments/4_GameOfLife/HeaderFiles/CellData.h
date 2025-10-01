#pragma once

#include <iostream>

class CellData 
{
private:
	bool isAlive;
	uint8_t neighborAliveCount;

public:

	// Constructor:
	CellData() : 
		isAlive(false), 
		neighborAliveCount(0) 
	{};

	// Setters
	bool GetAliveStatus() const { return isAlive; };
	uint8_t GetNeighborCount() const { return neighborAliveCount; };

	// Getters
	void SetAliveStatus(bool status) { isAlive = status; };
	void SetAliveNeighborCount(uint8_t count) { neighborAliveCount = count; };
};