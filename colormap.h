#ifndef COLORMAP_H
#define COLORMAP_H

#include <fantom/graphics.hpp>

#define C_MAX_VAL   255.0

using namespace fantom;

class ColorMap
{
public:
    ColorMap(double min, double max);

    Color getColor(double val);

private:
    double min;
    double max;
    double multiplier;

    Color getColorRGB(int r, int g, int b);

    std::vector<Color> colors = {
        getColorRGB(255, 247, 236),
        getColorRGB(254, 232, 200),
        getColorRGB(253, 212, 158),
        getColorRGB(253, 187, 132),
        getColorRGB(252, 141, 89),
        getColorRGB(239, 101, 72),
        getColorRGB(215, 48, 31),
        getColorRGB(153, 0, 0)
    };

};

#endif // COLORMAP_H
