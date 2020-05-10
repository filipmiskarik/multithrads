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

    ui->factorialLineEdit->setValidator(new QIntValidator);
    ui->eratostenosLineEdit->setValidator(new QIntValidator);

    ui->resultsFactorialButton->setEnabled(false);
    ui->stopFactorialButton->setEnabled(false);
    ui->pauseFactorialButton->setEnabled(false);
    ui->unpauseFactorialButton->setEnabled(false);

    ui->resultsEratostenosButton->setEnabled(false);
    ui->stopEratostenosButton->setEnabled(false);
    ui->pauseEratostenosButton->setEnabled(false);
    ui->unpauseEratostenosButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    workerThread.quit();
    workerThread.wait();
    EratostenosWorkerThread.quit();
    EratostenosWorkerThread.wait();
    delete ui;
}

/// Factorial functions

void MainWindow::handleFactorialResults()
{
    ui->resultsFactorialButton->setEnabled(true);
    ui->factorialLineEdit->setEnabled(true);
    ui->startFactorialButton->setEnabled(true);
    ui->stopFactorialButton->setEnabled(false);
    ui->pauseFactorialButton->setEnabled(false);
    ui->unpauseFactorialButton->setEnabled(false);
}

void::MainWindow::handleFactorialProgress(int percentage)
{
    ui->factorialProgressBar->setValue(percentage);
}

void MainWindow::startFactorial()
{
    if(ui->factorialLineEdit->text() == "")
    {
        ui->factorialLineEdit->setPlaceholderText("I need a number... ");
        ui->factorialLineEdit->setStyleSheet("#factorialLineEdit { color: red; border: 1px solid red;}");
    }
    else
    {
        ui->factorialLineEdit->setStyleSheet("");
        QString input = ui->factorialLineEdit->text();

        ui->resultsFactorialButton->setEnabled(false);
        ui->factorialLineEdit->setEnabled(false);
        ui->factorialProgressBar->setValue(0);
        ui->pauseFactorialButton->setEnabled(true);
        ui->startFactorialButton->setEnabled(false);
        ui->stopFactorialButton->setEnabled(true);

        worker->start();
        MainWindow::factoriaResumeSignal(input.toLongLong());
    }
}

void MainWindow::pauseFactorial()
{
    MainWindow::factoriaPauseSignal();

    ui->pauseFactorialButton->setEnabled(false);
    ui->unpauseFactorialButton->setEnabled(true);
}

void MainWindow::resumeFactorial()
{
    QString input = ui->factorialLineEdit->text();
    MainWindow::factoriaResumeSignal(input.toLongLong());

    ui->pauseFactorialButton->setEnabled(true);
    ui->unpauseFactorialButton->setEnabled(false);
}

void MainWindow::stopFactorial()
{
    if(worker->isRunning())
    {
        worker->terminate();
    }

    sTimer = new QTimer();
    sTimer->setSingleShot(true);
    connect(sTimer, SIGNAL(timeout()), SLOT(resetFactorialProgressBar()));
    sTimer->start(1);

    ui->factorialLineEdit->setEnabled(true);
    ui->startFactorialButton->setEnabled(true);
    ui->stopFactorialButton->setEnabled(false);
    ui->pauseFactorialButton->setEnabled(false);
    ui->unpauseFactorialButton->setEnabled(false);
}

void MainWindow::openResultsFactorial()
{
    QDesktopServices::openUrl(QUrl("FactorialResults.txt", QUrl::TolerantMode));
}

void MainWindow::resetFactorialProgressBar()
{
    ui->factorialProgressBar->setValue(0);
}

/// Eratostenos function

void MainWindow::handleEratostenosResults()
{
    ui->resultsEratostenosButton->setEnabled(true);
    ui->eratostenosLineEdit->setEnabled(true);
    ui->startEratostenosButton->setEnabled(true);
    ui->stopEratostenosButton->setEnabled(false);
    ui->pauseEratostenosButton->setEnabled(false);
    ui->unpauseEratostenosButton->setEnabled(false);
}

void::MainWindow::handleEratosProgress(int percentage)
{
    ui->eratostenosProgressBar->setValue(percentage);
}

void MainWindow::startEratostenos()
{
    if(ui->eratostenosLineEdit->text() == "")
    {
        ui->eratostenosLineEdit->setPlaceholderText("I need end number...");
        ui->eratostenosLineEdit->setStyleSheet("#eratostenosLineEdit { color: red; border: 1px solid red;}");
    }
    else
    {
        ui->eratostenosLineEdit->setStyleSheet("");
        QString input = ui->eratostenosLineEdit->text();

        ui->resultsEratostenosButton->setEnabled(false);
        ui->eratostenosLineEdit->setEnabled(false);
        ui->eratostenosProgressBar->setValue(0);
        ui->stopEratostenosButton->setEnabled(true);
        ui->pauseEratostenosButton->setEnabled(true);
        ui->startEratostenosButton->setEnabled(false);


        if(!EratostenosWorker->isRunning())
        {
            EratostenosWorker->start();
        }

        MainWindow::eratosResumeSignal(input.toLongLong());
    }
}

void MainWindow::pauseEratostenos()
{
    MainWindow::eratosPauseSignal();
    ui->pauseEratostenosButton->setEnabled(false);
    ui->unpauseEratostenosButton->setEnabled(true);
}

void MainWindow::resumeEratostenos()
{
    QString input = ui->eratostenosLineEdit->text();
    MainWindow::eratosResumeSignal(input.toLongLong());
    ui->pauseEratostenosButton->setEnabled(true);
    ui->unpauseEratostenosButton->setEnabled(false);
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
    ui->stopEratostenosButton->setEnabled(false);
    ui->pauseEratostenosButton->setEnabled(false);
    ui->unpauseEratostenosButton->setEnabled(false);
    ui->startEratostenosButton->setEnabled(true);
}

void MainWindow::openResultsEratostenos()
{
    QDesktopServices::openUrl(QUrl("EratostenosResults.txt", QUrl::TolerantMode));
}

void MainWindow::reseteratostenosProgressBar()
{
    ui->eratostenosProgressBar->setValue(0);
}
