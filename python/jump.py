def writePls(wf,data):
    count = 0
    for i in range(0,len(data)):
        for j in range(0,len(data[i])):
            if (j < len(data[i])-1):
                wf.write(str(data[i][j])+',')
            else:
                wf.write(str(data[i][j])+'\n')

data = []
header = ["rArm","rArm2","rFore","rFore2","rFore3","lArm","lArm2","lFore",
          "lFore2","lFore3","head","head2","rLeg","rLeg2",
          "rCalf","rFoot","lLeg","lLeg2","lCalf","lFoot","bodi","bodUD","bodFB"]
#data.append(header)
rArm = 0.0
rArm2 = 0.0
rFore = 0.0
rFore2 = 0.0
rFore3 = 0.0
lArm = 0.0
lArm2 = 0.0
lFore = 0.0
lFore2 = 0.0
lFore3 = 0.0
head = 0.0
head2 = 0.0
bodi = 0.0
bodUD = 0.0
bodFB = 0.0
rLeg = 0.0
rLeg2 = 0.0
rCalf = 0.0
lLeg = 0.0
lLeg2 = 0.0
lCalf = 0.0
rFoot = 0.0
lFoot = 0.0
#goPos
for i in range(0,10):
    head2 = head2 - 0.065
    bodFB = bodFB + 0.055
    bodUD = bodUD - 0.009
    rLeg = rLeg - 0.14
    lLeg = lLeg - 0.14
    rCalf = rCalf + 0.255
    lCalf = lCalf + 0.255
    rFoot = rFoot - 0.14
    lFoot = lFoot - 0.14
    rArm = rArm + 0.005
    lArm = lArm + 0.005
    rFore = rFore - 0.165
    lFore = lFore - 0.165
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
#goUp
for i in range(0,10):
    head2 = head2 + 0.065
    bodFB = bodFB - 0.055
    bodUD = bodUD + 0.03
    
    rLeg = rLeg + 0.14
    lLeg = lLeg + 0.14
    rCalf = rCalf - 0.255
    lCalf = lCalf - 0.255
    rFoot = rFoot + 0.14
    lFoot = lFoot + 0.14
    
    rArm = rArm - 0.3
    lArm = lArm - 0.3
    rFore = rFore + 0.165
    lFore = lFore + 0.165
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
#goDown
for i in range(0,10):
    head2 = head2 - 0.065
    bodFB = bodFB + 0.055
    bodUD = bodUD - 0.03
    
    rLeg = rLeg - 0.14
    lLeg = lLeg - 0.14
    rCalf = rCalf + 0.255
    lCalf = lCalf + 0.255
    rFoot = rFoot - 0.14
    lFoot = lFoot - 0.14
    
    rArm = rArm + 0.3
    lArm = lArm + 0.3
    rFore = rFore - 0.165
    lFore = lFore - 0.165
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
#goNeg
for i in range(0,10):
    head2 = head2 + 0.065
    bodFB = bodFB - 0.055
    bodUD = bodUD + 0.009
    rLeg = rLeg + 0.14
    lLeg = lLeg + 0.14
    rCalf = rCalf - 0.255
    lCalf = lCalf - 0.255
    rFoot = rFoot + 0.14
    lFoot = lFoot + 0.14
    rArm = rArm - 0.005
    lArm = lArm - 0.005
    rFore = rFore + 0.165
    lFore = lFore + 0.165
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)

for i in range(0,len(data)):
    for j in range(0,len(data[i])):
        if (abs(data[i][j]) < 0.000001):
            #print data[i][j]
            data[i][j] = 0.0
                     
wf = open('positions.txt','w')
writePls(wf, data)
wf.close()

#print data
