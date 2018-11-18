#pragma once
#include "GlobalDefines.h"
#include "Object.h"

#define MAX_GRID_ROW		6
#define MAX_GRID_COLUMN		12

constexpr int MAX_RECTANGLES{ 2 };
constexpr int MAX_MENUTRIANGLES{ 14 }; /* 800 / 60 = 13, 총 14개 만들기*/
constexpr int MAX_SLICETRIANGLES{ 2 };
constexpr int MAX_STARS{ 50 };
constexpr int MAX_CELL{ 55 };
/* 숙제 1에 종속적인 GlobalDefines.h 역할. */