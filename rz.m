function Rzalpha = rz(alpha)
% alpha in radian
Rzalpha = [cos(alpha),  -sin(alpha), 0, 0;
           sin(alpha),  cos(alpha),  0, 0;
           0,           0,           1, 0;
           0,           0,           0, 1];
end