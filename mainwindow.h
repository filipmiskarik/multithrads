#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT
QThread workerThread;
QThread eratosWorkerThread;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

signals:
     void operate(long int timeout);
     void operateEratos(long int timeout);

     void factoriaPauseSignal();
     void factoriaResumeSignal();

     void eratosPauseSignal();
     void eratosResumeSignal();

public slots:
     void handleResults(QString result);
     void handleProgress(int percentage);
     void startEratostenos();
     void startFaktorial();
     void handleEratosResults(QString result);
     void handleEratosProgress(int percentage);

     void pauseFactorial();
     void resumeFactorial();

     void pauseEratos();
     void resumeEratos();
};
#endif // MAINWINDOW_H
