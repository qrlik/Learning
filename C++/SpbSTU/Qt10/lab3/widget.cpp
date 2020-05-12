#include "widget.h"

#include <QApplication>
#include <QStyleFactory>
#include <QFrame>
#include <QSizePolicy>
#include <QDebug>

namespace {
    QSpinBox* createSpinBox(){
        QSpinBox* spin_box = new QSpinBox();
        spin_box->setRange(0,50);
        return spin_box;
    }

    QSlider* createSlider(){
        QSlider* slider = new QSlider(Qt::Horizontal);
        slider->setRange(0, 50);
        slider->setTickPosition(QSlider::TicksBelow);
        slider->setTickInterval(5);
        slider->setMinimumWidth(200);
        return slider;
    }

    QLineEdit* createLineEdit() {
        QLineEdit* line_edit = new QLineEdit();
        QIntValidator* int_validator = new QIntValidator(0, 50, line_edit);
        line_edit->setValidator(int_validator);
        return line_edit;
    }

    QLabel* createLabel(){
        QLabel* label = new QLabel(QStringLiteral("not set"));
        label->setFrameStyle(QFrame::Box | QFrame::Raised);
        label->setLineWidth(0);
        label->setMidLineWidth(1);
        label->setAlignment(Qt::AlignCenter);
        label->setFixedHeight(label->sizeHint().height());
        return label;
    }

    // buttons
    QComboBox* createComboBox(Widget* widget){
        QComboBox* combo_box = new QComboBox(widget);
        combo_box->addItem(QStringLiteral("Horizontal layout"));
        combo_box->addItem(QStringLiteral("Vertical layout"));
        combo_box->addItem(QStringLiteral("Grid layout"));
        if(!widget->connect(combo_box, SIGNAL(activated(int)), SLOT(changeLayout(int)))){
            qDebug() << "Connection failed in createComboBox";
        }
        return combo_box;
    }

    QPushButton* createFusionStyle(Widget* widget){
        QPushButton* fusion_button = new QPushButton(QStringLiteral("Fusion style"), widget);
        auto fusion_connection = widget->connect(fusion_button, SIGNAL(clicked()), SLOT(fusion()));
        if(!fusion_connection){
            qDebug() << "No fusion connection";
        }
        return fusion_button;
    }

    QPushButton* createDisable(Widget* widget){
        QPushButton* disable_button = new QPushButton(QStringLiteral("Disable"), widget);
        auto disable_connection = widget->connect(disable_button, SIGNAL(clicked()), SLOT(disable_enable()));
        if(!disable_connection){
            qDebug() << "No disable connection";
        }
        return disable_button;
    }

    QPushButton* createExit(Widget* widget){
        QPushButton* exit_button = new QPushButton(QStringLiteral("Exit"), widget);
        auto close_connection = widget->connect(exit_button, SIGNAL(clicked()), SLOT(close()));
        if(!close_connection){
            qDebug() << "No close connection";
        }

        return exit_button;
    }

}

Widget::Widget(QWidget *parent): QWidget(parent), _spin_box(createSpinBox()),
    _slider(createSlider()), _line_edit(createLineEdit()), _label(createLabel()) {
    this->setWindowTitle(QStringLiteral("SignalSlot_2020"));
    this->setWindowFlags(Qt::WindowFlags {Qt::CustomizeWindowHint | Qt::WindowTitleHint});

    QHBoxLayout* top_layout = new QHBoxLayout(this);

    this->connectObjects(_spin_box, _slider, _line_edit, _label);
    _frame = new QFrame(this);
    _frame->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    _frame->setLayout(createHBoxLayout());

    _combo_box = createComboBox(this);
    _fusion_button = createFusionStyle(this);
    _disable_button = createDisable(this);

    // Layout for buttons
    QVBoxLayout* buttons_layout = new QVBoxLayout();
    buttons_layout->addWidget(_combo_box);
    buttons_layout->addWidget(_fusion_button);
    buttons_layout->addWidget(_disable_button);
    buttons_layout->addWidget(createExit(this));

    top_layout->addWidget(_frame);
    top_layout->addLayout(buttons_layout);
    _spin_box->setValue(1);
}

Widget::~Widget() {
}

// layouts for frame
QHBoxLayout* Widget::createHBoxLayout() const{
    _spin_box->setFixedWidth(35);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(_spin_box);
    layout->addWidget(_slider);
    layout->addWidget(_line_edit);
    layout->addWidget(_label);
    return layout;
}

QVBoxLayout* Widget::createVBoxLayout() const{
    _spin_box->setMinimumWidth(35);
    _spin_box->setMaximumWidth(QWIDGETSIZE_MAX);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(_spin_box, 0, Qt::AlignHCenter);
    //layout->addWidget(_slider, 0, Qt::AlignHCenter);
    layout->addWidget(_slider);
    layout->addWidget(_line_edit);
    layout->addWidget(_label, 0, Qt::AlignHCenter);
    return layout;
}

QGridLayout* Widget::createGridLayout() const {
    _spin_box->setMinimumWidth(35);
    _spin_box->setMaximumWidth(QWIDGETSIZE_MAX);

    QGridLayout* layout = new QGridLayout();
    layout->addWidget(_spin_box, 0, 0);
    layout->addWidget(_slider, 0, 1);
    layout->addWidget(_line_edit, 1, 0);
    layout->addWidget(_label, 1, 1);
    return layout;
}

void Widget::connectObjects(QSpinBox* spin_box, QSlider* slider,
                            QLineEdit* line_edit, QLabel* label) const {
    //spin_box -> all
    auto box_slider = slider->connect(spin_box, SIGNAL(valueChanged(int)), SLOT(setValue(int)));
    //auto box_slider = connect(spin_box, qOverload<int>(&QSpinBox::valueChanged), slider, &QSlider::setValue);
    auto box_line = line_edit->connect(spin_box, SIGNAL(valueChanged(QString)), SLOT(setText(QString)));
    auto box_label = label->connect(spin_box, SIGNAL(valueChanged(int)), SLOT(setNum(int)));

    // slider -> spin_box
    auto slider_box = spin_box->connect(slider, SIGNAL(valueChanged(int)), SLOT(setValue(int)));

    // line_edit -> spin_box
    auto line_this = this->connect(line_edit, SIGNAL(textChanged(QString)), SLOT(stringToInt(QString)));
    auto this_spin = spin_box->connect(this, SIGNAL(mySignal(int)), SLOT(setValue(int)));

    if(!(box_slider || box_line || box_label ||
         slider_box || line_this || this_spin)){
        qDebug() << "Something wrong in connectObjects function";
    }
}

void Widget::stringToInt(QString str){
    mySignal(str.toInt());
}

void Widget::changeLayout(int value){
    QLayout* old_layout = _frame->layout();
    delete old_layout;
    _frame->hide();
    switch (value) {
    case 0:
        _frame->setLayout(createHBoxLayout());
        break;
    case 1:
        _frame->setLayout(createVBoxLayout());
        break;
    case 2:
        _frame->setLayout(createGridLayout());
        break;
    }
    _frame->show();
}

void Widget::disable_enable(){
    if(_frame->isEnabled()){
        _frame->setEnabled(false);
        _combo_box->setEnabled(false);
        _disable_button->setText(QStringLiteral("Enabled"));
    } else {
        _frame->setEnabled(true);
        _combo_box->setEnabled(true);
        _disable_button->setText(QStringLiteral("Disabled"));
    }
}

void Widget::fusion(){
    if(_fusion_button->text() == QStringLiteral("Fusion style")){
        QApplication::setStyle(QStyleFactory::create(QStringLiteral("Fusion")));
        _fusion_button->setText(QStringLiteral("Windows style"));
    } else {
        QApplication::setStyle(QStyleFactory::create(QStringLiteral("Windowsvista")));
        _fusion_button->setText(QStringLiteral("Fusion style"));
    }
}

