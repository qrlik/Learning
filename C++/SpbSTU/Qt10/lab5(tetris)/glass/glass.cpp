#include "glass.h"
#include <QMessageBox>
#include <QDebug>

Glass::Glass(QWidget *parent) : QWidget(parent), _rows(0), _columns(0),
    _score(0), _cur(nullptr), _next(nullptr),
    _timer_step(250), _is_GameOn(false) {
    if(!connect(this, &Glass::signalGlassInit, this, &Glass::slotGlassInit, Qt::QueuedConnection)){
        qDebug() << "No Glass Init connection";
    }
    emit signalGlassInit(); // after setup ui in event loop
}

Glass::~Glass(){
    delete _cur;
    delete _next;
}

void Glass::clear(){
    for(auto& row : _array){
        row.fill(Figure::_emptyCell_color);
    }
    _score = 0;
}

bool Glass::push_cur_down(){
    Index _upper_tmp = _cur->upper();
    uint next_y = _cur->lower()._y + 1;
    if(next_y < _rows && _array[next_y][_upper_tmp._x] == Figure::_emptyCell_color){
        ++_upper_tmp._y;
        _cur->setUpper(_upper_tmp);
        update();
        return true;
    }
    return false;
}

void Glass::move_left(){
    Index _upper = _cur->upper();
    if(_upper._x > 0){
        for(size_t i = 0, size = _cur->colors().size(); i < size; ++i){
            if(_array[_upper._y + i][_upper._x - 1] != Figure::_emptyCell_color){
                return;
            }
        }
        _upper._x -= 1;
        _cur->setUpper(_upper);
        update();
    }
}

void Glass::move_right(){
    Index _upper = _cur->upper();
    if(_upper._x + 1 < _columns){
        for(size_t i = 0, size = _cur->colors().size(); i < size; ++i){
            if(_array[_upper._y + i][_upper._x + 1] != Figure::_emptyCell_color){
                return;
            }
        }
        _upper._x += 1;
        _cur->setUpper(_upper);
        update();
    }
}

void Glass::move_above(uint y, uint x1, uint x2, uint range) { // сдвиг клеток вниз (грубый)
    for(;x1 <= x2; ++x1) { // в каждом столбце
        for(uint i = 0; i < y + 1 - range; ++i){ // сдвигаем на range вниз в [0, y)
            _array[y - i][x1] = _array[y - range - i][x1];
        }

        for(uint i = 0; i < range; ++i){ // обнуляем верхние range клеток
            _array[i][x1] = Figure::_emptyCell_color;
        }
    }
}

bool Glass::checkColors(){ // грубый перебор
    uint maximum_reject = 2; // от 3 и выше клеток схлопываем

    for(size_t r = 0; r < _rows; ++r){ // горизонтали
        QVector<QColor>& row = _array[r];
        QColor actual_color = row[0];
        uint count = 1;
        for(size_t c = 1; c < _columns; ++c){
            if(row[c] == actual_color){
                ++count;
            } else if(row[c] != actual_color) {
                if(actual_color == Figure::_emptyCell_color || count <= maximum_reject){
                    actual_color = row[c];
                    count = 1;
                } else {
                    _score += count;
                    move_above(r, c -  count, c - 1, 1);
                    return true;
                }
            }
        }
        if(count > maximum_reject && actual_color != Figure::_emptyCell_color){
            _score += count;
            move_above(r, _columns -  count, _columns - 1, 1);
            return true;
        }
    }

    for(size_t c = 0; c < _columns; ++c){ // вертикали
        QColor actual_color = _array[0][c];
        uint count = 1;
        for(size_t r = 1; r < _rows; ++r){
            if(_array[r][c] == actual_color){
                ++count;
            } else if(_array[r][c] != actual_color){
                if(actual_color == Figure::_emptyCell_color || count <= maximum_reject){
                    actual_color = _array[r][c];
                    count = 1;
                } else {
                    _score += count;
                    move_above(r - 1, c, c, count);
                    return true;
                }
            }
        }
        if(count > maximum_reject && actual_color != Figure::_emptyCell_color){
            _score += count;
            move_above(_rows - 1, c, c, count);
            return true;
        }
    }
    return false;
}

void Glass::paintEvent(QPaintEvent*){
    QPainter painter(this);
    painter.setPen(Qt::black);
    int y = 0;
    for(const auto& row : _array){
        int x = 0;
        for(const auto& color : row){
            painter.setBrush(color);
            painter.drawRect(x, y, Figure::_cell_size, Figure::_cell_size);
            x += Figure::_cell_size;
        }
        y += Figure::_cell_size;
    }
    if(_is_GameOn){
        _cur->paint(painter);
    }
}

void Glass::keyPressEvent(QKeyEvent* event){
    if(_is_GameOn){
        switch (event->key()) {
        case(Qt::Key_Left):
            move_left();
            break;
        case(Qt::Key_Right):
            move_right();
            break;
        case(Qt::Key_Down):
            _cur->rotateColors(true);
            update();
            break;
        case(Qt::Key_Up):
            _cur->rotateColors(false);
            update();
            break;
        case(Qt::Key_Space):
            while(push_cur_down()) {;}
            break;
        default:
            break;
        }
    }
    QWidget::keyPressEvent(event);
}

void Glass::timerEvent(QTimerEvent*) {
    if(push_cur_down()){
        update();
    } else { // фигурка уперлась, добавляем в массив
        Index _cell = _cur->upper();
        for(const auto& color : _cur->colors()){
            _array[_cell._y][_cell._x] = color;
            ++_cell._y;
        }

        while(checkColors()) {;}
        emit signalScore(_score);

        delete _cur;
        _cur = _next;
        _cur->setUpper(_columns / 2, 0);
        _next = new Figure();
        this->signalNextFigure(_next);

        if(_array[_cur->lower()._y + 1][_cur->lower()._x] != Figure::_emptyCell_color){
            // сразу некуда падать, завершаем игру
            _is_GameOn = false;
            killTimer(_timerID);
            QMessageBox::information(this, QStringLiteral("Game over!"), QStringLiteral("Game over!"));
        }
    }
}

uint Glass::rows() const {
    return _rows;
}

uint Glass::columns() const{
    return _columns;
}

void Glass::setRows(uint rows) {
    _rows = rows;
}

void Glass::setColumns(uint columns) {
    _columns = columns;
}

void Glass::slotGlassInit(){
    _cur = new Figure();
    _next = new Figure();
    assert(static_cast<uint>(_cur->colors().size()) <= _rows);
    _array.resize(_rows);
    for(auto& row : _array){
        row.fill(Figure::_emptyCell_color, _columns);
    }
    this->setFixedSize(_columns * Figure::_cell_size + 1, _rows * Figure::_cell_size + 1);
    signalSizeComputed();
}

void Glass::slotNewGame(){
    if(_is_GameOn){
        killTimer(_timerID);
    }
    _is_GameOn = true;
    this->signalNextFigure(_next);
    this->clear();
    _cur->setUpper(_columns / 2, 0);
    _timerID = startTimer(_timer_step);
    this->setFocus();
    this->update();
}
