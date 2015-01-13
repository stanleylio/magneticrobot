% process the magnetic field readings from c2x.txt
% Measurements were done in a grid within a square on a horizontal plane.
% c.f. bot2.m and ctrl4.py
%
% The bot traverses the waypoints on the grid in a "zig zag" order (start
% at the beginning of a row, finish the row, then start at the end of the
% next row and finish at the beginning of that row). See bot2.m which
% generated the waypoints/path.
clc
close all
clear all

NAME = 'c3d';

%% Import
% configurations
load([NAME '_configs.mat'])
XCOUNT = numel(x_range);
YCOUNT = numel(y_range);
ZCOUNT = numel(z_range);

% raw readings
[x_m,y_m,z_m] = import_readings([NAME '.txt']);

%% Remove background magnetic field
[x_bg,y_bg,z_bg] = import_readings([NAME '_background.txt']);
x_m = x_m - x_bg;
y_m = y_m - y_bg;
z_m = z_m - z_bg;

%% Change raw readings to unit of Gauss
x_m = x_m/GAIN;
y_m = y_m/GAIN;
z_m = z_m/GAIN;

%% Correct for the pose of the magnetometer at the time of reading
% From magnetometer's frame to the robotic arm's base frame, then to the
% frame of the point where the measurement was taken.
% All that just to reorient the readings to the same orientation as the
% base frame, but at the location of the measurement points.

% Rm4: transforms readings from magnetometer's frame in frame {4}
% homogeneous coordinate
Rm4 = [1 0 0; 0 0 -1; 0 1 0];
Rm4 = [Rm4 [0 0 0]'; 0 0 0 1];

for i = 1:size(waypoints,2)
    jt = solve123(waypoints(:,i));
    % R40: transforms coordinates in {4} to {0}
    R40 = dh40(jt);
    % these three are equivalent:
    %r = R40*Rm4*[x_m(i) y_m(i) z_m(i) 1]' - R40*[0 0 0 1]';
    %r = R40*(Rm4*[x_m(i) y_m(i) z_m(i) 1]' - [0 0 0 1]');
    r = R40*Rm4*[x_m(i) y_m(i) z_m(i) 1]' - [waypoints(:,i) ; 1];
    x_m(i) = r(1);
    y_m(i) = r(2);
    z_m(i) = r(3);
end

%% Optional F: magnitude field strength
if 1 == ZCOUNT
    F = zeros(size(x_m));
    for i = 1:numel(x_m)
        F(i) = norm([x_m(i),y_m(i),z_m(i)]');
    end
    F = reshape(F,XCOUNT,YCOUNT);
    [X,Y] = meshgrid(x_range,y_range);
end

%% Visualize
% normalize for better-looking quivers
x_m_n = zeros(size(x_m));
y_m_n = zeros(size(y_m));
z_m_n = zeros(size(y_m));
for i = 1:size(waypoints,2)
    ndiv = norm([x_m(i) y_m(i) z_m(i)]);
    x_m_n(i) = x_m(i)/ndiv;
    y_m_n(i) = y_m(i)/ndiv;
    z_m_n(i) = z_m(i)/ndiv;
end

figure
if 1 == ZCOUNT
    % 2D, PLANE only
    quiver(waypoints(1,:)',waypoints(2,:)',...
        x_m_n,y_m_n,.8,'linewidth',1.0)
    
    hold on
    
    contour(X,Y,F,'linewidth',2)
    h = colorbar('location','eastoutside');
    xlabel(h,'(Gauss)')

    % location of magnets
    for i = 1:size(MAG_POS,1)
        plot(MAG_POS(i,1),MAG_POS(i,2),'r*')
        plot(MAG_POS(i,1),MAG_POS(i,2),'ro')
    end
else
    % 3D, CUBE
    quiver3(waypoints(1,:)',waypoints(2,:)',waypoints(3,:)',...
        x_m_n,y_m_n,z_m_n,.9,'linewidth',1.0)
    
    hold on
    
    % location of magnets
    for i = 1:size(MAG_POS,1)
        plot3(MAG_POS(i,1),MAG_POS(i,2),MAG_POS(i,3),'r*')
        plot3(MAG_POS(i,1),MAG_POS(i,2),MAG_POS(i,3),'ro')
    end
end

axis equal
grid on
title('Magnetic Field Around Neodymium Magnets (Background Compensated)')
xlabel('X, mm')
ylabel('Y, mm')
zlabel('Z, mm')
legend('Normalized Field Direction',...
    'Magnet','location','southoutside')
saveas(gcf,[NAME '.png'])

