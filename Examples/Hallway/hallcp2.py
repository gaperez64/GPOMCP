import os
for i in range(1,10):
	os.system("python genHallway.py hallway"+str(i)+".txt hallway"+str(i)+".POMDP")

