#include "web.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QBitmap>
#include <QTextStream>
#include <QToolTip>
#include <QPainter>

const QString spider_icon_path = QStringLiteral("Images_lab1/icon2.ico");

using namespace std;

Web::Web(QWidget* parent) : QWidget(parent), is_Drawing(false),
    spider(spider_icon_path), default_cursor(new QCursor(Qt::CrossCursor)) {
    this->setWindowFlags(Qt::WindowFlags { Qt::Window | Qt::WindowStaysOnTopHint } );

    this->setGeometry(0, 0, 450, 450);
    this->setMaximumSize(450, 450);
    const auto desktop_center = QApplication::desktop()->screenGeometry().center();
    this->move(desktop_center.x() - 460, desktop_center.y() - 460);

    this->setCursor(*default_cursor);
    spider.setMask(QBitmap(spider));
    drawing_cursor = new QCursor(spider);
}

Web::~Web() {
    delete default_cursor;
    delete drawing_cursor;
}

void Web::mousePressEvent(QMouseEvent* event) {
// Задание (2).1 Выведите в tooltip координаты мыши, полученные  при нажатии  правой кнопки мыши, только в том случае, если нажата клавиша-модификатор Ctrl
    if(event->button() == Qt::MouseButton::RightButton &&
            event->modifiers() & Qt::KeyboardModifier::ControlModifier){
        QPoint global_pos = event->pos(); // позиция мыши
        QString pos_str;
        QTextStream tpos_str(&pos_str);
        tpos_str << QStringLiteral("x = ") << global_pos.x() << QStringLiteral(", y = ") << global_pos.y(); // строка для ToolTip
        QToolTip::showText(event->globalPos(), pos_str,this);
    }
// Задание (2).2 Рисуем паутину при нажатой левой кнопке мыши
    if(event->button() == Qt::MouseButton::LeftButton){
        is_Drawing = true;
        // Задание (2).3 На время рисования изменяйте вид курсора (на паучка)
        this->setCursor(*drawing_cursor);
    }
}

void Web::mouseReleaseEvent(QMouseEvent* event) {
    if(event->button() == Qt::MouseButton::LeftButton){
        is_Drawing = false;
        repaint();
        this->setCursor(*default_cursor);
    }
}

void Web::mouseMoveEvent(QMouseEvent*){
    //Задание (2).4 Отслеживайте положение курсора относительно виджета и не позволяйте паучку «покидать» пределы рабочей области виджета.
    QPoint pos = this->mapFromGlobal(QCursor::pos());
    if(!this->rect().contains(pos)){
        this->unsetCursor();
    } else if(is_Drawing){
        this->setCursor(*drawing_cursor);
        this->repaint();
    }
}

namespace{
    int distance_for_points(QPoint p1, QPoint p2, bool is_vertical){
        if(is_vertical) {
            return p2.y() - p1.y();
        }
        return p2.x() - p1.x();
    }

    void add_points_from_line(QVector<QPoint>& v, QPoint p1, QPoint p2, int steps){
        bool is_vertical = p1.x() == p2.x();
        int step = distance_for_points(p1, p2, is_vertical) / steps;

        for(int i = 1; i < steps; ++i){
            if(is_vertical){
                p1.setY(p1.y() + step);
            } else{
                p1.setX(p1.x() + step);
            }
            v.push_back(p1);
        }
    }
}

void Web::PaintingWeb(){
    const size_t steps = 8; // steps >= 1!
    QRect client_area = this->rect();
    const QPoint& t_l = client_area.topLeft();
    const QPoint& t_r = client_area.topRight();
    const QPoint& b_l = client_area.bottomLeft();
    const QPoint& b_r = client_area.bottomRight();
    // рисуем точки по краю клиентской области
    QVector<QPoint> points;
    points.reserve(4 * steps);
    points.push_back(t_l);
    add_points_from_line(points, t_l, t_r, steps);
    points.push_back(t_r);
    add_points_from_line(points, t_r, b_r, steps);
    points.push_back(b_r);
    add_points_from_line(points, b_r, b_l, steps);
    points.push_back(b_l);
    add_points_from_line(points, b_l, t_l, steps);
    // рисуем линии (курсор -> окно)
    QVector<QLineF> lines;
    lines.reserve(4 * steps); // 4*(steps - 1) + 4
    QPoint cursor_pos = this->mapFromGlobal(QCursor::pos());
    for(const auto& p : points){
        lines.push_back(QLineF{cursor_pos, p});
    }
    // рисуем поперечные линии
    QVector<QLineF> cross_lines;
    for(int i = 0; i + 1 < lines.size(); ++i){
        cross_lines.push_back(QLineF{lines[i].center(), lines[i + 1].center()});
    }
    cross_lines.push_back(QLineF{lines.begin()->center(), lines.rbegin()->center()});

    QPainter painter(this);
    painter.drawLines(lines);
    painter.drawLines(cross_lines);
}

void Web::paintEvent(QPaintEvent*) {
    if(is_Drawing){
        PaintingWeb();
    }
}

//Задание (2).5 При изменении размера требуется формировать в заголовке окна строку, содержащую информацию о текущей ширине и высоте виджета
void Web::resizeEvent(QResizeEvent*){
    QSize cur_size = this->size();
    QString title;
    QTextStream title_os(&title);
    title_os << QStringLiteral("1. Паутина: width = ") << cur_size.width() << QStringLiteral(" height = ") << cur_size.height();
    this->setWindowTitle(title);
}
