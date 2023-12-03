#pragma once

#include "Layer.h"

class DrawingApp {
private:
    vector<Layer> layers;

public:
    void addLayer(int layerId) {
        Layer layer(layerId);
        layers.push_back(layer);
    }

    void addShapeToLayer(int layerId, Shape* shape) {
        for (auto& layer : layers) {
            if (layer.getId() == layerId) {
                layer.addShape(shape);
                break;
            }
        }
    }

    void drawLayers() const {
        for (const auto& layer : layers) {
            layer.drawShapes();
        }
    }

    void removeLayer(int layerId) {
        auto it = find_if(layers.begin(), layers.end(), [layerId](const Layer& layer) {
            return layer.getId() == layerId;
            });

        if (it != layers.end()) {
            layers.erase(it);
        }
    }

    void removeShapeFromLayer(int layerId, int shapeIndex) {
        auto it = find_if(layers.begin(), layers.end(), [layerId](const Layer& layer) {
            return layer.getId() == layerId;
            });

        if (it != layers.end()) {
            it->removeShape(shapeIndex);
        }
    }

    // Add any additional methods you need for the DrawingApp class
};

