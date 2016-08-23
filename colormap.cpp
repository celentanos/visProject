#include "colormap.h"

ColorMap::ColorMap(double min, double max)
{
    this->min = min;
    this->max = max;
    multiplier = (max - min) / colors.size();
}

Color ColorMap::getColor(double val)
{
//    if(val < 0)
//        val = 0;
    int i = (val - min) / multiplier - 1;
    if(i >= colors.size())
        std::cout << "i:" << i << std::endl;
    if(i < 0)
        i++;
    return colors.at(i);
}

Color ColorMap::getColorRGB(int r, int g, int b)
{
    return Color(r / C_MAX_VAL, g / C_MAX_VAL, b / C_MAX_VAL);
}
