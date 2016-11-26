from shutil import copyfile
import os
for i in range(1,10):
	os.system("mkdir "+"./hallway"+str(i))
	os.system("python genHallway.py "+"maph"+str(i)+".txt "+"hallway"+str(i)+".POMDP")
	os.system("mv maph"+str(i)+ ".txt ./hallway"+str(i))
	os.system("mv hallway"+str(i)+ ".POMDP ./hallway"+str(i))

