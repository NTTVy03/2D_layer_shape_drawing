#pragma once

#pragma once

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
	}

	int r() { return _r; }
	int g() { return _g; }
	int b() { return _b; }

	void set(int r, int g, int b) {
		_r = r;
		_g = g;
		_b = b;
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