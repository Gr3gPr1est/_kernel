import ctypes, sys
from ctypes import *
import io
from itertools import product
from sys import argv


with open("devicelist.txt") as f:
    for line in f:
        for word in line.split():

            devicename = word
            kernel32 = windll.kernel32
            hevDevice = kernel32.CreateFileA("\\\\.\\"+str(devicename), 0xC0000000, 0, None, 0x3, 0, None)


            if not hevDevice or hevDevice == -1:
                print (""),
                hevDevice = kernel32.CreateFileA("\\\\.\\GLOBALROOT\\Device\\"+str(devicename), 0xC0000000, 0, None, 0x3, 0, None)
                if not hevDevice or hevDevice == -1:
                    print ("*** Couldn't get Device Driver handle.")
                else:
                    print ("2nd OPENED!")
                    print (devicename)
                    file = open("target_drivers.txt","a")
                    file.write(devicename+"\n")
                    file.close()
            else:
                print (devicename)
                file = open("target_drivers.txt","a")
                file.write(devicename+"\n")
                file.close()

