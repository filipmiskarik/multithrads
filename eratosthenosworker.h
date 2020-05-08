#ifndef ERATOSTHENOSWORKER_H
#define ERATOSTHENOSWORKER_H

#include <QObject>
#include <QThread>

extern long long end_number;

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
    void unPauseThread(long long input);
};

#endif // ERATOSTHENOSWORKER_H
