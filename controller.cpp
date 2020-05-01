#include "controller.h"
#include "faktorialworker.h"

Controller::Controller()
{
    FaktorialWorker *worker = new FaktorialWorker;
    worker->moveToThread(&workerThread);

    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &Controller::operate, worker, &Worker::doWork);
    connect(worker, &Worker::resultReady, this, &Controller::handleResults);

    workerThread.start();
}
