#ifndef SPIDER_H
#define SPIDER_H

#include <QWidget>

class Spider : public QWidget
{
    Q_OBJECT

public:
    Spider(QWidget* parent = nullptr);
    Spider(const QColor& background_color, const QString& background_path,
           QWidget* parent = nullptr);
    ~Spider();
};
#endif // SPIDER_H
