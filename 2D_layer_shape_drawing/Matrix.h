#pragma once

#include <vector>
#include <cmath>

#include "Shape.h"

using namespace std;

class Matrix {
private:
	vector<vector<double>> matrix;

	const vector<vector<double>> I_3 = { {1,0,0}, {0,1,0}, {0,0,1} };
public:
	Matrix() { matrix = I_3; }
	Matrix(double m11, double m12, double m13, double m21, double m22, double m23, double m31, double m32, double m33) { 
		matrix = {{m11, m12, m13}, {m21, m22, m23}, {m31, m32, m33}};
	}

	vector<vector<double>> Elements() { return matrix; }

	void Reset() { matrix = I_3; }

	double calculateDeterminant() {
		double det = 0;

		det += matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]);
		det -= matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]);
		det += matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);

		return det;
	}

	void Invert() {
		double determinant = calculateDeterminant();

		if (determinant == 0) {
			return;
		}

		vector<vector<double>> invertedMatrix = matrix;

		double invDet = 1.0 / determinant;

		invertedMatrix[0][0] = (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) * invDet;
		invertedMatrix[0][1] = (matrix[0][2] * matrix[2][1] - matrix[0][1] * matrix[2][2]) * invDet;
		invertedMatrix[0][2] = (matrix[0][1] * matrix[1][2] - matrix[0][2] * matrix[1][1]) * invDet;
		invertedMatrix[1][0] = (matrix[1][2] * matrix[2][0] - matrix[1][0] * matrix[2][2]) * invDet;
		invertedMatrix[1][1] = (matrix[0][0] * matrix[2][2] - matrix[0][2] * matrix[2][0]) * invDet;
		invertedMatrix[1][2] = (matrix[0][2] * matrix[1][0] - matrix[0][0] * matrix[1][2]) * invDet;
		invertedMatrix[2][0] = (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]) * invDet;
		invertedMatrix[2][1] = (matrix[0][1] * matrix[2][0] - matrix[0][0] * matrix[2][1]) * invDet;
		invertedMatrix[2][2] = (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]) * invDet;

		matrix = invertedMatrix;
	}

	void Rotate(double angle) {
		// angle: 0 - 360
		double rad = angle * M_PI / 180;
		double sinAngle = sin(rad);
		double cosAngle = sin(rad);

		Matrix rotate = Matrix();
		rotate.matrix[0][0] = cosAngle;
		rotate.matrix[0][1] = -sinAngle;
		rotate.matrix[1][0] = sinAngle;
		rotate.matrix[1][1] = cosAngle;

		Multiply(rotate);
	}

	void Translate(double offsetX, double offsetY) {
		Matrix translate = Matrix();
		translate.matrix[2][0] = offsetX;
		translate.matrix[2][1] = offsetY;

		Multiply(translate);
	}

	void Scale(double scaleX, double scaleY) {
		Matrix scale = Matrix();
		scale.matrix[0][0] = scaleX;
		scale.matrix[1][1] = scaleY;

		Multiply(scale);
	}

	void Shear(double shearX, double shearY) {
		Matrix shear = Matrix();
		shear.matrix[0][1] = shearY;
		shear.matrix[1][0] = shearX;

		Multiply(shear);
	}

	void Multiply(Matrix other) {
		vector<vector<double>> m1 = this->matrix;
		vector<vector<double>> m2 = other.Elements();
		vector<vector<double>> ans = I_3;

		int numRow = m1.size();
		int numCol = m2[0].size();
		int sz = m1[0].size();

		for (int i = 0; i < numRow; i++) {
			for (int j = 0; j < numCol; j++) {
				double sum = 0;
				for (int k = 0; k < sz; k++) {
					sum += m1[i][k] * m2[k][j];
				}
				ans[i][j] = sum;
			}
		}

		matrix = ans;
	}

	vector<Point> TransformPoints(vector<Point> points) {
		if (matrix == I_3) return points;

		vector<Point> ans;
	
		int sz = points.size();
		for (int i = 0; i < sz; i++) {
			ans.push_back(TransformPoint(points[i]));
		}

		return ans;
	}

	Point TransformPoint(Point point) {
		if (matrix == I_3) return point;

		int x = point.x;
		int y = point.y;

		int newX = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2];
		int newY = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2];

		return Point(newX, newY);
	}
};
