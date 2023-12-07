#pragma once

#include "RGB.h"
#include <vector>

using namespace std;

class Cell { // virtual Pixel
private:
	int _layer;
	RGBColor _color;
	bool _isBounder;
public:
	Cell() {
		_layer = -1;
		_color = RGBColor::BACKROUND;
		_isBounder = false;
	}

	Cell(int layer, RGBColor color, bool isBounder) {
		_layer = layer;
		_color = color;
		_isBounder = isBounder;
	}

	int layer() { return _layer; }
	RGBColor color() { return _color; }
	bool isBounder() { return _isBounder; }

	void setLayer(int layer) { _layer = layer; }
	void setColor(RGBColor color) { if (color != RGBColor::NONE) _color = color; }
	void setIsBounder(bool isBounder) { _isBounder = isBounder; }
	void set(int layer, RGBColor color, bool isBounder) {
		_layer = layer;
		if (color != RGBColor::NONE) _color = color;
		_isBounder = isBounder;
	}

	void clear() {
		_layer = -1;
		_color = RGBColor::BACKROUND;
		_isBounder = false;
	}
};

class Canvas
{
private:
	int _height;
	int _width;
	vector<vector<Cell*>> _screen;

	bool isInScreen(int y, int x) {
		return !(y < 0 || y >= _height || x < 0 || x >= _width);
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

		/*
		printf("Create Canvas\n");
		printf("Canvas: width = %d, height = %d\n", _screen[0].size(), _screen.size());
		*/
	}

	~Canvas() { erase(); }

	void erase() {
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

	void clear() {
		// printf("Clear canvas\n");
		for (int i = 0; i < _height; i++) {
			for (int j = 0; j < _width; j++) {
				_screen[i][j]->clear();
			}
		}
	}

	void rebuild(int _h, int _w) {
		// clear old data
		erase();

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

		/*
		printf("Rebuild Canvas\n");
		printf("Canvas: width = %d, height = %d\n", _screen[0].size(), _screen.size());
		printf("Screen: width = %d, height = %d\n", glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		*/
	}

	Cell* getCellAt(int x, int y) {
		if (!isInScreen(y,x))
			return nullptr;

		return _screen[y][x];
	}

	bool setCell(int x, int y, int layer, RGBColor color, bool isBounder) {
		// printf("Pixel: (x: % d, y : % d)\n", x, y);

		if (!isInScreen(y, x))
			return false;

		if (layer >= getCellAt(x, y)->layer()) {
			getCellAt(x, y)->set(layer, color, isBounder);
			return true;
		}

		return false;
	}
	
	bool isValidCell(int x, int y) {
		return isInScreen(y, x);
	}
};
