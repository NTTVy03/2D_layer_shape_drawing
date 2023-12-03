#pragma once

#include "Shape.h"
#include "RGB.h"

class Global {
public:
	static int selectedShapeType;  // 0: No shape selected, 1: Rectangle, 2: Circle, 3: Triangle
	static Shape* selectedShape;
	static bool isSelectingMode; // is in selecting mode (True: selecting; False: drawing)
	static Shape* newShape; // new shape in Selecting type of shape
	static RGBColor curFillColor;
};