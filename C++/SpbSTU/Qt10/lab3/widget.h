#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLayout>
#include <QLineEdit>
#include <QSlider>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QComboBox>

class Widget : public QWidget
{
    Q_OBJECT
    QSpinBox* _spin_box;
    QSlider* _slider;
    QLineEdit* _line_edit;
    QLabel* _label;
    QFrame* _frame;

    QComboBox* _combo_box;
    QPushButton* _fusion_button;
    QPushButton* _disable_button;

    QHBoxLayout* createHBoxLayout() const;
    QVBoxLayout* createVBoxLayout() const;
    QGridLayout* createGridLayout() const;

    void connectObjects(QSpinBox*, QSlider*, QLineEdit*, QLabel*) const;
signals:
    void mySignal(int);
private slots:
    void stringToInt(QString);
    void changeLayout(int);
    void disable_enable();
    void fusion();
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
};
#endif // WIDGET_H
