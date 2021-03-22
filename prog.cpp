#include <iostream>
#include<iomanip>
#include <time.h>
#include<cmath>
#include <stdlib.h>
#include<vector>
#include <iterator>
#include <cmath>
#include <clocale>
#include<algorithm>
#define _USE_MATH_DEFINES
#define pb push_back
using namespace std;

int applic_count(double l, double time) //моделирование числа заявок
{
    int n = 0;
    double v, yfl, T;
    v = 1.0;
    while(true) // while T <= 0
    {
        do{
           yfl = 0.01 * (rand() % 101);
        }while(yfl == 0); //чтобы не умножать на 0
        v = v * yfl;
        T = v - exp(-1.0 * l * time);
        if(T <= 0) {break;}
        n++;
    }
    return n;
}

vector<double> input(int n, double time) //моделирование входящего потока
{
    vector<double> ksi;
    double yfl;
    while(ksi.size() < n)
    {
        yfl = 0.01 * (rand() % 101);
        yfl *= time;
        {ksi.pb(yfl);} //push_back to vector
    }
    return ksi;
}

vector<double> output(int n, double m) //моделирования потока обслуживания
{
    double yfl;
    vector<double> eta;
    srand(time(0)); //функция рандома от времени
    while(eta.size() < n)
    {
        yfl = 0.01 * (rand() % 101);
        eta.pb(-log(1 - yfl) / m);
    }
    return eta;
}

int main()
{
    setlocale(0, "rus");
    srand(time(NULL));
    int accept_count = 0; //принятые заявки
    int fault_count = 0; //отказанные заявки
    int app_count; //общее количество заявок
    int canals_count; //число каналов
    // интенсивность входящего потока, интенсивность потока обслуживаний, общее время работы
    double lambda, mu, time_work;
    cout<<"lambda=";cin>>lambda; //ввод интенсивности входящего потока
    cout<<"mu=";cin>>mu; //ввод интенсивности потока обслуживаний,
    cout<<"canals_count=";cin>>canals_count;

    mu*=canals_count;

    cout<<"time_work=";cin>>time_work;
    app_count=applic_count(lambda,time_work); //моделирование числа заявок
    vector<double> input_app = input(app_count,time_work); //моделирование входящего потока
    vector<double> output_app = output(app_count,mu); //моделирования потока обслуживания
    cout<<"\n\n\n";
    sort(input_app.begin(),input_app.end());
    //вектор времени выхода заявки, вектор времени появления заявки в системе, вектор появления заявки
    vector<double> gamma(app_count+1),betta(app_count+1),t(app_count+1);
    t[0]=0;
    gamma[0]=0;
    double time_proc=0; //сумма времени обработок заявки
    int i;
    for(i = 1; i < app_count+1; i++)
    {
        t[i] = input_app[i-1]; //инициализация входящим потоком
        cout<<"Application "<<i<<"\n";
        cout<<"Arrival time -- "<<input_app[i]<<"\n"; //прибытие заявки
        cout<<"App's status";
        if(t[i] < time_work) //если время работы не окончено
        {
            if(t[i] < gamma[i-1]) //проверка на выход предыдущей заявки
            {
                fault_count++;
                cout<<" -- fault \n\n";
            }
            else
            {
                betta[i-1] = t[i]; //новая заявка
				gamma[i] = betta[i-1] + output_app[i-1]; //расчет времени выхода
				if(gamma[i] <= time_work)
                {
                    accept_count++;
                    cout<<" -- accept \n";
                    cout<<"Processing time -- "<<output_app[i-1]<<endl;
                    cout<<"Exit time -- "<<gamma[i]<<"\n\n";
                    time_proc+=output_app[i-1];

                }
                else //конец работы
                {
                    fault_count++;
                    cout<<" -- fault\n\n";
                }
            }
        }
        else
        {
            cout<<" -- null\nTime is out\n\n";break;
        }
    }

    /*----------------------------------------*/
    cout<<"lambda="<<lambda<<"\nmu="<<mu/canals_count;
    cout<<"canals_count="<<canals_count<<"\ntime_work="<<time_work;

    double p_th,p_em,a_th,a_em,count_th,count_em,serv_time_th,serv_time_em;
    double smo_time_th,smo_time_em,proc_count_th,proc_count_em,busy_count_th,busy_count_em;
    p_em = accept_count * 1.0 / (accept_count + fault_count);
    p_th = canals_count * 1.0 / (lambda / mu + canals_count);
    //вероятности
    cout<<"\np_th="<<p_th<<"\np_em="<<p_em<<endl;

    a_th = lambda * p_th;
    a_em = accept_count * 1.0 / time_work;
    //абсолютные пропускные спобобности
    cout<<"\na_th="<<a_th<<"\na_em="<<a_em<<endl;

    busy_count_th = canals_count * lambda * 1.0 / (lambda + mu);
    busy_count_em = canals_count * time_proc / time_work;
    //занятые каналы
    cout<<"\nbusy_count_th="<<busy_count_th<<"\nbusy_count_em="<<busy_count_em<<endl;

    count_th = lambda * time_work;
    count_em = accept_count + fault_count;
    //число заявок
    cout<<"\ncount_th="<<count_th<<"\ncount_em="<<count_em<<endl;

    serv_time_th = 1.0 / mu;
    serv_time_em = time_proc / accept_count;
    //среднее время обработки заявки
    cout<<"\nserv_time_th="<<serv_time_th<<"\nserv_time_em="<<serv_time_em<<endl;

    smo_time_th = 1.0 / (lambda + mu);
    smo_time_em = time_proc / (accept_count + fault_count);
    //среднее время в системе одной заявки
    cout<<"\nsmo_time_th="<<smo_time_th<<"\nsmo_time_em="<<smo_time_em<<endl;

    proc_count_th = lambda / mu / (lambda / mu + canals_count);
    proc_count_em = 1.0 - time_proc / time_work;
    //среднее число заявок под обслуживанием
    cout<<"\nproc_count_th="<<proc_count_th<<"\nproc_count_em="<<proc_count_em;
    return 0;
}