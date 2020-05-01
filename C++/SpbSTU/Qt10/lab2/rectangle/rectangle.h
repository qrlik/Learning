#ifndef RECTANGE_H
#define RECTANGE_H

#include <QWidget>
#include <QMouseEvent>
#include <QPixmap>

//Нарисуйте на виджете прямоугольник следующим образом: 
//1)	при нажатии правой  кнопки мыши определяются координаты одной из вершин.  
//2)	При перемещении мыши (при нажатой кнопке) определяются координаты противоположной вершины и выполняется отрисовка текущего прямоугольника. 
//3)	Рисование прямоугольника завершается, когда кнопка мыши отжимается.
//Попытайтесь доработать программу  таким образом, чтобы отображались все нарисованные прямоугольник, а не только самый последний.
                                          
class Rectangle : public QWidget {
    Q_OBJECT
    QPixmap rectangles;
    QVector<QRect> rects_cont;
    QPoint rect_point_f;
    QPoint rect_point_s;
    bool is_Drawing;

    void PaintingRect();
protected:
    virtual void mousePressEvent(QMouseEvent*) override;
    virtual void mouseReleaseEvent(QMouseEvent*) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
public:
    explicit Rectangle(QWidget* parent = nullptr);
    virtual ~Rectangle();
};

#endif // RECTANGE_H
