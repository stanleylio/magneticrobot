function T = dh(v)
% the angles v are in radian
a = v(1);
alpha = v(2);
d = v(3);
theta = v(4);

Rztheta = rz(theta);
Dzd = translate([0,0,d]);
Dxa = translate([a,0,0]);
Rxalpha = rx(alpha);

T = Rztheta*Dzd*Dxa*Rxalpha;
end