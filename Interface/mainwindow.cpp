#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>
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
    //ui->Recording_state->set

    Recording_increment = 1;
    connect(ui->Start_record_button, SIGNAL(pressed()), this, SLOT(Start_Thread()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SendStrToOpenCR()
{
    if (OutputSpeech != NULL)
    {
        QString path = "/home/pi/env/SLIRUS_interface/FindPort.py";
        QProcess *process = new QProcess();
        connect(process, &QProcess::readyReadStandardOutput, [process, this]()
        {
            QString output = process->readAllStandardOutput();
            OutputOpenCR = output;
            const char* chartemp= MainWindow::QStringtoChar(OutputOpenCR);
            if (MainWindow::startsWith("Interface:", chartemp) == true)
            {
                QString qstemp = OutputOpenCR;
                qstemp.remove(0,10);
                chartemp= MainWindow::QStringtoChar(qstemp);

                if (MainWindow::startsWith("debut:", chartemp) == true)
                {
                    qstemp.remove(0,6);
                    Display_image(qstemp);
                }
                else if (MainWindow::startsWith("fin:", chartemp) == true)
                {
                    qstemp.remove(0,4);
                    //Erase_image();
                }
            }
            else if (MainWindow::startsWith("stop", chartemp) == true)
                    QCoreApplication::exit();
            qDebug() << "output: " << output;
        });

        connect(process, &QProcess::readyReadStandardError, [process, this]()
        {
            QString err = process->readAllStandardError();
            qDebug() << "error: " << err;
            OutputError = err;
        });

        connect(process, &QProcess::started, [process, this]()->void
        {
            process->write((QString(OutputSpeech) + QString("\n")).toLatin1());
        });
        process->start("/usr/bin/python3", QStringList() << path);

        process->waitForFinished();
        process->close();
    }
    else
    {
        MainWindow::MessageBoxError("No phrase was recorded");
    }
}

void MainWindow::Record_sequence()
{
    int Record_time = MainWindow::GetRecordingTime();

    if (Record_time == -1)
        QCoreApplication::exit();

    else {
        //Commencement de l'enregistrement
        QString path = "/home/pi/Documents/Projets_S4/Enregistrement_test.py";
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
        Message_toTranscript();
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
        MainWindow::MessageBoxError("You did not write an appropriate recording time");
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

void MainWindow::Message_toTranscript()
{
    Display_Recording_Labels();
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

void MainWindow::Display_image(QString lettre)
{
    QString qstr_temp = "/home/pi/env/SLIRUS_interface/Images/";
    qstr_temp.append(lettre);
    qstr_temp.append(".png");
    qDebug() << qstr_temp << endl;
    image.load(qstr_temp);
    image.scaled(400,400);
    scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->MainWindow::mainImage->setScene(scene);

}

void MainWindow::MessageBoxError(QString message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
}

void MainWindow::on_Start_traduction_pressed()
{
    if (com_.joinable())
        com_.join();

    com_ = std::thread(&MainWindow::SendStrToOpenCR,this);
    MainWindow::SendStrToOpenCR();
}

bool MainWindow::startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre);
    size_t lenstr = strlen(str);
    return lenstr < lenpre ? false : memcmp(pre,str,lenpre) == 0;
}

void MainWindow::on_See_charac_returnPressed() //when somebody press "enter" in the lineEdit
{
    QString str = "all";
    if(ui->See_charac->text() == str)
        MainWindow::Display_image("Complete_image");
    else if (ui->See_charac->text().length() > 1)
        MainWindow::MessageBoxError("The line has more than one character");
    else
        MainWindow::Display_image(ui->See_charac->text());
}

const char* MainWindow::QStringtoChar(QString qs)
{
    std::string utf8_text = qs.toUtf8().constData();
    const char* temp_char = utf8_text.c_str();
    return temp_char;
}

void MainWindow::Start_Thread() //1 to start Record_Sequence and 2 for SendStrtoOpenCR
{
    OutputSpeech = "";
    if (com_.joinable())
        com_.join();

    com_ = std::thread(&MainWindow::Record_sequence,this);
}

void MainWindow::Display_Recording_Labels()
{
    qDebug() << Recording_increment << endl;
    if (Recording_increment == 1)
         ui->Recording_state->setText("Now Recording...");

     else if (Recording_increment == 0)
         ui->Recording_state->setText("Stopped recording");
}
