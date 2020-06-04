#ifndef NEXTFIGURE_H
#define NEXTFIGURE_H
#include "figure.h"
#include <QWidget>
#include <QPaintEvent>

class NextFigure : public QWidget {
    Q_OBJECT
    Figure* _next;
protected:
    virtual void paintEvent(QPaintEvent*) override;
public:
    explicit NextFigure(QWidget *parent = nullptr);

signals:

public slots:
    void slotUpdateFigure(Figure*);
};

#endif // NEXTFIGURE_H
