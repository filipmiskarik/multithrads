#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "eratosthenosworker.h"
#include "faktorialworker.h"
#include <QMainWindow>
#include <QThread>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT
    QThread workerThread;
    QThread eratosWorkerThread;
    FaktorialWorker *worker = new FaktorialWorker;
    EratosthenosWorker *EratosWorker = new EratosthenosWorker;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTimer* sTimer;

private:
    Ui::MainWindow *ui;

signals:
     void operate(long int timeout);
     void operateEratos(long int timeout);

     void factoriaPauseSignal();
     void factoriaResumeSignal(long long input);

     void eratosPauseSignal();
     void eratosResumeSignal(long long input);

public slots:
     void handleResults();
     void handleProgress(int percentage);
     void startEratostenos();
     void startFaktorial();
     void handleEratosResults();
     void handleEratosProgress(int percentage);

     void pauseFactorial();
     void resumeFactorial();

     void pauseEratos();
     void resumeEratos();

     void stopFact();
     void stopEratos();

     void openResultsEratos();
     void openResultsFact();

private slots:
     void resetProgressBarEratos();
     void resetProgressBarFact();

};
#endif // MAINWINDOW_H
