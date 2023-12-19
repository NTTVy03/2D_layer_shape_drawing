#pragma once

#pragma once

#include <math.h>

class RGBColor {
public:
	// color
	static const RGBColor WHITE;
	static const RGBColor BLACK;
	static const RGBColor RED;
	static const RGBColor GREEN;
	static const RGBColor BLUE;
	static const RGBColor YELLOW;
	static RGBColor BACKROUND;
	static RGBColor BOUNDER;
	static const RGBColor NONE;

private:
	int _r;
	int _g;
	int _b;

	double selectedFactor = 0.5;
public:
	RGBColor() {
		_r = 255;
		_g = 255;
		_b = 255;
	}

	RGBColor(int r, int g, int b) {
		_r = r;
		_g = g;
		_b = b;

		normalize();
	}

	int r() { return _r; }
	int g() { return _g; }
	int b() { return _b; }

	void normalize() {
		int MAX = 255;
		_r = (_r > MAX ? MAX : _r);
		_g = (_g > MAX ? MAX : _g);
		_b = (_b > MAX ? MAX : _b);
	}

	void set(int r, int g, int b) {
		_r = r;
		_g = g;
		_b = b;

		normalize();
	}

	void setToUnselectedColor() {
		if (*this == NONE) return;

		_r = (double)(_r / selectedFactor);
		_g = (double)(_g / selectedFactor);
		_b = (double)(_b / selectedFactor);

		normalize();
	}

	void setToSelectedColor() {
		if (*this == NONE) return;

		_r = (double)(_r * selectedFactor);
		_g = (double)(_g * selectedFactor);
		_b = (double)(_b * selectedFactor);

		normalize();
	}

	RGBColor& operator=(const RGBColor& other) {
		if (this != &other) {
			_r = other._r;
			_g = other._g;
			_b = other._b;
		}
		return *this;
	}

	bool operator==(const RGBColor& other) const {
		return (_r == other._r && _g == other._g && _b == other._b);
	}

	bool operator!=(const RGBColor& other) const {
		return !(*this == other);
	}
};