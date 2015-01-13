function Tv = translate(v)
% v is a vector with three elements (can be either row or column vector)
    [m,n] = size(v);
    if (1 == m && 3 == n)
        v = v';
    elseif (1 == n && 3 == m)
    else
        disp('precond violation in translate(): v is not a 3D vector');
    end
    
    Tv = eye(4);
    Tv(:,4) = [v; 1];
end