#include "rectange.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QBitmap>

Rectangle::Rectangle(QWidget* parent) : QWidget(parent), is_Drawing(false) {
    this->setWindowFlags(Qt::WindowFlags { Qt::Window | Qt::WindowStaysOnTopHint } );

    this->setGeometry(0, 0, 450, 450);
    this->setMaximumSize(450, 450);
    this->setWindowTitle(QStringLiteral("2. Прямоугольники"));
    const auto desktop_center = QApplication::desktop()->screenGeometry().center();
    this->move(desktop_center.x(), desktop_center.y() - 460);

    rectangles = QPixmap{450,450}; // фон для рисования прямоугольников
    rectangles.fill(Qt::white);
    rectangles.setMask(QBitmap(rectangles));
}

void Rectangle::mousePressEvent(QMouseEvent* event) {
// Задание (2).6 Рисуем прямоугольник при нажатой правой кнопке мыши
    if(event->button() == Qt::MouseButton::RightButton){
        is_Drawing = true;
        rect_point_f = event->pos();
    }
}

void Rectangle::mouseReleaseEvent(QMouseEvent* event) {
    if(event->button() == Qt::MouseButton::RightButton){
        rect_point_s = event->pos();

        QPainter painter(&rectangles);
        painter.drawRect(QRect{rect_point_f, rect_point_s});
        is_Drawing = false;
        repaint();
        //rect_point_f = (rect_point_s = QPoint{0,0});
    }
}

void Rectangle::mouseMoveEvent(QMouseEvent*){
    //Задание (2).4 Отслеживайте положение курсора относительно виджета и не позволяйте паучку «покидать» пределы рабочей области виджета.
    if(is_Drawing){
        rect_point_s = this->mapFromGlobal(QCursor::pos());
        this->repaint();
    }
}

void Rectangle::PaintingRect(){
    QPainter painter(this);
    painter.drawPixmap(this->rect().topLeft(), rectangles); // рисуем фон с предыдущими прямоугольниками
    if(is_Drawing) {
        painter.drawRect(QRect{rect_point_f, rect_point_s});
    }
}

void Rectangle::paintEvent(QPaintEvent*) {
    PaintingRect();
}

Rectangle::~Rectangle(){}
