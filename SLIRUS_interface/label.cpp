#include "label.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"

#include <QLabel>
#include <QDebug>


Label::Label()
{

}

Label::~Label()
{

}

void Label::Set_label_visible(QLabel *label)
{
   label->setVisible(true);
}

void Label::Set_label_invisible(QLabel *label)
{
   label->setVisible(false);
}

void Label::Display_Recording_Labels()
{
    qDebug() << MainWindow::Recording_increment << endl;
    if (MainWindow::Recording_increment == 1)
         ui->Recording_state->setText("Now Recording...");

     else if (MainWindow::Recording_increment == 0)
         ui->Recording_state->setText("Stopped recording");
}

bool Label::startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre);
    size_t lenstr = strlen(str);
    return lenstr < lenpre ? false : memcmp(pre,str,lenpre) == 0;
}
