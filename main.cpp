#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;

int a;
int b;
int thread_number;
int n;
double *result;
double h;

void set_h(){
    h = (b - a) / 1.0 / n;
}

struct data{
    int index;
};

double f(double x)
{
    return(10 - x);
}

void integralpram()
{
    double sum = 0;
    result = new double[thread_number];
    data data_t[thread_number];
    set_h();
    for (int i = 0; i < thread_number; i++) {
        data_t[i].index = i;
    }

    #pragma omp parallel for
    for (int i = 0; i < thread_number; ++i) {
        double S = 0;
        double F, xi;
        int start = data_t[i].index * (n / thread_number);
        int finish = data_t[i].index * (n / thread_number) + (n / thread_number);
        for (int s = start; s < finish; s++)
        {
            xi = a + s * h;
            F = f(xi);
            S += F * h;
        }
        result[data_t[i].index] = S;
    }

    for (int i = 0; i < thread_number; ++i) {
        sum += result[i];
    }

    cout << "result - " << sum << endl;
}

int main()
{
    setlocale(LC_ALL, "ru");
    cout << "Введите нижний предел интегрирования" << endl;
    cin >> a;
    cout << "Введите верхний предел интегрирования" << endl;
    cin >> b;
    cout << "Введите кол-во шагов" << endl;
    cin >> n;
    cout << "Введите кол-во потоков" << endl;
    cin >> thread_number;

    auto begin = std::chrono::steady_clock::now();
    integralpram();
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    cout << "The time: " << elapsed_ms.count() << " ms";
}