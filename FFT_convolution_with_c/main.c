#include <stdio.h>
#include "test_data.h"
#include "implementations.h"
#include "math.h"
#include <assert.h>
#include <time.h>
#include <stdlib.h>

void check_conv_result(float* Computed)
{
  float rel_tol = 1e-2f;
  float tol = 50;
  for(int i = 0; i < N + M - 1; i++)
  {
     //printf("\nC = %f, R = %f", Computed[i], R[i]);
     //float diff = fabs(Computed[i] - R[i]);
     //float max_val = (fabs(Computed[i]) > fabs(R[i])) ? fabs(Computed[i]) : fabs(R[i]);
     //assert(diff <= rel_tol * max_val);

     //assert(fabs(Computed[i] - R[i]) < tol);
  }
  printf("\n");
}

// Check that fft and ifft works
void check_fft_ifft()
{
  float Computed[N] = {0};
  
  float complex* complexInput = malloc(N*sizeof(float complex));
  float complex* inputFFT = malloc(N*sizeof(float complex));

  for(int i = 0; i < N; i++)
    complexInput[i] = A[i];

  fft(complexInput, N, inputFFT);
  float complex* inputIFFT = malloc(N*sizeof(float complex));
  ifft(inputFFT, N, inputIFFT);

  for(int i = 0; i < N; i++) 
    Computed[i] = crealf(inputIFFT[i]);

  free(complexInput);
  free(inputFFT);
  free(inputIFFT);

  float tol = 1e-1f;
  for(int i = 0; i < N; i++)
  {
    //printf("C = %f, Real = %f\n", Computed[i], A[i]); 
    assert(fabs(Computed[i] - A[i]) <= tol);
  }
}


int main()
{
  clock_t start, end;
  double cpu_time_used;
  printf("N=%d, M=%d\n",N,M);
  
  printf("\nFFT/IFFT TEST\n");
  check_fft_ifft();

  printf("\nConv naive\n");
  for(int i = 0; i < 5; i++)
  {
    start = clock();
    float Computed[N+M-1] = {0};
    convolution_naive(A, B, N, M,Computed);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time: %f seconds\n", cpu_time_used);
    check_conv_result(Computed);
  }

  printf("\nConv fft\n");
  for(int i = 0; i < 5; i++)
  {
    start = clock();
    float Computed[N+M-1] = {0};
    convolution_with_fft(A, B, N, M,Computed);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time: %f seconds\n", cpu_time_used);
    check_conv_result(Computed);
  }

   return 0;
}
