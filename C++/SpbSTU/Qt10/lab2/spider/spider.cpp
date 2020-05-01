#include "spider.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QBitmap>

const QString spider_icon_path = QStringLiteral("Images_lab1/icon2.ico");

using namespace std;

Spider::Spider(QWidget* parent) : QWidget(parent), spider(spider_icon_path),
     spider_pos(0,0), spider_direct(1,1), timer_id(0), is_Spider(false) {
    this->setWindowFlags(Qt::WindowFlags { Qt::Window | Qt::WindowStaysOnTopHint } );

    this->setGeometry(0, 0, 450, 400);
    this->setMaximumSize(450, 450);
    this->setWindowTitle(QStringLiteral("3. Бегущий паучок"));
    const auto desktop_center = QApplication::desktop()->screenGeometry().center();
    this->move(desktop_center.x() - 460, desktop_center.y() + 30);

    spider_way = QPixmap{450, 450};
    spider_way.fill(Qt::white);
    spider_way.setMask(QBitmap(spider_way));
}

Spider::~Spider() {
}

void Spider::keyPressEvent(QKeyEvent* event){
    if(event->key() == Qt::Key_S && !is_Spider){
        is_Spider = true;
        spider_pos.setX(0);
        spider_pos.setY(0);
        spider_direct.setX(1);
        spider_direct.setY(1);
        timer_id = startTimer(1); // стартуем таймер каждые 1мс
    }
}

void Spider::keyReleaseEvent(QKeyEvent* event){
    if(event->key() == Qt::Key_E && is_Spider){
        is_Spider = false;
        killTimer(timer_id);
        spider_way.fill(Qt::white); // очищаем и обновляем фон
        spider_way.setMask(QBitmap(spider_way));
        QWidget::repaint();
    }
}

void Spider::timerEvent(QTimerEvent*) {
    repaint();
}

void Spider::PaintingSpider(){
    QRect client_area = this->rect();
    QPoint next_pos{spider_pos.x() + spider_direct.x(), spider_pos.y() + spider_direct.y()};
    // корректируем направление паучка
    if(client_area.left() == next_pos.x()){
        spider_direct.setX(1);
    }
    if(client_area.right() == next_pos.x()){
        spider_direct.setX(-1);
    }
    if(client_area.top() == next_pos.y()){
        spider_direct.setY(1);
    }
    if(client_area.bottom() == next_pos.y()){
        spider_direct.setY(-1);
    }
    next_pos.setX(spider_pos.x() + spider_direct.x());
    next_pos.setY(spider_pos.y() + spider_direct.y());

    QPainter way_painter(&spider_way);
    way_painter.drawLine(spider_pos, next_pos); // дорисовываем кусочек пути
    QPainter painter(this);
    painter.drawPixmap(client_area.topLeft(), spider_way); // выводим картинку с пройденным путем
    painter.drawPixmap(spider_pos, spider); // и сам паучок

    spider_pos = next_pos; // двигаем позицию
}

void Spider::paintEvent(QPaintEvent*) {
    if(is_Spider){
        PaintingSpider();
    }
}
