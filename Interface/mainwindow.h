#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QStringList>

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
    void Message();

private slots:
    void Record_sequence();
    void on_Start_record_button_clicked();

private:
    Ui::MainWindow *ui;
    int time_;
    int Recording_increment;
    QString OutputSpeech;
    QString OutputError;
    QStringList OutputSpeechList;
};

#endif // MAINWINDOW_H
