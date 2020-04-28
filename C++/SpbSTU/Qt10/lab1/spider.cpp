#include "spider.h"
#include <QIcon>
#include <QPixmap>
#include <QBitmap>
#include <QApplication>
#include <QDesktopWidget>

Spider::Spider(QWidget* parent) : QWidget(parent) {
    //    Задание 1.
        this->setWindowFlags(Qt::WindowFlags { Qt::Window | Qt::WindowStaysOnTopHint } );

    //    Задание 2. Поэкспериментируйте с состоянием главного окна
    //    this->setWindowState(Qt::WindowFullScreen);
    //    this->setWindowState(Qt::WindowMaximized);
    //    this->setWindowState(Qt::WindowMinimized);

    //    Задание 3. Установите заголовок окна
        this->setWindowTitle(QObject::tr("First studywork"));

    //    Задание 4.1. Установите пиктограмму приложения в Windows
    //    this->setWindowIcon(QIcon(QStringLiteral("Images_lab1/icon2.ico")));

    //    Задание 5.1. Сделайте фон окна разным в активном состоянии и в неактивном
    //    QPalette new_pallete = this->palette();
    //    new_pallete.setColor(QPalette::Active, QPalette::Window, Qt::darkGreen);
    //    new_pallete.setColor(QPalette::Inactive, QPalette::Window, Qt::white);
    //    this->setPalette(new_pallete);
}

Spider::Spider(const QColor& background_color, const QString& background_path,
               QWidget *parent) : Spider(parent) {
//    Задание 5.2. задайте картинку в качестве фона окна (лучше для неактивного состояния)
    QPixmap window_pic(background_path);
    QPalette window_palette;
    window_palette.setBrush(QPalette::Inactive, QPalette::Window, QBrush(window_pic));
    window_palette.setColor(QPalette::Active, QPalette::Window, background_color);
    this->setPalette(window_palette);

//    Задание 5.3. сделайте фон окна полупрозрачным
//    this->setWindowOpacity(0.5);

//    Задание 6.1. В нашей заготовке размеры и позиция окна задаются по умолчанию ОС.Задайте эти значения самостоятельно посредством перечисленных выше методов
    auto window_pic_size = window_pic.size();
    this->setGeometry(0, 0, window_pic_size.width(), window_pic_size.height());

//    Задание 6.2. расположите окно в любой четверти экрана
    const auto desktop_geometry = QApplication::desktop()->screenGeometry();
    this->move(desktop_geometry.center());

//    Задание 6.3. задайте минимальный/максимальный размер окна
    this->setMaximumSize(window_pic_size.width(), window_pic_size.height());

//    Задание 6.4. задайте фиксированный размер окна 
//    this->setFixedSize(window_pic_size);

//    Задание 7.1. замените стандартную стрелку на любой из предоставляемых Qt курсоров
//    this->setCursor(Qt::BusyCursor);

//    Задание 7.2. Измените вид курсора
    QString cursor_path = QStringLiteral("Images_lab1/cursor1.cur");
    QPixmap window_cursor(cursor_path);
    window_cursor.setMask(QBitmap(cursor_path));
    this->setCursor(QCursor(window_cursor));
}

Spider::~Spider()
{
}
