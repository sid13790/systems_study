#! /usr/bin/env python3
import os
import argparse
import subprocess
import matplotlib.pyplot as plt
import numpy as np

parser = argparse.ArgumentParser()
parser.add_argument("trials", help="set iteration number")
args = parser.parse_args()

if not os.path.isfile("./tlb"):
    subprocess.run(["make"])

y = []
a = np.arange(14)
x = 2**a
for i in x:
    result = subprocess.run(["./tlb", str(i), args.trials], stdout=subprocess.PIPE)
    result = result.stdout.decode('utf-8').strip()
    print(f"i: {i}, trials: {args.trials} result: {result}")
    y.append(float(result))

fig = plt.figure()
plt.plot(a, y, marker='o', color='orange')
plt.margins(0)
plt.xticks(a, x) # evenly spaced
plt.xlabel('Number Of Pages')
plt.ylabel('Time Per Access (ns)')
plt.title('TLB Size Measurement')
plt.savefig('tlb.png', dpi=227)
# plt.show()
