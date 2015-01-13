% mapping DH angles of the joints to PWM duration
% just verifying the formulae. sandbox code.
clc
close all
clear all

theta = 60
y = round((1438 - 1912)/45.0*theta + -45*(1438 - 1912)/45.0 + 1912)

theta = 45
y = round((1534 - 1985)/45.0*theta + -45*(1534 - 1985)/45.0 + 1985)

theta = -45
y = round((2093 - 1160)/90.0*theta + 2093)

