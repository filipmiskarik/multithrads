#include "eratosthenosworker.h"
#include <QDebug>
#include <QFile>

#define MAX 400000

EratosthenosWorker::EratosthenosWorker()
{
    canRun = true;
}

long long end_number;

void EratosthenosWorker::run()
{
    QFile::remove("eratos.txt");
    long long p = 2;
    double per = 0.0;
    QString result = "";
    std::vector<bool> numbers (end_number + 1, true);

    while (p <= end_number)
    {
        if(canRun)
        {
            per = static_cast<double>(p) / static_cast<double>(end_number) * 100;
            emit progressUp(qRound(per));

            if (!numbers[p])
            {
                p++;
                continue;
            }

            result += QString::number(p) + ", ";

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
}


void EratosthenosWorker::pauseThread()
{
    canRun = false;
}

void EratosthenosWorker::unPauseThread(long long input)
{
    end_number = input;
    canRun = true;
}
