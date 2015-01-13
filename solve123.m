function theta = solve123(oc)
% Given the centre of the wrist in {0}, find theta1, theta2 and theta3 in
% radian.
% Return [NaN NaN NaN] if the position is not reachable.
% Physical constraints of the arm are used to reject redundant solutions.
% 0 <= theta2 <= pi/2
% -2*pi/3 <= theta3 <= 0      % about -120 degree

% There should be one theta1, two of theta2 and two of theta3.
% Only one of each will be returned.
if numel(oc) ~= 3
    fprintf('precond violation in solve123\n')
    theta1 = NaN;
    theta2 = NaN;
    theta3 = NaN;
else
    d1 = 68.5;      % mm
    a2 = 103;       % mm
    a3 = 165;       % mm
    xc = oc(1);
    yc = oc(2);
    zc = oc(3);
    
    % ----------------
    % solve for theta1
    % ----------------
    theta1 = atan2(yc,xc);
    
    % ----------------
    % solve for all possible theta3
    % ----------------
    r = sqrt(xc^2 + yc^2);
    h = zc - d1;
    c3 = ((r^2 + h^2) - (a2^2 + a3^2)) / (2*a2*a3);
    if abs(c3) <= 1
        s3 = sqrt(1 - c3^2);
        theta3 = [atan2(s3,c3) atan2(-s3,c3)]';

        % ----------------
        % solve for all possible theta2
        % ----------------
        PQR1 = atan2(a3*sin(theta3(1)), (a2 + a3*cos(theta3(1))));
        PQR2 = atan2(a3*sin(theta3(2)), (a2 + a3*cos(theta3(2))));
        PQH = atan2(h, r);
        theta2 = [PQH - PQR1; PQH - PQR2];

        % so far, unique theta1, two theta2, two theta3.
        % Now pick the right ones:
        if theta3(1) <= 0
            theta2 = theta2(1);
            theta3 = theta3(1);
        elseif theta3(2) <= 0
            theta2 = theta2(2);
            theta3 = theta3(2);
        else
            fprintf('?!');
            theta1 = NaN;
            theta2 = NaN;
            theta3 = NaN;
        end
    else
        theta1 = NaN;
        theta2 = NaN;
        theta3 = NaN;
    end
end
theta = [theta1 theta2 theta3]';
end