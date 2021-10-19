#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <sys/time.h>

using namespace std;

ofstream result;
int launch_count;
string mt;
string lc;
string data_cap;
string name;
int I = 0;
double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}
long long Kbtob(long long number)
{
    return number << 10;
}
long long Mbtob(long long number)
{
    return number << 20;
}
void run_ssd(int count)
{
    int numb = count / 8;
    int* array = new int[numb];
    for (int i = 0; i < numb; i++) {
        array[i] = rand();
    }
    ofstream write;
    ifstream read;
    double* t = new double[launch_count];
    double* r = new double[launch_count];
    double T = wtime();
    for (int j = 0; j < launch_count; j++) {
        write.open("file.txt");
        t[j] = wtime();
        for (int i = 0; i < numb; i++) {
            write << array[i];
        }
        t[j] = wtime() - t[j];
        write.close();
        cout << t[j] << '\n';
    }
    T = wtime() - T;
    double R = wtime();
    cout << "total: " << T << '\n';
    for (int j = 0; j < launch_count; j++) {
        read.open("file.txt");
        r[j] = wtime();
        while (!read.eof()) {
            read.get();
        }
        r[j] = wtime() - r[j];
        read.close();
        cout << r[j] << '\n';
    }
    R = wtime() - R;
    cout << "total: " << R << '\n';
    result << mt << ';' << count << ';' << "int;" << count << ';' << I
           << ";gettimeofday;" << t[0] << ';' << T / launch_count << ';'
           << count / T * launch_count * 1000000 << ';'
           << abs(t[0] - T / launch_count) << ';'
           << abs((t[0] - T / launch_count) / t[0]) << ';' << r[0] << ';'
           << R / launch_count << ';' << count / R * launch_count * 1000000
           << ';' << abs(r[0] - R / launch_count) << ';'
           << abs((r[0] - R / launch_count) / r[0]) << ';' << '\n';
}
void run_ram(int count)
{
    int numb = count / 8;
    int* array = new int[numb];
    for (int i = 0; i < numb; i++)
        array[i] = rand();
    int* buff = new int[numb];
    double* t = new double[launch_count];
    double* r = new double[launch_count];
    double T = wtime();
    for (int j = 0; j < launch_count; j++) {
        t[j] = wtime();
        for (auto i = 0; i < numb; i++)
            buff[i] = array[i];
        t[j] = wtime() - t[j];
        cout << t[j] << '\n';
    }
    T = wtime() - T;
    cout << "total: " << T << '\n';
    double R = wtime();
    for (int j = 0; j < launch_count; j++) {
        r[j] = wtime();
        *(array + j);
        r[j] = wtime() - r[j];
        cout << r[j] << '\n';
    }
    R = wtime() - R;
    cout << "total: " << R << '\n';
    result << mt << ';' << count << ';' << "int;" << count << ';' << count
           << ";gettimeofday;" << t[0] << ';' << T / launch_count << ';'
           << count / T * launch_count * 1000000 << ';'
           << abs(t[0] - T / launch_count) << ';'
           << abs((t[0] - T / launch_count) / t[0]) << ';' << r[0] << ';'
           << R / launch_count << ';' << count / R * launch_count * 1000000
           << ';' << abs(r[0] - R / launch_count) << ';'
           << abs((r[0] - R / launch_count) / r[0]) << ';' << '\n';
}
void run_flush(int count)
{
    int numb = count / 8;
    int* array = new int[numb];
    for (int i = 0; i < numb; i++)
        array[i] = rand();
    ofstream write;
    ifstream read;
    double* t = new double[launch_count];
    double* r = new double[launch_count];
    double T = wtime();
    for (int j = 0; j < launch_count; j++) {
        write.open("file.txt");
        t[j] = wtime();
        for (int i = 0; i < numb; i++)
            write << array[i];
        t[j] = wtime() - t[j];
        write.close();
        cout << t[j] << '\n';
    }
    T = wtime() - T;
    double R = wtime();
    cout << "total: " << T << '\n';
    for (int j = 0; j < launch_count; j++) {
        read.open("file.txt");
        r[j] = wtime();
        while (!read.eof()) {
            read.get();
        }
        r[j] = wtime() - r[j];
        read.close();
        cout << r[j] << '\n';
    }
    R = wtime() - R;
    cout << "total: " << R << '\n';
    result << mt << ';' << count << ';' << "int;" << count << ';' << I
           << ";gettimeofday;" << t[0] << ';' << T / launch_count << ';'
           << count / T * launch_count * 1000000 << ';'
           << abs(t[0] - T / launch_count) << ';'
           << abs((t[0] - T / launch_count) / t[0]) << ';' << r[0] << ';'
           << R / launch_count << ';' << count / R * launch_count * 1000000
           << ';' << abs(r[0] - R / launch_count) << ';'
           << abs((r[0] - R / launch_count) / r[0]) << ';' << '\n';
}
int main(int argc, char** argv)
{
    long long size;
    data_cap = argv[4];
    mt = argv[2];
    lc = argv[6];
    string name = argv[2];
    name.append(".csv");

    launch_count = stoi(lc, nullptr);
    result.open(name);
    result << "MemoryType;BlockSize;ElementType;BufferSize;LaunchNum;Timer;"
              "WriteTime;AverageWriteTime;WriteBandwidth;AbsError;"
              "RelError;ReadTime;AverageReadTime;ReadBandwidth;AbsError;"
              "RelError;"
              "\n";

    if (*(data_cap.end() - 2) == 'M' || *(data_cap.end() - 2) == 'm') {
        int number = stoi(data_cap, nullptr);
        size = Mbtob(number);
    } else if (*(data_cap.end() - 2) == 'K' || *(data_cap.end() - 2) == 'k') {
        int number = stoi(data_cap, nullptr);
        size = Kbtob(number);
    } else {
        size = stoi(data_cap, nullptr);
    }
    if (mt == "RAM")
        run_ram(size);
    else if (mt == "SSD")
        run_ssd(size);
    else if (mt == "flush")
        run_flush(size);
    else if (mt == "SSD_MB")
        for (I = 0; I < 20; I++) {
            run_ssd(Mbtob(4 + I * 4));
        }
    else if (mt == "RAM_CACHE") {
        I = 0;
        run_ram(64);
        I = 1;
        run_ram(Kbtob(128));
        I = 2;
        run_ram(Kbtob(512));
        I = 3;
        run_ram(Mbtob(3));
    } else if (mt == "flush_MB") {
        for (I = 0; I < 20; I++) {
            run_flush(Mbtob(4 + I * 4));
        }
    } else if (mt == "SSD_CL") {
        for (; launch_count < 30; launch_count += 5) {
            I = launch_count;
            run_ssd(Mbtob(1));
        }
    }
    result.close();
    return 0;
}