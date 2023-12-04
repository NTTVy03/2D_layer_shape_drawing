#pragma once

#include "RGB.h"
#include <vector>

using namespace std;

class Cell { // virtual Pixel
private:
	int _x, _y;
	int _layer;
	RGBColor _color;
	bool _isBounder;
public:
	Cell() {
		_x = -1;
		_y = -1;
		_layer = -1;
		_color = RGBColor::WHITE;
		_isBounder = false;
	}

	Cell(int x, int y, int layer, RGBColor color, bool isBounder) {
		_x = x;
		_y = y;
		_layer = layer;
		_color = color;
		_isBounder = isBounder;
	}

	int x() { return _x; }
	int y() { return _y; }
	int layer() { return _layer; }
	RGBColor color() { return _color; }
	bool isBouder() { return _isBounder; }

	void setX(int x) { _x = x; }
	void setY(int y) { _y = y; }
	void setLayer(int layer) { _layer = layer; }
	void setColor(RGBColor color) { _color = color; }
	void setIsBounder(bool isBounder) { _isBounder = isBounder; }
	void set(int x, int y, int layer, RGBColor color, bool isBounder) {
		_x = x;
		_y = y;
		_layer = layer;
		_color = color;
		_isBounder = isBounder;
	}
};

class Canvas
{
private:
	int _height;
	int _width;
	vector<vector<Cell*>> _screen;

	bool isInScreen(int i, int j) {
		return !(i < 0 || i >= _height || j < 0 || j >= _width);
	}
public:
	Canvas() {
		_height = 0;
		_width = 0;
	}

	Canvas(int _h, int _w) {
		_height = _h;
		_width = _w;

		_screen.resize(_height);
		for (int i = 0; i < _height; i++) {
			_screen[i].resize(_width);
			for (int j = 0; j < _width; j++) {
				_screen[i][j] = new Cell();
			}
		}
	}

	~Canvas() { clear(); }

	void clear() {
		for (int i = 0; i < _height; i++) {
			for (int j = 0; j < _width; j++) {
				delete _screen[i][j];
			}
			_screen[i].clear();
		}

		_screen.clear();
	}

	int _h() { return _height; }
	int _w() { return _width; }

	void rebuild(int _h, int _w) {
		// clear old data
		clear();

		// build new data
		_height = _h;
		_width = _w;

		_screen.resize(_height);
		for (int i = 0; i < _height; i++) {
			_screen[i].resize(_width);
			for (int j = 0; j < _width; j++) {
				_screen[i][j] = new Cell();
			}
		}
	}

	Cell* getCellAt(int i, int j) {
		if (!isInScreen(i,j))
			return nullptr;

		return _screen[i][j];
	}

	bool setCell(int x, int y, int layer, RGBColor color, bool isBounder) {
		if (!isInScreen(x, y))
			return false;

		getCellAt(x, y)->set(x, y, layer, color, isBounder);
		return true;
	}
};
