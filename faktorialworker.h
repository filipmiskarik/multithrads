#ifndef FAKTORIALWORKER_H
#define FAKTORIALWORKER_H

#include <QObject>
#include <QThread>



class FaktorialWorker : public QThread
{
    Q_OBJECT

public:
    int multiply(int x, int res[], int res_size);
    bool canRun;
    FaktorialWorker();

protected:
    void run() override;

signals:
    void resultReady(QString result);
    void progressUp(int percentage);


public slots:
    QString* doFactorial(long int n);
    void pauseThread();
    void unPauseThread();

};

#endif // FAKTORIALWORKER_H
