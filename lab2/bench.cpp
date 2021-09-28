#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sys/time.h>

using namespace std;
string opt;
ofstream result;
int m = 10000;
int n = 10000;
double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}
template <typename type>
void matrix_vector_product(type a, type b, type c, int m, int n)
{
    for (int i = 0; i < m; i++) {
        c[i] = 0.0;
        for (int j = 0; j < n; j++) {
            c[i] += a[i * n + j] * b[j];
        }
    }
}
void run_double()
{
    double *a, *b, *c;
    a = new double[m * n];
    b = new double[m * n];
    c = new double[m * n];
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            a[i * n + j] = i + j;
    }
    for (int j = 0; j < n; j++)
        b[j] = j;
    matrix_vector_product(a, b, c, m, n);
    delete (a);
    delete (b);
    delete (c);
}
void run_int()
{
    int *a, *b, *c;
    a = new int[m * n];
    b = new int[m * n];
    c = new int[m * n];
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            a[i * n + j] = i + j;
    }
    for (int j = 0; j < n; j++)
        b[j] = j;
    matrix_vector_product(a, b, c, m, n);
    delete (a);
    delete (b);
    delete (c);
}
void run_float()
{
    float *a, *b, *c;
    a = new float[m * n];
    b = new float[m * n];
    c = new float[m * n];
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            a[i * n + j] = i + j;
    }
    for (int j = 0; j < n; j++)
        b[j] = j;
    matrix_vector_product(a, b, c, m, n);
    delete (a);
    delete (b);
    delete (c);
}
void test_int()
{
    auto cpuinfo = popen("cat /proc/cpuinfo | grep 'model name'", "r");
    char cpumodel[50];
    fscanf(cpuinfo, "model name     : %50[^\n]s", cpumodel);
    double* k = new double[10];
    double t_int = wtime();
    for (int i = 0; i < 10; i++) {
        k[i] = wtime();
        run_int();
        k[i] = wtime() - k[i];
        cout << i + 1 << "-e испытание заняло " << k[i] << " секунд" << endl;
    }
    t_int = wtime() - t_int;
    cout << "Общее время испытаний заняло " << t_int << " секунд" << endl;
    double int_average = t_int / 10;
    for (int i = 0; i < 10; i++) {
        result << cpumodel << ";"
               << "DGEMV;"
               << "int;" << opt << ";" << i + 1 << ";" << m * n << ";"
               << int_average << ";" << abs(k[i] - int_average) << ";"
               << (abs(k[i] - int_average) / k[i]) * 100 << ";"
               << m * n / int_average << ";" << endl;
    }
    result << "\n";
}
void test_double()
{
    auto cpuinfo = popen("cat /proc/cpuinfo | grep 'model name'", "r");
    char cpumodel[50];
    fscanf(cpuinfo, "model name     : %50[^\n]s", cpumodel);
    double* k = new double[10];
    double t_double = wtime();
    for (int i = 0; i < 10; i++) {
        k[i] = wtime();
        run_double();
        k[i] = wtime() - k[i];
        cout << i + 1 << "-e испытание заняло " << k[i] << " секунд" << endl;
    }
    t_double = wtime() - t_double;
    cout << "Общее время испытаний заняло " << t_double << " секунд" << endl;
    double double_average = t_double / 10;
    for (int i = 0; i < 10; i++) {
        result << cpumodel << ";"
               << "DGEMV;"
               << "double;" << opt << ";" << i + 1 << ";" << m * n << ";"
               << double_average << ";" << abs(k[i] - double_average) << ";"
               << (abs(k[i] - double_average) / k[i]) * 100 << ";"
               << m * n / double_average << ";" << endl;
    }
    result << "\n";
}
void test_float()
{
    auto cpuinfo = popen("cat /proc/cpuinfo | grep 'model name'", "r");
    char cpumodel[50];
    fscanf(cpuinfo, "model name     : %50[^\n]s", cpumodel);
    double* k = new double[10];
    double t_float = wtime();
    for (int i = 0; i < 10; i++) {
        k[i] = wtime();
        run_float();
        k[i] = wtime() - k[i];
        cout << i + 1 << "-e испытание заняло " << k[i] << " секунд" << endl;
    }
    t_float = wtime() - t_float;
    cout << "Общее время испытаний заняло " << t_float << " секунд" << endl;
    double float_average = t_float / 10;
    for (int i = 0; i < 10; i++) {
        result << cpumodel << ";"
               << "DGEMV;"
               << "float;" << opt << ";" << i + 1 << ";" << m * n << ";"
               << float_average << ";" << abs(k[i] - float_average) << ";"
               << (abs(k[i] - float_average) / k[i]) * 100 << ";"
               << m * n / float_average << ";" << endl;
    }
    result << "\n";
}
int main(int argc, char** argv)
{
    // string* file = new string[2];
    // for (int i = 1; i < 2; i++) {
    //     file[i] = argv[i];
    //     file[i].append(".csv");
    // }
    // result.open(file[1]);
    opt = argv[2];
    result.open(argv[1]);
    result << "PModel;Task;OpType;Opt;LNum;InsCount;AvTime;AbsErr;RelErr(%);"
              "TaskPerf\n";
    test_int();
    test_double();
    test_float();
    result.close();
    return 0;
}
