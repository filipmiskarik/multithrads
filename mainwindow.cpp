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
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qRegisterMetaType<long int>("long int");
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(worker, &FaktorialWorker::resultReady, this, &MainWindow::handleFactorialResults);
    connect(worker, &FaktorialWorker::progressUp, this, &MainWindow::handleFactorialProgress);

    connect(this, &MainWindow::factoriaPauseSignal, worker, &FaktorialWorker::pauseThread);
    connect(this, &MainWindow::factoriaResumeSignal, worker, &FaktorialWorker::unPauseThread);

    qRegisterMetaType<std::vector<unsigned long long>>("std::vector<unsigned long long>");
    connect(&EratostenosWorkerThread, &QThread::finished, EratostenosWorker, &QObject::deleteLater);
    connect(EratostenosWorker, &EratosthenosWorker::resultReady, this, &MainWindow::handleEratostenosResults);
    connect(EratostenosWorker, &EratosthenosWorker::progressUp, this, &MainWindow::handleEratosProgress);

    connect(this, &MainWindow::eratosPauseSignal, EratostenosWorker, &EratosthenosWorker::pauseThread);
    connect(this, &MainWindow::eratosResumeSignal, EratostenosWorker, &EratosthenosWorker::unPauseThread);

    ui->setupUi(this);
    ui->faktorialProgresBar->setRange(0,100);
    ui->EratosProgresBar->setRange(0,100);
    ui->EratosProgresBar->setValue(0);
    ui->faktorialProgresBar->setValue(0);

    connect(ui->eratosButton, SIGNAL(released()), this, SLOT(startEratostenos()));
    connect(ui->faktorialButton, SIGNAL(released()), this, SLOT(startFactorial()));

    connect(ui->unpauseThread_Factorial, SIGNAL(released()), this, SLOT(resumeFactorial()));
    connect(ui->pauseThread_Factorial, SIGNAL(released()), this, SLOT(pauseFactorial()));

    connect(ui->unpauseThread_Eratos, SIGNAL(released()), this, SLOT(resumeEratostenos()));
    connect(ui->pauseThread_Eratos, SIGNAL(released()), this, SLOT(pauseEratostenos()));

    connect(ui->stopThread_Factorial, SIGNAL(released()), this, SLOT(stopFactorial()));
    connect(ui->stopThread_Eratos, SIGNAL(released()), this, SLOT(stopEratostenos()));

    connect(ui->pushButtonEratos, SIGNAL(released()), this, SLOT(openResultsEratostenos()));
    connect(ui->pushButtonFact, SIGNAL(released()), this, SLOT(openResultsFactorial()));

    ui->pushButtonEratos->setEnabled(false);
    ui->pushButtonFact->setEnabled(false);
    ui->lineEditFact->setValidator(new QIntValidator);
    ui->lineEditEratos->setValidator(new QIntValidator);
}

MainWindow::~MainWindow()
{
    workerThread.quit();
    workerThread.wait();
    EratostenosWorkerThread.quit();
    EratostenosWorkerThread.wait();
    delete ui;
}

void MainWindow::handleFactorialResults()
{
    ui->pushButtonFact->setEnabled(true);
    ui->lineEditFact->setEnabled(true);
}
void::MainWindow::handleFactorialProgress(int percentage)
{
    ui->faktorialProgresBar->setValue(percentage);
}

void MainWindow::handleEratostenosResults()
{
    ui->pushButtonEratos->setEnabled(true);
    ui->lineEditEratos->setEnabled(true);
}

void::MainWindow::handleEratosProgress(int percentage)
{
    ui->EratosProgresBar->setValue(percentage);
}

void MainWindow::startEratostenos()
{
    QString input = ui->lineEditEratos->text();
    ui->pushButtonEratos->setEnabled(false);
    ui->lineEditEratos->setEnabled(false);
    ui->EratosProgresBar->setValue(0);
    if(!EratostenosWorker->isRunning())
    {
        EratostenosWorker->start();
    }

    MainWindow::eratosResumeSignal(input.toLongLong());
}

void MainWindow::startFactorial()
{
    QString input = ui->lineEditFact->text();
    ui->pushButtonFact->setEnabled(false);
    ui->lineEditFact->setEnabled(false);
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

void MainWindow::pauseEratostenos()
{
    MainWindow::eratosPauseSignal();
}

void MainWindow::resumeEratostenos()
{
    QString input = ui->lineEditEratos->text();
    MainWindow::eratosResumeSignal(input.toLongLong());
}

void MainWindow::stopFactorial()
{
    if(worker->isRunning())
    {
        worker->terminate();
    }
    ui->lineEditFact->setEnabled(true);

    sTimer = new QTimer();
    sTimer->setSingleShot(true);
    connect(sTimer, SIGNAL(timeout()), SLOT(resetFactorialProgressBar()));
    sTimer->start(1);
}

void MainWindow::stopEratostenos()
{
    EratostenosWorker->canRun = false;
    if(!EratostenosWorker->wait(200))
    {
      EratostenosWorker->terminate();
      EratostenosWorker->wait();
    }

    sTimer = new QTimer();
    sTimer->setSingleShot(true);
    connect(sTimer, SIGNAL(timeout()), SLOT(resetEratostenosProgressBar()));
    sTimer->start(1);
    ui->lineEditEratos->setEnabled(true);
}

void MainWindow::openResultsEratostenos()
{
    QDesktopServices::openUrl(QUrl("eratos.txt", QUrl::TolerantMode));
}

void MainWindow::openResultsFactorial()
{
    QDesktopServices::openUrl(QUrl("fact.txt", QUrl::TolerantMode));
}

void MainWindow::resetEratostenosProgressBar()
{
    ui->EratosProgresBar->setValue(0);
}

void MainWindow::resetFactorialProgressBar()
{
    ui->faktorialProgresBar->setValue(0);
}





