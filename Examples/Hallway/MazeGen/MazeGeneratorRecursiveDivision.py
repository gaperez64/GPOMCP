import random
import numpy

def isIn(i,j,num_rows,num_cols):
    b = 1
    if (i < 0) or (j < 0) or (i >= num_rows) or (j >= num_cols):
        b=0;
    return b;


def Modify(M,x_ul,y_ul,x_dr,y_dr):
    if (x_dr - x_ul < 5) or (y_dr - y_ul < 5): pass
    else:
        x_cut = random.randint(x_ul+2,x_dr-2) if not (x_ul + 2 == x_dr - 2) else x_ul + 2
        y_cut = random.randint(y_ul+2,y_dr-2) if not (y_ul + 2 == y_dr - 2) else y_ul + 2
        
        hole_x1 = random.randint(x_ul+1,x_cut-1) if not (x_ul + 1 == x_cut - 1) else x_ul + 1
        hole_x2 = random.randint(x_cut+1,x_dr-1) if not (x_cut + 1 == x_dr - 1) else x_cut + 1
        hole_y1 = random.randint(y_cut+1,y_dr-1) if not (y_cut + 1 == y_dr - 1) else y_cut + 1
        
        for i in range(0,x_dr-x_ul): M[x_ul+i,y_cut] = '1'
        for i in range(0,y_dr-y_ul): M[x_cut,y_ul+i] = '1'
        
        for i in range(0,x_dr-x_ul): M[x_ul+i,y_ul] = '0'
        for i in range(0,x_dr-x_ul): M[x_ul+i,y_dr] = '0'
        for i in range(0,y_dr-y_ul): M[x_ul,y_ul+i] = '0'
        for i in range(0,y_dr-y_ul): M[x_dr,y_ul+i] = '0'
        
        M[hole_x1,y_cut] = '0'
        M[hole_x2,y_cut] = '0'
        M[x_cut,hole_y1] = '0'
        
        Modify(M,x_ul,y_ul,x_cut-1,y_cut-1)
        Modify(M,x_cut+1,y_ul,x_dr,y_cut-1)
        Modify(M,x_ul,y_cut+1,x_cut-1,y_dr)
        Modify(M,x_cut+1,y_cut+1,x_dr,y_dr)


#input
num_rows = int(input("Rows-2: "))
num_cols = int(input("Columns-2: "))

#generate a maze
M = numpy.chararray((num_rows,num_cols))
for i in range(0,num_rows):
    for j in range(0,num_cols):
        M[i,j] = '0';
        
Modify(M,0,0,num_rows-1,num_cols-1)

nowalls = []

for i in range(0,num_rows):
    for j in range(0,num_cols):
        if (M[i,j] == '0'): nowalls.append([i,j])
        
number = len(nowalls)
random.shuffle(nowalls)

u = nowalls[0]
M[u[0],u[1]] = '+'
u = nowalls[1]
M[u[0],u[1]] = 'x'
u = nowalls[2]
M[u[0],u[1]] = 'g'

for i in range(3,number-1):
    k = random.randint(1,60)
    if (k == 1):
        u = nowalls[i]
        M[u[0],u[1]] = '+'
    elif (k == 2):
        u = nowalls[i]
        M[u[0],u[1]] = 'x'
    elif (k == 3):
        u = nowalls[i]
        M[u[0],u[1]] = 'g'

#output
s1 = '1'
for i in range(0,num_cols+1):
    s1 = s1 + ' 1'
f = open('E:\workfile.txt', 'w')
f.write(s1+"\n")
for i in range(0,num_rows-1):
    s = '1 ' + M[i,0]
    for j in range(1,num_cols):
        s = s + ' ' + M[i,j]
    s = s + ' 1'
    f.write(s+"\n")
    
s = '1 ' + M[num_rows-1,0]
for j in range(1,num_cols):
    s = s + ' ' + M[num_rows-1,j]
s = s + ' 1'
f.write(s+"\n")
f.write(s1)
f.close()