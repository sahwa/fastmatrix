import pandas as pd
import numpy as np
import os

np.random.seed(123)

def make_tests():
  for i in range(1, 4):
      matrix = pd.DataFrame(np.random.rand(10, 10))
      out = "test.chr" + str(i) + ".chunkcounts.out"
      matrix.to_csv(out, sep=' ', index=False, header=False)

def run_tests():
  bin = "../bin/"

  infiles = [f"test.chr{i}.chunkcounts.out" for i in range(1, 4)]
  
  infile_str = ",".join(infiles)
  
  output = "test.AllChr.chunkcounts.out"
  
  cmd = f"{bin}fastmatrix -f {infile_str} -o {output}"
  
  os.system(cmd)

make_tests()
run_tests()
