import numpy as np
import sys
from pathlib import Path


def main(N, M):
  print(N)
  N = int(N)
  print(N)
  M = int(M)
  input = np.random.uniform(-50,50,N)
  kernel = np.random.uniform(-50,50,M)
  
  conv_result = np.convolve(input,kernel)
  
  # Write the header
  base_dir = Path(__file__).resolve().parent
  output_file = base_dir / 'test_data.h'
  
  with open(output_file, 'w') as fd:
    fd.write(f"#define N {N}\n")
    fd.write(f"#define M {M}\n") 

    fd.write(f"float A[{N}] = {{")
    for i, a_i in enumerate(input):
      fd.write(f"{a_i:.20f},")
    fd.write("};\n")

    fd.write("\n")
    
    fd.write(f"float B[{M}] = {{")
    for i, a_i in enumerate(kernel):
      fd.write(f"{a_i:.20f},")
    fd.write("};\n")

    fd.write("\n")

    fd.write(f"float R[{M+N-1}] = {{")
    for i, a_i in enumerate(conv_result):
      fd.write(f"{a_i:.20f},")

    fd.write("};\n")

if __name__ == "__main__":
  main(sys.argv[1], sys.argv[2])            




