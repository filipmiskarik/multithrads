#include "faktorialworker.h"

#include <QThread>
#include <QDebug>
#include <QFile>
#include <bits/stdc++.h>

#define MAX 400000

FaktorialWorker::FaktorialWorker()
{
    canRun = true;
}

int FaktorialWorker::multiply(int x, int res[], int res_size)
{
    int carry = 0;

    for (int i=0; i<res_size; i++)
    {
        int prod = res[i] * x + carry;

        res[i] = prod % 10;

        carry  = prod/10;
    }

    while (carry)
    {
        res[res_size] = carry%10;
        carry = carry/10;
        res_size++;
    }
    return res_size;
}

void FaktorialWorker::run() {
    QFile::remove("fact.txt");
    int count = 0;

    count ++;
    qDebug() << "FUNGUJU FACTORIAL!!!!";
    qDebug() << "F" + QString::number(count);

    long int n = 54000;

    int res[MAX];
    QString resultFinal = "";

    res[0] = 1;
    int res_size = 1;

    int x = 2;
    while(x <= n)
    {
        double per = 0.0;
        if(canRun)
        {
            res_size = multiply(x, res, res_size);
            per = (double) x / (double) n * 100.0;
            emit progressUp((int)per);
            x++;
        }
    }
    int pos = res_size - 1;

    for(int j = (res_size - 1); j >= 0; j -= 10000)
    {
        if(canRun)
        {
            int lastpos = pos - 10000;
            if(lastpos < 0)
            {
                lastpos = 0;
            }
            if(pos == res_size - 1)
            {
                lastpos++;
            }
            for (int i = pos; i >= lastpos; i--)
            {
                resultFinal += QString::number(res[i]);
            }

            pos -= 10000;
            emit resultReady();
        }
    }

    const QString qPath("fact.txt");
    QFile qFile(qPath);
    if (qFile.open(QIODevice::WriteOnly))
    {
        QTextStream out(&qFile); out << resultFinal;
        qFile.close();
        emit resultReady();
    }
}

void FaktorialWorker::pauseThread() {
    qDebug() << "ZAPAUZOVAL JSEM FACTORAL";
    canRun = false;
}

void FaktorialWorker::unPauseThread() {
    qDebug() << "ODPAUZOVAL JSEM FACTORIAL";
    canRun = true;
}

