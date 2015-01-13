'''
support functions for the eBay 3DOF arm
'''

# Compose a Pololu SetTarget() command given servo ID and PWM duration
# Pololu controller accepts angles expressed in unit of quarter microsecond
def pololu(servo,ms):
    y = 4*ms
    return bytearray([132,servo] + [y & 0x7F , (y >> 7) & 0x7F])

# magic values obtained from experiment.
# c.f. DH kinematics of the arm (on your scratch paper)
def j1_theta2ms(theta):
    return int(round((1535 - 1980)/45.0*(theta - 45.0) + 1980))

def j2_theta2ms(theta):
    return int(round((1500 - 1005)/45.0*(theta - 45.0) + 1005))

def j3_theta2ms(theta):
    return int(round((1941 - 1477)/45.0*(theta - -90) + 1477))

# Compose a Pololu SetTarget() command given servo ID and DH angle
def setpos_cmd(servo,theta):
    if 0 == servo:
        return pololu(servo,j1_theta2ms(theta))
    if 1 == servo:
        return pololu(servo,j2_theta2ms(theta))
    if 2 == servo:
        return pololu(servo,j3_theta2ms(theta))
