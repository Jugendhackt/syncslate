using namespace std;

#include "firstwidget.h"
#include "ui_firstwidget.h"
#include <QFileDialog>
#include <QString>
#include <iostream>

FirstWidget::FirstWidget(QWidget *parent):
    // Super constructor
    QWidget(parent),
    // Constructor
    ui(new Ui::FirstWidget) {
        ui -> setupUi(this); // -> replacement for dot notation (dynamically)
    }


// Destructor
FirstWidget::~FirstWidget() {
    delete ui;
}

void FirstWidget::on_pushButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open Video File"),
        "~",
        tr("Video files (*.mov *.avi *.mpeg4 *.mp4)")
    );

    cout << fileName.toStdString();
}
