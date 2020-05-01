#ifndef SPIDER_H
#define SPIDER_H

#include <QWidget>
#include <QKeyEvent>
#include <QTimerEvent>
#include <QPixmap>

//Реализуйте передвижение паучка по окну. 
//Начинать  и останавливать движение можно по событию нажатия клавиши
//Попробуйте отображать траекторию движения паучка. 

class Spider : public QWidget
{
    Q_OBJECT
    QPixmap spider;
    QPixmap spider_way;
    QPoint spider_pos;
    QPoint spider_direct;
    int timer_id;
    bool is_Spider;

    void PaintingSpider();
protected:
    virtual void keyPressEvent(QKeyEvent*) override;
    virtual void keyReleaseEvent(QKeyEvent*) override;
    virtual void timerEvent(QTimerEvent*) override;

    virtual void paintEvent(QPaintEvent*) override;
public:
    explicit Spider(QWidget* parent = nullptr);
    virtual ~Spider();
};
#endif // SPIDER_H
