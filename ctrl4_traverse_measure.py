'''
Measure magnetic field over a 2D grid by traversing the set of waypoints
from "waypoints.txt"

Using Pololu servo controller
3DOF eBay arm with Hitec servos

Stanley Lio, March 2014
'''
import serial
import time
from arm1 import setpos_cmd

s_arm = serial.Serial('COM28',115200,timeout=5)
s_imu = serial.Serial('COM5',115200,timeout=5)
print 'Robotic arm at', s_arm.portstr
print 'Magnetometer at', s_imu.portstr

with open('waypoints.txt','r') as f:
    lines = f.readlines()

W = []
for line in lines:
    w = line.strip().split(',') # strip and split
    w = [float(i) for i in w]   # convert list of strings to list of floats
    W.append(w)

#print W

idx = 1
try:
    with open('c3.txt','w') as f:
        for w in W:
            # move arm to waypoint
            print '{0} of {1}:'.format(idx,len(W)),
            print w
            s_arm.write(setpos_cmd(0,w[0]))
            s_arm.write(setpos_cmd(1,w[1]))
            s_arm.write(setpos_cmd(2,w[2]))
            time.sleep(w[3])
            
            # read magnetometer
            s_imu.write('\n')
            line = s_imu.readline()
            print '{0}:{1}'.format(i,line),
            f.write(line)

            idx = idx + 1
except KeyboardInterrupt:
    print 'user interrupted'

time.sleep(1)
s_arm.write(setpos_cmd(0,90))
s_arm.write(setpos_cmd(1,120))
s_arm.write(setpos_cmd(2,-140))

s_arm.close()
s_imu.close()

print '... done.'
