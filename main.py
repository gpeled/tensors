
import sys
print(sys.path)

import gpuutil as gu

u = gu.getUtil()
print("Utilization returned is %d" % (u))
