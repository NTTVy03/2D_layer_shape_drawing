#pragma once

#include "Shape.h"
#include "Util.h"
#include "VirtualScreen.h"
#include "Global.h"

class DrawingApp {
private:
    vector<Shape*> shapes;
public:
    void addShape(Shape* newShape) {
        shapes.push_back(newShape);
    }

    void drawShapes(Canvas & canva) const {
        for (const auto& shape : shapes) {
            // shape->draw(canva);
            shape->render(canva);
        }
    }

    void removeShapebyLayer(int layer) {
        int sz = shapes.size();

        for (int shapeIndex = 0; shapeIndex < sz; shapeIndex++) {
            if (shapes[shapeIndex]->getLayer() == layer) {
                shapes.erase(shapes.begin() + shapeIndex);
            }
        }
    }    

    Shape* findShapebyLayer(int layer) {
        for (const auto& shape : shapes) {
            if (shape->getLayer() == layer) {
                return shape;
            }
        }

        return nullptr;
    }
    // Add any additional methods you need for the DrawingApp class
};

