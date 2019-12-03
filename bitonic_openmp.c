#include <stdio.h>
#include <stdlib.h>

#define TAM 1024
int DOWN = 1, UP = -1;

void geraAleatorios(int numero[])
{
    srand(time(NULL));
    int valor;

    for (int i = 0; i < TAM; i++)
    {
        valor = rand() % 1000;
        numero[i] = valor;
    }
}

void swap(int a[], int i, int j, int dir)
{
    int test = (dir == DOWN && a[i] < a[j]) || (dir == UP && a[i] > a[j]);
    if (test)
    {
        int k = a[j];
        a[j] = a[i];
        a[i] = k;
    }
}

void bitonic_internal(int num[], int ini, int tam, int dir)
{
    int passo, i, j;
    for (passo = tam; passo > 1; passo /= 2)
    {
        for (j = 0; j < tam / passo; j++)
        {
            for (i = passo * j; i < passo * j + passo / 2; i++)
            {
                swap(num, ini + i, ini + i + passo / 2, dir);
            }
        }
    }
}

void bitonic(int num[])
{
    int passo, i, dir = UP;
    for (passo = 2; passo <= TAM; passo *= 2)
    {
        #pragma omp parallel for shared(num)
        for (i = 0; i < TAM; i += passo)
        {
            if (i == 0 && passo != 2)
            {
                dir = DOWN;
            }
            bitonic_internal(num, i, passo, dir);
            dir *= -1;
        }
    }
}

int main(void)
{
    int i, j, valores[TAM];

    geraAleatorios(valores);
    bitonic(valores);
    // for (i = 0; i < TAM; i++)
    // {
    //     printf("%d: %d\n", i, valores[i]);
    // }
}