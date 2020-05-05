#include "eratosthenosworker.h"
#include <QDebug>
#include <QFile>

#define MAX 400000

EratosthenosWorker::EratosthenosWorker()
{
    canRun = false;
}

QString* EratosthenosWorker::doEratosthenos(long int end_number)
{
    QString result = "";

    std::vector<bool> numbers (end_number, true);
    int integer = 0;
    double per = 0.0;

    for (unsigned long long p = 2; p < end_number; p++)
    {
        integer++;
        if ( ! numbers[p])
            continue;

        qDebug() << integer;

        result += QString::number(p) + ", ";

        per = (double) p / (double) end_number * 100;

        emit progressUp((int)per);
        if(integer % 1000 == 0) // a co posledni beh? to asi neemitne...
        {
            emit resultReady();
            result = "";
        }


        if (p * p >= end_number)
            continue;

        for (unsigned long long i = p * p; i <= end_number; i += p)
        {
            numbers[i] = false;
        }
    }
}

long int end_number = 15234567;

void EratosthenosWorker::run() {

    unsigned long long p = 2;
    double per = 0.0;
    QString result = "";
    std::vector<bool> numbers (end_number + 1, true);

    while (p <= end_number)
    {
        if(canRun)
        {
            if (!numbers[p])
            {
                p++;

                continue;
            }

            result += QString::number(p) + ", ";

            per = (double) p / (double) end_number * 100;
            emit progressUp((int)per);

            if (p * p >= end_number)
            {
                p++;
                continue;
            }

            for (unsigned long long i = p * p; i <= end_number; i += p)
            {
                numbers[i] = false;
            }

            p++;
        }
    }

    const QString qPath("eratos.txt");
    QFile qFile(qPath);
    if (qFile.open(QIODevice::WriteOnly))
    {
        QTextStream out(&qFile); out << result;
        qFile.close();
        emit resultReady();
    }

    qDebug() << "koncim";
}


void EratosthenosWorker::pauseThread() {
    qDebug() << "ZAPAUZOVAL JSEM ERATOSE";
    canRun = false;
}

void EratosthenosWorker::unPauseThread() {
    qDebug() << "ODPAUZOVAL JSEM ERATOSE";
    canRun = true;
}
