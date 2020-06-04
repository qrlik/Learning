#ifndef FIGURE_H
#define FIGURE_H

#include <QVector>
#include <QColor>
#include <QPainter>

struct Index{
    uint _x; // column
    uint _y; // row
};

class Figure {
    QVector<QColor> _array;
    Index _upper;
public:
    static const QColor _emptyCell_color;
    static const uint8_t _cell_size;

    explicit Figure();
    const QVector<QColor>& colors() const;
    Index upper() const;
    Index lower() const;
    void setUpper(Index i);
    void setUpper(uint x, uint y);
    void rotateColors(bool down_rotate);
    void randomizeColors();
    void paint(QPainter& painter);
};

#endif // FIGURE_H
