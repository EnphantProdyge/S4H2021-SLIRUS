#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "label.h"

#include <QPushButton>
#include <QMessageBox>
#include <QLabel>
#include <QProcess>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QGraphicsScene>
#include <thread>
#include <mutex>
#include <condition_variable>

std::thread com_;
std::mutex mutex;
std::condition_variable cv_;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Phrase_recorded->setText(" ");
    ui->Recording_state->setText(" "); //set labels to a space in initialisation

    Recording_increment = 1;
    Letter_increment = 1;
    connect(ui->Start_record_button, SIGNAL(pressed()), this, SLOT(Start_Thread()));
    //When button Start_record_button is pressed in interface, Start_thread is executed
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene; //scene created in Display_image()
}

void MainWindow::SendStrToOpenCR() //Function sends string to openCR and plots the right character in interface
{
    //qDebug() << "Message envoye: " << trad_string << endl;
    if (trad_string != NULL)
    {
        QString path = "/home/pi/env/SLIRUS_interface/FindPort.py";
        QProcess *process = new QProcess();
        connect(process, &QProcess::readyReadStandardOutput, [process, this]()
        {
            QString output = process->readAllStandardOutput();
            const char* chartemp= MainWindow::QStringtoChar(output);
            QString qstemp = output;
            if (startsWith("Interface:next", chartemp) == true)
            {
                qstemp.remove(0,Letter_increment);
                qstemp.remove(1,qstemp.length());
                //MainWindow::Display_image(qstemp,true);
                Letter_increment = Letter_increment + 1;
            }


            else if (startsWith("Interface:stop", chartemp) == true)
            {
                Letter_increment = 1;
                //MainWindow::Display_image(" ",false);
            }

            else
            {
                OutputOpenCR = output;
                qDebug() << "output: " << output;
                output.remove(1,output.length());
                //MainWindow::Display_image(output,true);
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
                process->write((trad_string + QString("\n")).toLatin1());
        });
        process->start("/usr/bin/python3", QStringList() << path);

        process->waitForFinished();
        process->close();
        ui->Start_record_button->setEnabled(true);
        ui->See_charac->setEnabled(true);
        ui->Recording_time->setEnabled(true);
    }
    else
    {
        MainWindow::MessageBoxError("No phrase was recorded");
    }
}

void MainWindow::Record_sequence() //Function that records the speech-to-text
{
    int Record_time = MainWindow::GetRecordingTime();

    if (Record_time == -1)
        QCoreApplication::exit();

    else {
        //Commencement de l'enregistrement
        QString path = "/home/pi/Documents/Projets_S4/Enregistrement.py";
        //Modify previous line for a different system
        QProcess *process = new QProcess();
        connect(process, &QProcess::readyReadStandardOutput, [process, this]()
        {
            QString output = process->readAllStandardOutput();
            mutex.lock();
            OutputSpeech = output;
            mutex.unlock();
            qDebug() << "output: " << output;
            if (Recording_increment != -1)
            {
                Display_Recording_Labels();
                Recording_increment = Recording_increment - 1;
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
        process->start("/usr/bin/python3", QStringList() << path);

        process->waitForFinished();
        process->close();
        ui->Start_traduction->setEnabled(true);
        ui->See_charac->setEnabled(true);
        ui->Recording_time->setEnabled(true);
        ui->transcript_charac->setEnabled(true);
        ui->Start_record_button->setEnabled(true);
        Message_toTranscript();
    }


}

int MainWindow::GetRecordingTime() //Gets time from the text line edit in the interface. Base time is 5 seconds
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
        MainWindow::MessageBoxError("You did not write an appropriate recording time");
        time_ = -1;
        return -1;
    }
}


void MainWindow::Message_toTranscript() //Separates the receiving string to keep only the real message
{
    Label label;
    label.Display_Recording_Labels();
    if (OutputError != NULL)
    {
        com_.join();
        MainWindow::MessageBoxError("No words were recorded");
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
        Recording_increment = 1;
    }
}

void MainWindow::Display_image(QString lettre, bool choice)
{
    qDebug() << "lettre a display: " << lettre << endl;
    //Displays the image related to the character in the QGraphicView
    mutex.lock();
    if (choice)
    {
        QString qstr_temp = "/home/pi/env/SLIRUS_interface/Images/";
        qstr_temp.append(lettre);
        qstr_temp.append(".png");
        image.load(qstr_temp);
        image.scaled(400,400);
        scene = new QGraphicsScene(this);
        scene->addPixmap(image);
        scene->setSceneRect(image.rect());
        ui->MainWindow::mainImage->setScene(scene);
    }
    else
        scene->clear();
    mutex.unlock();
}

void MainWindow::MessageBoxError(QString message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
}

void MainWindow::on_Start_traduction_pressed() //Button to start traduction
{
    if (com_.joinable())
        com_.join();
    ui->Start_traduction->setEnabled(false);
    ui->See_charac->setEnabled(false);
    ui->Recording_time->setEnabled(false);
    ui->transcript_charac->setEnabled(false);
    ui->transcript_charac->setEnabled(false);
    //ui->Start_record_button->setEnabled(false);

    trad_string = OutputSpeech;
    com_ = std::thread(&MainWindow::SendStrToOpenCR,this);
}

void MainWindow::on_See_charac_returnPressed() //when somebody press "enter" to see a specific character on screen
{
    QString str = "all";
    if(ui->See_charac->text() == str)
        MainWindow::Display_image("Complete_image", true);
    else if (ui->See_charac->text().length() > 1)
        MainWindow::MessageBoxError("The line has more than one character");
    else
        MainWindow::Display_image(ui->See_charac->text(), true);
}

const char* MainWindow::QStringtoChar(QString qs)
{
    std::string utf8_text = qs.toUtf8().constData();
    const char* temp_char = utf8_text.c_str();
    return temp_char;
}

void MainWindow::Start_Thread() //this function is called when the user is ready to use the speech-to-text
{
    ui->Start_traduction->setEnabled(false);
    ui->See_charac->setEnabled(false);
    ui->Recording_time->setEnabled(false);
    ui->transcript_charac->setEnabled(false);
    ui->transcript_charac->setEnabled(false);
    ui->Start_record_button->setEnabled(false);
    OutputSpeech = "";
    if (com_.joinable())
        com_.join();

    com_ = std::thread(&MainWindow::Record_sequence,this);
}


void MainWindow::on_transcript_charac_returnPressed() //when escape is pressed to transcript only certain characters
{
    ui->Start_record_button->setEnabled(false);
    ui->See_charac->setEnabled(false);
    ui->Recording_time->setEnabled(false);
    if (com_.joinable())
        com_.join();

    trad_string = ui->transcript_charac->text();
    com_ = std::thread(&MainWindow::SendStrToOpenCR,this);
}

bool MainWindow::startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre);
    size_t lenstr = strlen(str);
    return lenstr < lenpre ? false : memcmp(pre,str,lenpre) == 0;
}

void MainWindow::Display_Recording_Labels() //displays a label when the system is ready to record
{
    if (MainWindow::Recording_increment == 1)
         ui->Recording_state->setText("Now Recording...");

     else if (MainWindow::Recording_increment == 0)
         ui->Recording_state->setText("Stopped recording");
}
