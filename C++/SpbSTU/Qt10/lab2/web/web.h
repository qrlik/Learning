#ifndef WEB_H
#define WEB_H

#include <QWidget>
#include <QMouseEvent>
#include <QPixmap>

//Нарисуйте на виджете паутину. Особенность паутины в том, что центр ее должен находиться в позиции курсора,
//а вторые концы отрезков паутины  могут располагаться по краям прямоугольника рабочей области.
//Причем при изменении позиции курсора паутина должна перерисовываться.  

class Web : public QWidget
{
    Q_OBJECT
    QPixmap spider;
    QCursor* default_cursor;
    QCursor* drawing_cursor;
    bool is_Drawing;

    void PaintingWeb();
protected:
    virtual void mousePressEvent(QMouseEvent*) override;
    virtual void mouseReleaseEvent(QMouseEvent*) override;
    virtual void mouseMoveEvent(QMouseEvent*) override;

    virtual void paintEvent(QPaintEvent*) override;
    virtual void resizeEvent(QResizeEvent*) override;
public:
    explicit Web(QWidget *parent = nullptr);
    virtual ~Web();
};

#endif // WEB_H
