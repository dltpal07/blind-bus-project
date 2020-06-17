import os.path
import scipy.io
import os
import subprocess
import sys
from time import sleep


def get_int(a,b,c,d):
    print("\n\n ---------- start finding bus number by matlab ---------- \n")
    os.chdir("C:\\semi\\")
    sys.path.append("C:\\Program Files\\MATLAB\\MATLAB Runtime\\v98\\runtime\\win64")
    mat = {'point':[a,b,c,d]}
    
    scipy.io.savemat('finmat.mat', mat)
    print("> picture's coordinates are saved") 
    sleep(5)
   
    
    #os.system("numdetect.exe")
    subprocess.call(["numdetect.exe"])
    while(1):
        if(os.path.isfile('busmat.mat') == True):
            sleep(5)
            mat_file = scipy.io.loadmat('busmat.mat')
            break
        
    print("> found bus number in this picture")
    list = []
    a = mat_file['fin']
    for i in a:
        if i>='0' and i<='9' or i=='-':
                list.append(i)
                
    b = ("".join(map(str, list)))
    b = b.rstrip('\n')
    if os.path.isfile('busmat.mat') == True:
        os.remove('busmat.mat')
    os.chdir("C:\\Program Files (x86)\\Intel RealSense SDK 2.0\\samples\\x64\\Debug\\")
    print("\n ---------- finish finding bus number by matlab ---------- \n\n")
    return b

