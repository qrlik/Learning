#ifndef SALARY_H
#define SALARY_H

#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>

//При нажатии на кнопку «Да»  выводится   информационное сообщение QMessageBox с текстом “Мы так и думали”.
//При наведении на кнопку «Нет» изменяются  ее координаты расположения – кнопка «убегает от мышки».  


class Salary : public QWidget
{
    Q_OBJECT
    QPushButton no_button;
    QPushButton yes_button;
public:
    explicit Salary(QWidget *parent = nullptr);
    virtual ~Salary();

    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // SALARY_H
