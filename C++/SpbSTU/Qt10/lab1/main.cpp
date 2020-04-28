#include "spider.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QColorDialog>
#include <QFileDialog>

const QColor default_active_background_color = Qt::darkGreen;
const QString default_inactive_background_picture = QStringLiteral("Images_lab1/WomanAndDog.jpg");

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
//    Задание 9.1. Проверьте работу переводчика
    QTranslator translator; // создаем переводчик
    translator.load(QLocale::system(), QStringLiteral("spider"), //формирование спецификации файла возложим на QTranslator
                    QStringLiteral("_"), QStringLiteral("."));
    QApplication::installTranslator(&translator);

//    Задание 9.2. 5.	Перевод текста в стандартных диалогах
    QString filespec = QStringLiteral("qt_") + QLocale::system().name(); // сформировать спецификацию имеющегося файла  .qm
    QString directory = QLibraryInfo::location(QLibraryInfo::TranslationsPath); // сформировать директорию этого файла
    QTranslator std_translator;
    std_translator.load(filespec, directory);
    QApplication::installTranslator(&std_translator);

//    Задание 8.1. Используйте диалог выбора цвета фона
    QColor background_color = QColorDialog::getColor(Qt::white, nullptr,
                    QObject::tr("Choose active background color"));
    if(!background_color.isValid()) {
        background_color = default_active_background_color;
    }

//    Задание 8.2. Используйте диалог выбора файла для определения имени файла картинки
    QString background_picture_path = QFileDialog::getOpenFileName(nullptr,
            QObject::tr("Choose inactive background picture"),
            QStringLiteral("./Images_lab1"),
            QStringLiteral("*.jpg *.png *.bmp"));
    if(background_picture_path.isEmpty()){
        background_picture_path = default_inactive_background_picture;
    }

    Spider window(background_color, background_picture_path);
    window.show();

//    Задание 2.
//    w.showMaximized();

    return app.exec();
}
