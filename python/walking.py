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

#armsReady
for i in range(0,5):
    rArm2 = rArm2 - 0.2
    lArm2 = lArm2 + 0.2
    rFore2 = rFore2 + 0.37
    lFore2 = lFore2 - 0.37
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
#leftLeg1Up
for i in range(0,2):
    bodUD = bodUD - 0.001
    
    rLeg = rLeg - (0.15 / 2.0)
    lLeg = lLeg - (1.5 / 4.0)
    rCalf = rCalf + (0.3 / 2.0)
    lCalf = lCalf + (1.7 / 4.0)
    rFoot = rFoot - (0.2 / 2.0)
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
for i in range(0,2):
    bodUD = bodUD + 0.001
    
    rLeg = rLeg + (0.15 / 2.0)
    lLeg = lLeg - (1.5 / 4.0)
    rCalf = rCalf - (0.3 / 2.0)
    lCalf = lCalf + (1.7 / 4.0)
    rFoot = rFoot + (0.2 / 2.0)
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
    
#leftLeg1Down
for i in range(0,2):
    bodUD = bodUD - 0.001
    
    rLeg = rLeg - (0.15 / 2.0)
    lLeg = lLeg + (1.5 / 4.0)
    rCalf = rCalf + (0.3 / 2.0)
    lCalf = lCalf - (1.7 / 4.0)
    rFoot = rFoot - (0.2 / 2.0)
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
for i in range(0,2):
    bodUD = bodUD + 0.001
    
    rLeg = rLeg + (0.15 / 2.0)
    lLeg = lLeg + (1.5 / 4.0)
    rCalf = rCalf - (0.3 / 2.0)
    lCalf = lCalf - (1.7 / 4.0)
    rFoot = rFoot + (0.2 / 2.0)
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
    
#leftLeg2Up
for i in range(0,2):
    bodUD = bodUD - 0.001
    
    rLeg = rLeg - (0.15 / 2.0)
    lLeg = lLeg - (1.5 / 4.0)
    rCalf = rCalf + (0.3 / 2.0)
    lCalf = lCalf + (1.7 / 4.0)
    rFoot = rFoot - (0.2 / 2.0)
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
for i in range(0,2):
    bodUD = bodUD + 0.001
    
    rLeg = rLeg + (0.15 / 2.0)
    lLeg = lLeg - (1.5 / 4.0)
    rCalf = rCalf - (0.3 / 2.0)
    lCalf = lCalf + (1.7 / 4.0)
    rFoot = rFoot + (0.2 / 2.0)
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
    
#leftLegFlick
for i in range(0,2):
    lCalf = lCalf - (1.7 / 2.0)
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
for i in range(0,2):
    lCalf = lCalf + (1.7 / 2.0)
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
    
#leftLeg2Down
for i in range(0,2):
    bodUD = bodUD - 0.001
    
    rLeg = rLeg - (0.15 / 2.0)
    lLeg = lLeg + (1.5 / 4.0)
    rCalf = rCalf + (0.3 / 2.0)
    lCalf = lCalf - (1.7 / 4.0)
    rFoot = rFoot - (0.2 / 2.0)
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
for i in range(0,2):
    bodUD = bodUD + 0.001
    
    rLeg = rLeg + (0.15 / 2.0)
    lLeg = lLeg + (1.5 / 4.0)
    rCalf = rCalf - (0.3 / 2.0)
    lCalf = lCalf - (1.7 / 4.0)
    rFoot = rFoot + (0.2 / 2.0)
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
    
#rightLeg1Up
for i in range(0,2):
    bodUD = bodUD - 0.001
    
    lLeg = lLeg - (0.15 / 2.0)
    rLeg = rLeg - (1.5 / 4.0)
    lCalf = lCalf + (0.3 / 2.0)
    rCalf = rCalf + (1.7 / 4.0)
    lFoot = lFoot - (0.2 / 2.0)
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
for i in range(0,2):
    bodUD = bodUD + 0.001
    
    lLeg = lLeg + (0.15 / 2.0)
    rLeg = rLeg - (1.5 / 4.0)
    lCalf = lCalf - (0.3 / 2.0)
    rCalf = rCalf + (1.7 / 4.0)
    lFoot = lFoot + (0.2 / 2.0)
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
    
#rightLeg1Down
for i in range(0,2):
    bodUD = bodUD - 0.001
    
    lLeg = lLeg - (0.15 / 2.0)
    rLeg = rLeg + (1.5 / 4.0)
    lCalf = lCalf + (0.3 / 2.0)
    rCalf = rCalf - (1.7 / 4.0)
    lFoot = lFoot - (0.2 / 2.0)
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
for i in range(0,2):
    bodUD = bodUD + 0.001
    
    lLeg = lLeg + (0.15 / 2.0)
    rLeg = rLeg + (1.5 / 4.0)
    lCalf = lCalf - (0.3 / 2.0)
    rCalf = rCalf - (1.7 / 4.0)
    lFoot = lFoot + (0.2 / 2.0)
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)

#rightLeg2Up
for i in range(0,2):
    bodUD = bodUD - 0.001
    
    lLeg = lLeg - (0.15 / 2.0)
    rLeg = rLeg - (1.5 / 4.0)
    lCalf = lCalf + (0.3 / 2.0)
    rCalf = rCalf + (1.7 / 4.0)
    lFoot = lFoot - (0.2 / 2.0)
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
for i in range(0,2):
    bodUD = bodUD + 0.001
    
    lLeg = lLeg + (0.15 / 2.0)
    rLeg = rLeg - (1.5 / 4.0)
    lCalf = lCalf - (0.3 / 2.0)
    rCalf = rCalf + (1.7 / 4.0)
    lFoot = lFoot + (0.2 / 2.0)
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
    
#leftLegFlick
for i in range(0,2):
    rCalf = rCalf - (1.7 / 2.0)
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
for i in range(0,2):
    rCalf = rCalf + (1.7 / 2.0)
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
    
#rightLeg2Down
for i in range(0,2):
    bodUD = bodUD - 0.001
    
    lLeg = lLeg - (0.15 / 2.0)
    rLeg = rLeg + (1.5 / 4.0)
    lCalf = lCalf + (0.3 / 2.0)
    rCalf = rCalf - (1.7 / 4.0)
    lFoot = lFoot - (0.2 / 2.0)
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)  
for i in range(0,2):
    bodUD = bodUD + 0.001
    
    lLeg = lLeg + (0.15 / 2.0)
    rLeg = rLeg + (1.5 / 4.0)
    lCalf = lCalf - (0.3 / 2.0)
    rCalf = rCalf - (1.7 / 4.0)
    lFoot = lFoot + (0.2 / 2.0)
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)
    
#armsDown
for i in range(0,5):
    rArm2 = rArm2 + 0.2
    lArm2 = lArm2 - 0.2
    rFore2 = rFore2 - 0.37
    lFore2 = lFore2 + 0.37
    
    line = [rArm,rArm2,rFore,rFore2,rFore3,lArm,lArm2,lFore,
          lFore2,lFore3,head,head2,rLeg,rLeg2,
          rCalf,rFoot,lLeg,lLeg2,lCalf,lFoot,bodi,bodUD,bodFB]
    data.append(line)

for i in range(0,len(data)):
    for j in range(0,len(data[i])):
        if (abs(data[i][j]) < 0.000001):
            data[i][j] = 0.0
                     
wf = open('walking.txt','w')
writePls(wf, data)
wf.close()
