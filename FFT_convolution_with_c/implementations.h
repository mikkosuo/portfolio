#pragma once
#include <complex.h>


void fft(float complex* Input, int N_size, float complex* Output);
void ifft(float complex* Input, int N_size, float complex* Output);
void convolution_naive(float* Input, float* Kernel, int N_size, int M_size, float* Output);
void convolution_optimised(float* Input, float* Kernel, int N_size, int M_size, float* Output);
void convolution_with_fft(float* Input, float* Kernel, int N_size, int M_size, float* Output);
void convolution_complex(float* Input, float* Kernel, int N_size, int M_size, float* Output);
void convolution_with_fft_complex();
