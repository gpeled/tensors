
import sys
import torch
import datetime

import gpuutil as gu

u = gu.getUtil()
print("GPU utilization %d%%" % (u))


d1=1024
d2=1024
x = torch.rand(d1, d2).cuda()
y = torch.rand(d1, d2).cuda()

start_time=datetime.datetime.now()
print("Starting math. Time: %s"% start_time)
#result = torch.Tensor(d1, d2)
for i in range(100000):
    y.add_(x)
    #y=result
    #z=x.std()
    if not(i % 100):
        if (i % 4000):
            print(".",end="")
            sys.stdout.flush()
        else:
            print("GPU Util %d%%\n%d " % (gu.getUtil(),i),end="")

end_time = datetime.datetime.now()
print("\nDONE. Time: %s" % end_time)
print("Time spent in math: %s" % (end_time-start_time))
