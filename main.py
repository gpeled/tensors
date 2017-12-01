
import sys
print(sys.path)

import gpuutilmodule as gu

print("Starting to use gpuutilmodule.so through direct import");
u = gu.getUtil()
print("Utilization returned is %d" % (u))
print("Done")
