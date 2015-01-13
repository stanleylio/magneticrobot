function T = dh40(jt)
% jt in radian
% Forward kinematics
%     a    alpha d    theta
L = [0 0 68.5 jt(1);
    0 pi/2 0 0;
    103 0 0 jt(2);
    165 0 0 jt(3)];

T = dh(L(1,:))*dh(L(2,:))*dh(L(3,:))*dh(L(4,:));
end