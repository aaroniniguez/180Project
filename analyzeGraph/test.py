x = []
y = []
z = []
for line in open("straight_linez.txt","r"):
    x.append(line.replace("\n","").split(",")[0])
    y.append(line.replace("\n","").split(",")[1])
    z.append(line.replace("\n","").split(",")[2])
x.pop(0)
y.pop(0)
z.pop(0)
x = map(float,x)
z =map(float,z)
y =map(float,y)
#z should be >1.5 difference
#y should be <.5
#x should be <.5
avgVal = sum(x)/len(x)
x = [i-avgVal for i in x]
avgVal = sum(y)/len(y)
y = [i-avgVal for i in y]
avgVal = sum(z)/len(z)
z = [i-avgVal for i in z]
mydict = {}
for idx,val in enumerate(z):
    if val > 1.5:
        if y[idx] < .5:
            if x[idx] <.5:
                mydict[idx] = val, y[idx],x[idx]
print mydict
