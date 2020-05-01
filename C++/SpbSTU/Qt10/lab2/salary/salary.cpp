#include "salary.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>

Salary::Salary(QWidget *parent) : QWidget(parent), no_button(QStringLiteral("Нет"), this),
    yes_button(QStringLiteral("Да"), this) {
    this->setWindowFlags(Qt::WindowFlags{ Qt::Dialog | Qt::WindowStaysOnTopHint });

    this->setGeometry(0, 0, 450, 400);
    this->setFixedSize(450, 400);
    this->setWindowTitle(QStringLiteral("4. Вас устраивает зарплата?"));
    const auto desktop_center = QApplication::desktop()->screenGeometry().center();
    this->move(desktop_center.x(), desktop_center.y() + 30);

    no_button.move(80, 180);
    no_button.installEventFilter(this);
    yes_button.move(300, 180);
    yes_button.installEventFilter(this);
}

Salary::~Salary(){

}

namespace {
    QPoint getRandomPoint(QSize rect){
        return QPoint{rand() % rect.width(), rand() % rect.height()};
    }
}

bool Salary::eventFilter(QObject *watched, QEvent *event){
    if(watched == &yes_button) {
        if(event->type() == QEvent::Type::MouseButtonPress &&
                static_cast<QMouseEvent*>(event)->button() == Qt::MouseButton::LeftButton){
            //yes_button.isDown()
            QMessageBox::information(this, QStringLiteral("message"), QStringLiteral("Мы так и думали"));
            return true;
        }
        return false;
     }
    if(watched == &no_button){
        if(event->type() == QEvent::Type::Enter){
            auto enter_event = static_cast<QEnterEvent*>(event);

            QSize main_size = this->size();
            main_size.setWidth(main_size.width() / 2);

            QPoint enter_pos = enter_event->globalPos();
            while(no_button.rect().contains(no_button.mapFromGlobal(enter_pos))){
                no_button.move(getRandomPoint(main_size));
            }
            return true;
        }
        return false;
    }
    QWidget::eventFilter(watched, event);
}
