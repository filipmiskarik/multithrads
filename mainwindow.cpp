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
    ui->factorialProgressBar->setRange(0,100);
    ui->eratostenosProgressBar->setRange(0,100);
    ui->eratostenosProgressBar->setValue(0);
    ui->factorialProgressBar->setValue(0);

    connect(ui->startEratostenosButton, SIGNAL(released()), this, SLOT(startEratostenos()));
    connect(ui->startFactorialButton, SIGNAL(released()), this, SLOT(startFactorial()));

    connect(ui->unpauseFactorialButton, SIGNAL(released()), this, SLOT(resumeFactorial()));
    connect(ui->pauseFactorialButton, SIGNAL(released()), this, SLOT(pauseFactorial()));

    connect(ui->unpauseEratostenosButton, SIGNAL(released()), this, SLOT(resumeEratostenos()));
    connect(ui->pauseEratostenosButton, SIGNAL(released()), this, SLOT(pauseEratostenos()));

    connect(ui->stopFactorialButton, SIGNAL(released()), this, SLOT(stopFactorial()));
    connect(ui->stopEratostenosButton, SIGNAL(released()), this, SLOT(stopEratostenos()));

    connect(ui->resultsFactorialButton, SIGNAL(released()), this, SLOT(openResultsFactorial()));
    connect(ui->resultsEratostenosButton, SIGNAL(released()), this, SLOT(openResultsEratostenos()));

    ui->resultsEratostenosButton->setEnabled(false);
    ui->resultsFactorialButton->setEnabled(false);
    ui->factorialLineEdit->setValidator(new QIntValidator);
    ui->eratostenosLineEdit->setValidator(new QIntValidator);
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
    ui->resultsFactorialButton->setEnabled(true);
    ui->factorialLineEdit->setEnabled(true);
}
void::MainWindow::handleFactorialProgress(int percentage)
{
    ui->factorialProgressBar->setValue(percentage);
}

void MainWindow::handleEratostenosResults()
{
    ui->resultsEratostenosButton->setEnabled(true);
    ui->eratostenosLineEdit->setEnabled(true);
}

void::MainWindow::handleEratosProgress(int percentage)
{
    ui->eratostenosProgressBar->setValue(percentage);
}

void MainWindow::startEratostenos()
{
    QString input = ui->eratostenosLineEdit->text();
    ui->resultsEratostenosButton->setEnabled(false);
    ui->eratostenosLineEdit->setEnabled(false);
    ui->eratostenosProgressBar->setValue(0);
    if(!EratostenosWorker->isRunning())
    {
        EratostenosWorker->start();
    }

    MainWindow::eratosResumeSignal(input.toLongLong());
}

void MainWindow::startFactorial()
{
    QString input = ui->factorialLineEdit->text();
    ui->resultsFactorialButton->setEnabled(false);
    ui->factorialLineEdit->setEnabled(false);
    ui->factorialProgressBar->setValue(0);
    worker->start();

    MainWindow::factoriaResumeSignal(input.toLongLong());
}

void MainWindow::pauseFactorial()
{
    MainWindow::factoriaPauseSignal();
}

void MainWindow::resumeFactorial()
{
    QString input = ui->factorialLineEdit->text();
    MainWindow::factoriaResumeSignal(input.toLongLong());
}

void MainWindow::pauseEratostenos()
{
    MainWindow::eratosPauseSignal();
}

void MainWindow::resumeEratostenos()
{
    QString input = ui->eratostenosLineEdit->text();
    MainWindow::eratosResumeSignal(input.toLongLong());
}

void MainWindow::stopFactorial()
{
    if(worker->isRunning())
    {
        worker->terminate();
    }
    ui->factorialLineEdit->setEnabled(true);

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
    connect(sTimer, SIGNAL(timeout()), SLOT(reseteratostenosProgressBar()));
    sTimer->start(1);
    ui->eratostenosLineEdit->setEnabled(true);
}

void MainWindow::openResultsEratostenos()
{
    QDesktopServices::openUrl(QUrl("EratostenosResults.txt", QUrl::TolerantMode));
}

void MainWindow::openResultsFactorial()
{
    QDesktopServices::openUrl(QUrl("FactorialResults.txt", QUrl::TolerantMode));
}

void MainWindow::reseteratostenosProgressBar()
{
    ui->eratostenosProgressBar->setValue(0);
}

void MainWindow::resetFactorialProgressBar()
{
    ui->factorialProgressBar->setValue(0);
}





