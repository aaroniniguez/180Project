x = []
y = []
z = []
f = open("straight_linez.txt", "r")
lines = f.readlines()[1:-1]
for line in lines:
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
top = ()
#dummy variable
dif = 201
for idx,val in enumerate(z):
    if val > 2:
        top = (1,idx)
    if top and val < -2:
            dif = idx - top[1]
    #        print dif

if dif < 200:
    print "1,"+str(int(sum([i*i for i in z])))
else:
    print "0,0"
