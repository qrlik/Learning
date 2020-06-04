#include "figure.h"
#include <algorithm>
#include <random>
#include <QDebug>

const QColor Figure::_emptyCell_color = Qt::lightGray;
const uint8_t Figure::_cell_size = 20;

Figure::Figure(): _array(3), _upper({0,0}) {
    randomizeColors();
}

const QVector<QColor>& Figure::colors() const{
    return _array;
}

Index Figure::upper() const{
    return _upper;
}

Index Figure::lower() const {
    return Index{_upper._x, _upper._y + _array.size() - 1};
}

void Figure::setUpper(Index i){
    _upper = i;
}

void Figure::setUpper(uint x, uint y){
    _upper._x = x;
    _upper._y = y;
}

void Figure::rotateColors(bool down_rotate){
    if(down_rotate){ // right rotate
        std::rotate(_array.rbegin(), _array.rbegin() + 1, _array.rend());
    } else {
        std::rotate(_array.begin(), _array.begin() + 1, _array.end());
    }
}

void Figure::randomizeColors(){ // 2 - 18
    for(auto& color : _array){
        color = static_cast<Qt::GlobalColor>(7 + rand() % 3);
    }
}

void Figure::paint(QPainter& painter){
    Index _cell = _upper;
    for(const auto& color : this->colors()){
        painter.setBrush(color);
        painter.drawRect(_cell._x * Figure::_cell_size, _cell._y * Figure::_cell_size,
                         Figure::_cell_size, Figure::_cell_size);
        ++_cell._y;
    }
}
