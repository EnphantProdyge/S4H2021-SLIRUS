#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QStringList>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int GetRecordingTime();
    void Message_toTranscript();
    void MessageBoxError(QString message);
    const char* QStringtoChar(QString qs);
    void Display_image(QString lettre, bool choice);
    void Record_sequence();



private slots:

    void SendStrToOpenCR();
    void on_Start_traduction_pressed();
    void on_See_charac_returnPressed();
    void Start_Thread();


//public slots:
  //      void Display_image(QString lettre);

    void on_transcript_charac_returnPressed();

protected:
    Ui::MainWindow *ui;
    int Recording_increment;

private:
    int time_;
    int Letter_increment;
    QString OutputSpeech;
    QString OutputError;
    QString OutputOpenCR;
    QString trad_string;
    QStringList OutputSpeechList;
    QGraphicsScene *scene;
    QPixmap image;
};

#endif // MAINWINDOW_H
