#include "graphics/Rasterizer.hpp"

void Rasterizer::drawPoint(int x, int y, Pixel& pixel){
    frameBuffer_[y][x] = pixel;
}