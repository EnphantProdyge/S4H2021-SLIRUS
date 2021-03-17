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
    void Set_label_visible(QLabel *label);
    void Set_label_invisible(QLabel *label);
    void Message_toTranscript();
    bool startsWith(const char *pre, const char *str);
    void MessageBoxError(QString message);
    const char* QStringtoChar(QString qs);
    void Display_image(QString lettre);

private slots:
    void Record_sequence();
    void SendStrToOpenCR();
    void on_Start_traduction_pressed();
    void on_See_charac_returnPressed();

//public slots:
  //      void Display_image(QString lettre);

private:
    Ui::MainWindow *ui;
    int time_;
    int Recording_increment;
    QString OutputSpeech;
    QString OutputError;
    QString OutputOpenCR;
    QStringList OutputSpeechList;
    QGraphicsScene *scene;
    QPixmap image;
};

#endif // MAINWINDOW_H
