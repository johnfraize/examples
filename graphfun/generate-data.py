import numpy as np
import argparse
import random

parser = argparse.ArgumentParser(description="generate data for graphfun")
parser.add_argument('-c', '--count', type=int, default=500)
parser.add_argument('-y', '--ymax', type=int, default=127)
args = parser.parse_args()

data = [0,0,0,0]

# genrate the data needed for Gabe test
i = 0;
for x in data:
    data[i] = np.random.randint(low=0,high=args.ymax, size=args.count, dtype=int)
    i+=1

# for each row 
for n in range(args.count):
    # now lets add the column data
    line = ""
    for i in range(4):
        n = data[i][n]
        line+= str(n)
        if i < 3:
            line+=","
    print("{}".format(line))
        
    

print("data {} {}".format(line,args.count))
