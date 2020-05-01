#include "faktorialworker.h"

#include <QThread>
#include <QDebug>
#include <bits/stdc++.h>

#define MAX 400000

FaktorialWorker::FaktorialWorker()
{
    canRun = false;
}

int FaktorialWorker::multiply(int x, int res[], int res_size)
{
    int carry = 0;  // Initialize carry

    // One by one multiply n with individual digits of res[]
    for (int i=0; i<res_size; i++)
    {
        int prod = res[i] * x + carry;

        // Store last digit of 'prod' in res[]
        res[i] = prod % 10;

        // Put rest in carry
        carry  = prod/10;
    }

    // Put carry in res and increase result size
    while (carry)
    {
        res[res_size] = carry%10;
        carry = carry/10;
        res_size++;
    }
    return res_size;
}

QString* FaktorialWorker::doFactorial(long int n)
{  

    int res[MAX];
    QString resultFinal = "";

    // Initialize result
    res[0] = 1;
    int res_size = 1;
    double per = 0.0;

    for (int x=2; x <= n; x++)
    {
        res_size = multiply(x, res, res_size);
        per = (double) x / (double) n * 100.0;
        emit progressUp((int)per);
    }

    int pos = res_size - 1;
    for(int j = (res_size - 1); j >= 0; j -= 100000)
    {
        resultFinal = "";
        int lastpos = pos - 100000;
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

        pos -= 100000;
        emit resultReady(resultFinal);
    }
}

void FaktorialWorker::run() {

    int count = 0;

      //if(canRun)
      //{

          count ++;
         qDebug() << "FUNGUJU FACTORIAL!!!!";
         qDebug() << "F" + QString::number(count);

         long int n = 54000;

         int res[MAX];
         QString resultFinal = "";

         // Initialize result
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
                 resultFinal = "";
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
                 emit resultReady(resultFinal);
              }
          }
      //}

}

void FaktorialWorker::pauseThread() {
    qDebug() << "ZAPAUZOVAL JSEM FACTORAL";
    canRun = false;
}

void FaktorialWorker::unPauseThread() {
    qDebug() << "ODPAUZOVAL JSEM FACTORIAL";
    canRun = true;
}

