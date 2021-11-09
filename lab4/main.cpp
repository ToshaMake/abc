#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include "hpctimer.h"

const int buf_size = 10;

double time_buf[buf_size];

void dgemm_def(double *a, double *b, double *c, int n)
{
    int i, j, k;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            for (k = 0; k < n; k++)
            {
                *(c + i * n + j) += *(a + i * n + k) * *(b + k * n + j);
            }
        }
    }
}

void dgemm_opt_1(double *a, double *b, double *c, int n)
{
    int i, j, k;

    for (i = 0; i < n; i++)
    {
        for (k = 0; k < n; k++)
        {
            for (j = 0; j < n; j++)
            {
                *(c + i * n + j) += *(a + i * n + k) * *(b + k * n + j);
            }
        }
    }
}

void dgemm_opt_2(double *a, double *b, double *c, int n)
{
    int i, j, k, i0, k0, j0;
    double *c0, *b0, *a0;
    int BS = 64;

    for (i = 0; i < n; i += BS)
    {
        for (j = 0; j < n; j += BS)
        {
            for (k = 0; k < n; k += BS)
            {
                for (i0 = 0, c0 = (c + i * n + j), a0 = (a + i * n + k); i0 < BS; ++i0, c0 += n, a0 += n)
                {
                    for (k0 = 0, b0 = (b + k * n + j); k0 < BS; ++k0, b0 += n)
                    {
                        for (j0 = 0; j0 < BS; ++j0)
                        {
                            c0[j0] += a0[k0] * b0[j0];
                        }
                    }
                }
            }
        }
    }
}

void dgemm_opt_3(double *a, double *b, double *c, int n)
{
    int i, j, k;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n - 7; j += 8)
        {
            for (k = 0; k < n - 7; k += 8)
            {
                *(c + i * n + j) += *(a + i * n + k) * *(b + k * n + j);
                *(c + i * n + j + 1) += *(a + i * n + k + 1) * *(b + k * n + j + 1);
                *(c + i * n + j + 2) += *(a + i * n + k + 2) * *(b + k * n + j + 2);
                *(c + i * n + j + 3) += *(a + i * n + k + 3) * *(b + k * n + j + 3);
                *(c + i * n + j + 4) += *(a + i * n + k + 4) * *(b + k * n + j + 4);
                *(c + i * n + j + 5) += *(a + i * n + k + 5) * *(b + k * n + j + 5);
                *(c + i * n + j + 6) += *(a + i * n + k + 6) * *(b + k * n + j + 6);
                *(c + i * n + j + 7) += *(a + i * n + k + 7) * *(b + k * n + j + 7);
            }
        }
    }
}

void init_matrix(double *a, double *b, double *c, int n)
{
    int i, j, k;

    for (i = 0; i < n - 7; i+=8)
    {
        for (j = 0; j < n - 7; j+=8)
        {
            *(a + i * n + j) = 1.0;
            *(b + i * n + j) = 2.0;
            *(c + i * n + j) = 0.0;

            *(a + (i + 1) * n + j + 1) = 1.0;
            *(b + (i + 1) * n + j + 1) = 2.0;
            *(c + (i + 1) * n + j + 1) = 0.0;

            *(a + (i + 2) * n + j + 2) = 1.0;
            *(b + (i + 2) * n + j + 2) = 2.0;
            *(c + (i + 2) * n + j + 2) = 0.0;

            *(a + (i + 3) * n + j + 3) = 1.0;
            *(b + (i + 3) * n + j + 3) = 2.0;
            *(c + (i + 3) * n + j + 3) = 0.0;

            *(a + (i + 4) * n + j + 4) = 1.0;
            *(b + (i + 4) * n + j + 4) = 2.0;
            *(c + (i + 4) * n + j + 4) = 0.0;

            *(a + (i + 5) * n + j + 5) = 1.0;
            *(b + (i + 5) * n + j + 5) = 2.0;
            *(c + (i + 5) * n + j + 5) = 0.0;

            *(a + (i + 6) * n + j + 6) = 1.0;
            *(b + (i + 6) * n + j + 6) = 2.0;
            *(c + (i + 6) * n + j + 6) = 0.0;

            *(a + (i + 7) * n + j + 7) = 1.0;
            *(b + (i + 7) * n + j + 7) = 2.0;
            *(c + (i + 7) * n + j + 7) = 0.0;
        }
    }
}


int main(int argc, char *argv[])
{
    int i = atoi(argv[2]);
    double t = 0;

    int size = atoi(argv[1]);

    double *A = (double *)malloc(sizeof(double) * size * size);
    double *B = (double *)malloc(sizeof(double) * size * size);
    double *C = (double *)malloc(sizeof(double) * size * size);

    init_matrix(A, B, C, size);
    switch (i)
    {
    case 0:
        t = hpctimer_getwtime();
        dgemm_def(A, B, C, size);
        t = hpctimer_getwtime() - t;
        std::cout << "dgemm_opt_" << i << ' ' << "Size:" << size << "\n";
        std::cout << t << '\n';
        break;
    case 1:
        t = hpctimer_getwtime();
        dgemm_opt_1(A, B, C, size);
        t = hpctimer_getwtime() - t;
        std::cout << "dgemm_opt_" << i << ' ' << "Size:" << size << "\n";
        std::cout << t << '\n';
        break;

    case 2:
        t = hpctimer_getwtime();
        dgemm_opt_2(A, B, C, size);
        t = hpctimer_getwtime() - t;
        std::cout << "dgemm_opt_" << i << ' ' << "Size:" << size << "\n";
        std::cout << t << '\n';
        break;

    case 3:
        t = hpctimer_getwtime();
        dgemm_opt_3(A, B, C, size);
        t = hpctimer_getwtime() - t;
        std::cout << "dgemm_opt_" << i << ' ' << "Size:" << size << "\n";
        std::cout << t << '\n';
        break;
    }

    free(A);
    free(B);
    free(C);

    return 0;
}