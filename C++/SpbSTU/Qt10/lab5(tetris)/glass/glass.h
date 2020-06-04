#ifndef GLASS_H
#define GLASS_H
#include "figure.h"
#include <QWidget>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QTimerEvent>

class Glass : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(uint rows READ rows WRITE setRows)
    Q_PROPERTY(uint columns READ columns WRITE setColumns)

    QVector<QVector<QColor>> _array;

    uint _rows;
    uint _columns;
    int _score;
    int _timerID;
    Figure* _cur;
    Figure* _next;
    uint16_t _timer_step; //мс

    bool _is_GameOn;
    void clear();
    bool push_cur_down();
    void move_left();
    void move_right();
    void move_above(uint y, uint x1, uint x2, uint range);
    bool checkColors();

    protected:
    virtual void paintEvent(QPaintEvent*) override;
    virtual void keyPressEvent(QKeyEvent*) override;
    virtual void timerEvent(QTimerEvent*) override;

    public:
    explicit Glass(QWidget *parent = nullptr);
    virtual ~Glass();
    uint rows() const;
    uint columns() const;

    signals:
    void signalGlassInit();
    void signalNextFigure(Figure* next);
    void signalSizeComputed();
    void signalScore(int);

    private slots:
    void setRows(uint rows);
    void setColumns(uint columns);
    void slotGlassInit();
    public slots:
    void slotNewGame();
};

#endif // GLASS_H
