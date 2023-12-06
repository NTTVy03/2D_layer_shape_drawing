#pragma once

#include "Shape.h"
#include "VirtualScreen.h"
#include "Drawing.h"

class Global {
public:
	static int height; // screen height
	static int width; // screen width

	static Canvas canvas; // virtual screen

	static int maxLayer;

	static int selectedShapeType;  // 0: No shape selected, 1: Rectangle, 2: Circle, 3: Triangle
	static Shape* selectedShape;
	static bool isSelectingMode; // is in selecting mode (True: selecting; False: drawing)
	static Shape* newShape; // new shape in Selecting type of shape
	static RGBColor curFillColor;

	static DrawingApp drawingApp;

	static void switchMode(bool isSelectingMode) {
		Global::isSelectingMode = isSelectingMode;

		if (Global::newShape) {
			delete Global::newShape;
			Global::newShape = nullptr;
		}

		if (Global::selectedShape) Global::selectedShape->setUnselected(Global::canvas);
		Global::selectedShape = nullptr;
	}
};