%% Generate a path within a 3D cube with multiple horizontal 2D planes
% This generates the path (set of waypoints) for the bot to follow. Output
% is in "waypoints.txt". Configurations are in "NAME_configs.mat"
clc
close all
clear all

%% configurations
NAME = 'c3d';
PITCH = 10;             % mm
MAG_POS = [50,150,4];
GAIN = 230;             % gain: 230 LSb/Gauss
PAUSE = 0.2;            % time per waypoint

%% Calculate waypoint coordinates
x_range = 0:PITCH:100;
y_range = 100:PITCH:200;
z_range = 50:PITCH:150;
[X,Y,Z] = meshgrid(x_range,y_range,z_range);

waypoints = zeros(3,numel(X));          % in cartesian coordinate
waypoint_angles = zeros(3,numel(X));    % in terms of joint angles
for i = 1:numel(X)
    waypoints(:,i) = [X(i) Y(i) Z(i)]';
end

%% reordering to minimize arm travel
waypoints = reorder(waypoints,numel(x_range),numel(y_range),numel(z_range));

% compute DH angles from waypoint coordinates
for i = 1:numel(X)
    waypoint_angles(:,i) = rad2deg(solve123(waypoints(:,i)));
end

%%
figure
plot3(waypoints(1,:),waypoints(2,:),waypoints(3,:),'b.-')
hold on
plot3(waypoints(1,1),waypoints(2,1),waypoints(3,1),'ro')
axis equal
axis(axis + [-10 10 -10 10 -10 10])
grid on
xlabel('X_o, mm')
ylabel('Y_0, mm')
zlabel('Z_0, mm')
title('Waypoints')
legend('waypoints','starting point','location','southoutside')
view([30,30])
saveas(gcf,[NAME '_waypoints.png'])

%% write waypoints to text file
f = fopen('waypoints.txt','wt');
for i = 1:size(waypoint_angles,2)
    fprintf(f,'%.4f,%.4f,%.4f,%.3f\n',waypoint_angles(:,i),PAUSE);
end
fclose(f);

save([NAME '_configs.mat'],...
    'waypoints',...
    'x_range','y_range','z_range',...
    'PITCH','PAUSE','MAG_POS','GAIN')

fprintf('NAME=')
disp(NAME)
fprintf('%d by %d by %d, PITCH=%dmm, PAUSE=%.3fsec\n',...
    numel(x_range),numel(y_range),numel(z_range),PITCH,PAUSE);
disp('... done.')

