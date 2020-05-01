#include "eratosthenosworker.h"
#include "faktorialworker.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <iostream>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //FAKTORIAL
    FaktorialWorker *worker = new FaktorialWorker;
    //worker->moveToThread(&workerThread);
    qRegisterMetaType<long int>("long int");
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    //connect(this, &MainWindow::operate, worker, &FaktorialWorker::doFactorial);
    connect(worker, &FaktorialWorker::resultReady, this, &MainWindow::handleResults);
    connect(worker, &FaktorialWorker::progressUp, this, &MainWindow::handleProgress);

    connect(this, &MainWindow::factoriaPauseSignal, worker, &FaktorialWorker::pauseThread);
    connect(this, &MainWindow::factoriaResumeSignal, worker, &FaktorialWorker::unPauseThread);

    //ERATOS
    EratosthenosWorker *EratosWorker = new EratosthenosWorker;
    //Eratosworker->moveToThread(&eratosWorkerThread);
    qRegisterMetaType<std::vector<unsigned long long>>("std::vector<unsigned long long>");
    connect(&eratosWorkerThread, &QThread::finished, EratosWorker, &QObject::deleteLater);
    //connect(this, &MainWindow::operateEratos, EratosWorker, &EratosthenosWorker::doEratosthenos);
    connect(EratosWorker, &EratosthenosWorker::resultReady, this, &MainWindow::handleEratosResults);
    connect(EratosWorker, &EratosthenosWorker::progressUp, this, &MainWindow::handleEratosProgress);

    connect(this, &MainWindow::eratosPauseSignal, EratosWorker, &EratosthenosWorker::pauseThread);
    connect(this, &MainWindow::eratosResumeSignal, EratosWorker, &EratosthenosWorker::unPauseThread);

    ui->setupUi(this);
    ui->faktorialProgresBar->setRange(1,100);
    ui->EratosProgresBar->setRange(1,100);
    ui->EratosProgresBar->setValue(1);
    ui->faktorialProgresBar->setValue(1);


    //BUTTONY NA UI
    connect(ui->eratosButton, SIGNAL(released()), this, SLOT(startEratostenos()));
    connect(ui->faktorialButton, SIGNAL(released()), this, SLOT(startFaktorial()));

    connect(ui->unpauseThread_Factorial, SIGNAL(released()), this, SLOT(resumeFactorial()));
    connect(ui->pauseThread_Factorial, SIGNAL(released()), this, SLOT(pauseFactorial()));

    connect(ui->unpauseThread_Eratos, SIGNAL(released()), this, SLOT(resumeEratos()));
    connect(ui->pauseThread_Eratos, SIGNAL(released()), this, SLOT(pauseEratos()));

    //eratosWorkerThread.start();
    //workerThread.start();


    worker->start();
    EratosWorker->start();

}

MainWindow::~MainWindow()
{
    workerThread.quit();
    workerThread.wait();
    eratosWorkerThread.quit();
    eratosWorkerThread.wait();
    delete ui;
}

void MainWindow::handleResults(QString result)
{
    ui->faktorialTextArea->append(result);
}
void::MainWindow::handleProgress(int percentage)
{
    ui->faktorialProgresBar->setValue(percentage);
}

void MainWindow::handleEratosResults(QString result)
{
    qDebug() << "slot";
    ui->EratosTextArea->append(result);
}
void::MainWindow::handleEratosProgress(int percentage)
{
    ui->EratosProgresBar->setValue(percentage);
}

void MainWindow::startEratostenos()
{
     //MainWindow::operateEratos(50000);
     //workerThread.sleep(1000);

    MainWindow::eratosResumeSignal();
}

void MainWindow::startFaktorial()
{
    ui->faktorialTextArea->setText("");
     MainWindow::factoriaResumeSignal();
    //MainWindow::operate(54000);
}


void MainWindow::pauseFactorial()
{
    MainWindow::factoriaPauseSignal();
}

void MainWindow::resumeFactorial()
{

    MainWindow::factoriaResumeSignal();
}


void MainWindow::pauseEratos()
{
    MainWindow::eratosPauseSignal();
}

void MainWindow::resumeEratos()
{

    MainWindow::eratosResumeSignal();
}




