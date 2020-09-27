#include "dtmf.h"

u16 buffer_table[tableSize]; //256;

double freq[16][2] = {
    941, 1336, //0
    697, 1209, //1
    697, 1336, //2
    697, 1477, //3
    770, 1209, //4
    770, 1336, //5
    770, 1477, //6
    852, 1209, //7
    852, 1336, //8
    852, 1477, //9
    697, 1633, //A
    770, 1633, //B
    852, 1633, //C
    941, 1633, //D
    941, 1209, //*
    941, 1477, //#
};

void generate(int num){
    double row_freq, col_freq;
    int k = 0;
    u16 n = 0;
    u16 temp = 1023;
    double row_sinwave;
    double col_sinwave;
    row_freq = freq[num][0] / 8000;
    col_freq = freq[num][1] / 8000;
    for (n = 0; n < tableSize; n++)
    {
        row_sinwave = sin(2 * PI * n * row_freq);
        col_sinwave = sin(2 * PI * n * col_sinwave);
        buffer_table[n] = (row_sinwave + col_sinwave) * (temp);
    }
}