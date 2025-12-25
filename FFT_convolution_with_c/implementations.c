#include <math.h>
#include <complex.h>
#include <stdlib.h>

void fft_recursion(float complex* Input, int N, float complex* Output)
{
  if(N == 1)
  {
    Output[0] = Input[0];
    return;
  }
  
  // Even/odds split
  float complex* evens = malloc(N/2 * sizeof(float complex));
  float complex* odds = malloc(N/2 * sizeof(float complex));
  int counter = 0;
  for(int i = 0; i < N; i+=2)
  {
    evens[counter] = Input[i];
    odds[counter++] = Input[i+1];
  }

  // FFT for the evens/odds
  float complex* evensFFT = malloc(N/2 * sizeof(float complex));
  fft_recursion(evens, N/2, evensFFT);

  float complex* oddsFFT = malloc(N/2 * sizeof(float complex));
  fft_recursion(odds, N/2, oddsFFT);
  
  float complex W_N = cexpf(2*M_PI*I / N);
  for(int i = 0; i < N; i++)
  {
    Output[i] = evensFFT[i % (N/2)] + cpowf(W_N,-i)*oddsFFT[i % (N/2)];
  }

  free(evens);
  free(odds);
  free(evensFFT);
  free(oddsFFT);
}

void fft(float complex* Input, int N, float complex* Output)
{
  fft_recursion(Input, N, Output);
}

void ifft_recursion(float complex* Input, int N, float complex* Output)
{
  if(N == 1)
  {
    Output[0] = Input[0];
    return;
  }
  
  // Even/odds split
  float complex* evens = malloc(N/2 * sizeof(float complex));
  float complex* odds = malloc(N/2 * sizeof(float complex));
  int counter = 0;
  for(int i = 0; i < N; i+=2)
  {
    evens[counter] = Input[i];
    odds[counter++] = Input[i+1];
  }

  // FFT for the evens/odds
  float complex* evensIFFT = malloc(N/2 * sizeof(float complex));
  ifft_recursion(evens, N/2, evensIFFT);

  float complex* oddsIFFT = malloc(N/2 * sizeof(float complex));
  ifft_recursion(odds, N/2, oddsIFFT);
  
  float complex W_N = cexpf(-2*M_PI*I / N);
  for(int i = 0; i < N; i++)
  {
    Output[i] = evensIFFT[i % (N/2)] + cpowf(W_N,-i)*oddsIFFT[i % (N/2)];
  }
  
  free(evens);
  free(odds);
  free(evensIFFT);
  free(oddsIFFT);
}


void ifft(float complex* Input, int N, float complex* Output)
{
  ifft_recursion(Input, N, Output);
  for(int i = 0; i < N; i++)
  {
    Output[i] /= (float)N;
  }
}

void convolution_naive(float* Input, float* Kernel, int N, int M, float* Output)
{
  for (int i = 0; i < N+M-1; i++) 
  {
    float acc = 0;
    for (int m = 0; m < M; m++) 
      if (i - m >= 0 && i - m < N) 
        acc += Kernel[m] * Input[i - m];
    Output[i] = acc;
  }
}

void convolution_optimised(float* Input, float* Kernel, int N, int M, float* Output)
{
  for(int i = 0; i < N; i++)
  {
    int m = 0;
    for(; m < M - 3; m+=4)
    {
      Output[i+m]   += Input[i] * Kernel[m]; 
      Output[i+m+1] += Input[i] * Kernel[m+1]; 
      Output[i+m+2] += Input[i] * Kernel[m+2]; 
      Output[i+m+3] += Input[i] * Kernel[m+3]; 
    }
    for(; m < M; m++)
      Output[i+m] += Input[i] * Kernel[m];
  }
}

void convolution_with_fft(float* Input, float* Kernel,
                          int N, int M, float* Output)
{
    int L = 1;
    while (L < N + M - 1)
        L <<= 1;
    float complex* complexInput  = calloc(L, sizeof(float complex));
    float complex* complexKernel = calloc(L, sizeof(float complex));

    for (int i = 0; i < N; i++)
        complexInput[i] = Input[i];

    for (int i = 0; i < M; i++)
        complexKernel[i] = Kernel[i];


    float complex* inputFFT  = malloc(L * sizeof(float complex));
    float complex* kernelFFT = malloc(L * sizeof(float complex));
    fft(complexInput,  L, inputFFT);
    fft(complexKernel, L, kernelFFT);

    float complex* outputFFT = malloc(L * sizeof(float complex));
    for (int i = 0; i < L; i++)
        outputFFT[i] = inputFFT[i] * kernelFFT[i];

    float complex* outputIFFT = malloc(L * sizeof(float complex));
    ifft(outputFFT, L, outputIFFT);

    for (int i = 0; i < N + M - 1; i++)
        Output[i] = crealf(outputIFFT[i]);

    free(complexInput);
    free(complexKernel);
    free(inputFFT);
    free(kernelFFT);
    free(outputFFT);
    free(outputIFFT);
}

void convolution_complex(float* Input, float* Kernel, int N, int M, float* Output)
{
}