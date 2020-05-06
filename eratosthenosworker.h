#ifndef ERATOSTHENOSWORKER_H
#define ERATOSTHENOSWORKER_H

#include <QObject>
#include <QThread>



class EratosthenosWorker  : public QThread
{
    Q_OBJECT

public:
    bool canRun;
    EratosthenosWorker();

protected:
    void run();

signals:
    void resultReady();
    void progressUp(int percentage);

public slots:
    void pauseThread();
    void unPauseThread();
};

#endif // ERATOSTHENOSWORKER_H
