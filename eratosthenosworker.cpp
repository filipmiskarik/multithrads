#include "eratosthenosworker.h"
#include <QDebug>


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
            emit resultReady(result);
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

long int end_number = 234567;

void EratosthenosWorker::run() {

    unsigned long long p = 2;
    double per = 0.0;
    QString result = "";
    std::vector<bool> numbers (end_number + 1, true);

    while (p <= end_number)
    {
        if(p == 200000)
        {
            int flakac = 0;
        }
        if(canRun)
        {
            if (!numbers[p])
            {
                if(p == end_number || p % 1000 == 0)
                { 
                    emit resultReady(result);
                    result = "";
                }
                p++;

                continue;
            }

            result += QString::number(p) + ", ";

            per = (double) p / (double) end_number * 100;

            emit progressUp((int)per);

            if(p == end_number)
            {
                emit resultReady(result);
                result = "";
            }

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
}


void EratosthenosWorker::pauseThread() {
    qDebug() << "ZAPAUZOVAL JSEM ERATOSE";
    canRun = false;
}

void EratosthenosWorker::unPauseThread() {
    qDebug() << "ODPAUZOVAL JSEM ERATOSE";
    canRun = true;
}






//for(int j = (res_size - 1); j >= 0; j -= 1000)
//{
//    resultFinal = "";
//    int lastpos = pos - 1000;
//    if(lastpos < 0)
//    {
//        lastpos = 0;
//    }
//    if(pos == res_size - 1)
//    {
//        lastpos++;
//    }
//    for (int i = pos; i >= lastpos; i--)
//    {
//        resultFinal += QString::number(res[i]);
//    }

//    pos -= 1000;
//    emit resultReady(resultFinal);
//}
