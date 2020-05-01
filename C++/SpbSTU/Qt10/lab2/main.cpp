#include "web.h"
#include "rectange.h"
#include "spider.h"
#include "salary.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QColorDialog>
#include <QFileDialog>
#include <QBitmap>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Web web;
    web.show();
    Rectangle rectangle;
    rectangle.show();
    Spider spider;
    spider.show();
    Salary salary;
    salary.show();

    return app.exec();
}
