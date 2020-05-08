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
    QThread EratostenosWorkerThread;
    FaktorialWorker *worker = new FaktorialWorker;
    EratosthenosWorker *EratostenosWorker = new EratosthenosWorker;
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
     void handleFactorialResults();
     void handleFactorialProgress(int percentage);
     void startEratostenos();
     void startFactorial();
     void handleEratostenosResults();
     void handleEratosProgress(int percentage);

     void pauseFactorial();
     void resumeFactorial();

     void pauseEratostenos();
     void resumeEratostenos();

     void stopFactorial();
     void stopEratostenos();

     void openResultsEratostenos();
     void openResultsFactorial();

private slots:
     void resetEratostenosProgressBar();
     void resetFactorialProgressBar();

};
#endif // MAINWINDOW_H
