#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QThread>
#include "curl_utils.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MyThread : public QThread
{
    Q_OBJECT
signals:
    void updateUI(QString value);
public:
    void run();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_submit_clicked();
    void show_gpt_response(QString qstr);

private:
    Ui::MainWindow *ui;
    QWidget *widget;
    QObject *worker;
};
#endif // MAINWINDOW_H
