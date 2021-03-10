#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>
#include <QProcess>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Phrase_recorded->setText(" ");

    connect(ui->Start_record_button, SIGNAL(pressed()), this, SLOT(Record_sequence()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Record_sequence()
{
    Recording_increment = 1;
    int Record_time = MainWindow::GetRecordingTime();
    if (Record_time == -1)
        QCoreApplication::exit();

    else {
        //Commencement de l'enregistrement
        QString path = "/home/pi/Documents/Projets_S4/Enregistrement_test.py";
        QProcess *process = new QProcess();
        connect(process, &QProcess::readyReadStandardOutput, [process, this]()
        {
            QMessageBox msgBox;
            QString output = process->readAllStandardOutput();
            OutputSpeech = output;
            qDebug() << "output: " << output;
            if (Recording_increment == 1)
            {

                msgBox.setText("Now Recording...");
                msgBox.exec();
                Recording_increment = 0;
            }
        });

        connect(process, &QProcess::readyReadStandardError, [process, this]()
        {
            QString err = process->readAllStandardError();
            qDebug() << "error: " << err;
            OutputError = err;
        });

        connect(process, &QProcess::started, [process, this]()->void
        {
            char numberstring[(((sizeof time_) * CHAR_BIT) + 2)/3 + 2];
            sprintf(numberstring, "%d", time_);
            process->write((QString(numberstring) + QString("\n")).toLatin1());
        });
        //process->setWorkingDirectory("/home/pi/env/bin");
        process->start("/usr/bin/python3", QStringList() << path);

        process->waitForFinished();
        qDebug() << "patate" << endl;
        process->close();
        Message();
    }


}

int MainWindow::GetRecordingTime()
{
    if (ui->Recording_time->text().toInt() > 0)
    {
        time_ = ui->Recording_time->text().toInt();
        return ui->Recording_time->text().toInt();
    }
    else if (ui->Recording_time->text().isEmpty())
    {
        time_ = 5;
        return 5;
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("You did not write an appropriate recording time");
        msgBox.exec();
        time_ = -1;
        return -1;
    }
}

void MainWindow::Set_label_visible(QLabel *label)
{
   label->setVisible(true);
}

void MainWindow::Set_label_invisible(QLabel *label)
{
   label->setVisible(false);
}

void MainWindow::on_Start_record_button_clicked()
{

}

void MainWindow::Message()
{
    if (OutputError != NULL)
    {
        QMessageBox msgBox;
        msgBox.setText("No words were recorded");
        msgBox.exec();
    }
    else
    {
        OutputSpeechList = OutputSpeech.split(QRegExp("\\b"));
        int index = OutputSpeechList.QStringList::lastIndexOf(QString("\n"),-2);
        OutputSpeech.clear();
        for (int i=0;i<=index;i++)
        {
            OutputSpeechList.QStringList::removeFirst();
        }
        OutputSpeechList.QStringList::removeLast();
        OutputSpeech = OutputSpeechList.QStringList::join("");
        ui->Phrase_recorded->setText(OutputSpeech);
        qDebug() << OutputSpeech << endl;
    }
}
