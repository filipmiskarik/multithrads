#include "eratosthenosworker.h"
#include "faktorialworker.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDesktopServices>
#include <iostream>
#include <QTimer>
#include <QUrl>
#include <QIntValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qRegisterMetaType<long int>("long int");
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(worker, &FaktorialWorker::resultReady, this, &MainWindow::handleResults);
    connect(worker, &FaktorialWorker::progressUp, this, &MainWindow::handleProgress);

    connect(this, &MainWindow::factoriaPauseSignal, worker, &FaktorialWorker::pauseThread);
    connect(this, &MainWindow::factoriaResumeSignal, worker, &FaktorialWorker::unPauseThread);

    qRegisterMetaType<std::vector<unsigned long long>>("std::vector<unsigned long long>");
    connect(&eratosWorkerThread, &QThread::finished, EratosWorker, &QObject::deleteLater);
    connect(EratosWorker, &EratosthenosWorker::resultReady, this, &MainWindow::handleEratosResults);
    connect(EratosWorker, &EratosthenosWorker::progressUp, this, &MainWindow::handleEratosProgress);

    connect(this, &MainWindow::eratosPauseSignal, EratosWorker, &EratosthenosWorker::pauseThread);
    connect(this, &MainWindow::eratosResumeSignal, EratosWorker, &EratosthenosWorker::unPauseThread);

    ui->setupUi(this);
    ui->faktorialProgresBar->setRange(0,100);
    ui->EratosProgresBar->setRange(0,100);
    ui->EratosProgresBar->setValue(0);
    ui->faktorialProgresBar->setValue(0);

    //BUTTONY NA UI
    connect(ui->eratosButton, SIGNAL(released()), this, SLOT(startEratostenos()));
    connect(ui->faktorialButton, SIGNAL(released()), this, SLOT(startFaktorial()));

    connect(ui->unpauseThread_Factorial, SIGNAL(released()), this, SLOT(resumeFactorial()));
    connect(ui->pauseThread_Factorial, SIGNAL(released()), this, SLOT(pauseFactorial()));

    connect(ui->unpauseThread_Eratos, SIGNAL(released()), this, SLOT(resumeEratos()));
    connect(ui->pauseThread_Eratos, SIGNAL(released()), this, SLOT(pauseEratos()));

    connect(ui->stopThread_Factorial, SIGNAL(released()), this, SLOT(stopFact()));
    connect(ui->stopThread_Eratos, SIGNAL(released()), this, SLOT(stopEratos()));

    connect(ui->pushButtonEratos, SIGNAL(released()), this, SLOT(openResultsEratos()));
    connect(ui->pushButtonFact, SIGNAL(released()), this, SLOT(openResultsFact()));

    ui->pushButtonEratos->setEnabled(false);
    ui->pushButtonFact->setEnabled(false);
    ui->lineEditFact->setValidator(new QIntValidator);
    ui->lineEditEratos->setValidator(new QIntValidator);
}

MainWindow::~MainWindow()
{
    workerThread.quit();
    workerThread.wait();
    eratosWorkerThread.quit();
    eratosWorkerThread.wait();
    delete ui;
}

void MainWindow::handleResults()
{
    ui->pushButtonFact->setEnabled(true);
//    ui->faktorialProgresBar->setValue(100);
}
void::MainWindow::handleProgress(int percentage)
{
    ui->faktorialProgresBar->setValue(percentage);
}

void MainWindow::handleEratosResults()
{
    ui->pushButtonEratos->setEnabled(true);
//    ui->EratosProgresBar->setValue(100);
}

void::MainWindow::handleEratosProgress(int percentage)
{
    ui->EratosProgresBar->setValue(percentage);
}

void MainWindow::startEratostenos()
{
    QString input = ui->lineEditEratos->text();
    ui->pushButtonEratos->setEnabled(false);
    ui->EratosProgresBar->setValue(0);
    EratosWorker->start();
    MainWindow::eratosResumeSignal(input.toLongLong());
}

void MainWindow::startFaktorial()
{
    QString input = ui->lineEditFact->text();
    ui->pushButtonFact->setEnabled(false);
    ui->faktorialProgresBar->setValue(0);
    worker->start();

    MainWindow::factoriaResumeSignal(input.toLongLong());
}

void MainWindow::pauseFactorial()
{
    MainWindow::factoriaPauseSignal();
}

void MainWindow::resumeFactorial()
{
    QString input = ui->lineEditFact->text();
    MainWindow::factoriaResumeSignal(input.toLongLong());
}

void MainWindow::pauseEratos()
{
    MainWindow::eratosPauseSignal();
}

void MainWindow::resumeEratos()
{
    QString input = ui->lineEditEratos->text();
    MainWindow::eratosResumeSignal(input.toLongLong());
}

void MainWindow::stopFact()
{
    if(worker->isRunning())
    {
        worker->terminate();
    }

    sTimer = new QTimer();
    sTimer->setSingleShot(true);
    connect(sTimer, SIGNAL(timeout()), SLOT(resetProgressBarFact()));
    sTimer->start(1);
}

void MainWindow::stopEratos()
{
    if(EratosWorker->isRunning())
    {
         EratosWorker->terminate();
    }

    sTimer = new QTimer();
    sTimer->setSingleShot(true);
    connect(sTimer, SIGNAL(timeout()), SLOT(resetProgressBarEratos()));
    sTimer->start(1);
}

void MainWindow::openResultsEratos()
{
    QDesktopServices::openUrl(QUrl("eratos.txt", QUrl::TolerantMode));
}
void MainWindow::openResultsFact()
{
    QDesktopServices::openUrl(QUrl("fact.txt", QUrl::TolerantMode));
}

void MainWindow::resetProgressBarEratos()
{
    ui->EratosProgresBar->setValue(0);
}

void MainWindow::resetProgressBarFact()
{
    ui->faktorialProgresBar->setValue(0);
}





