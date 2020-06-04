#include "nextfigure.h"
#include <QPainter>
NextFigure::NextFigure(QWidget *parent) : QWidget(parent), _next(nullptr) {

}

void NextFigure::paintEvent(QPaintEvent*) {
    if(_next){
        QPainter painter(this);
        painter.translate(QPoint{(this->width() - Figure::_cell_size) / 2,
                                 (this->height() - _next->colors().size() * Figure::_cell_size) / 2});
        _next->paint(painter);
    }
}

void NextFigure::slotUpdateFigure(Figure* next){
    if(next){
        _next = next;
        update();
    }
}
