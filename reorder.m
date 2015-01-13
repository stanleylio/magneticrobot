function v = reorder(w,xcount,ycount,zcount)
v = w;

% flip every other row on every horizontal plane (row = along y)
for i = 2:2:xcount*zcount
    v(:,(i-1)*ycount+1:i*ycount)...
        = fliplr(v(:,(i-1)*ycount+1:i*ycount));
end

% flip every other plane
for i = 2:2:zcount
    v(:,(i-1)*xcount*ycount+1:i*xcount*ycount)...
        = fliplr(v(:,(i-1)*xcount*ycount+1:i*xcount*ycount));
end

% could have just reshape-flip-reshape... let's see how this go.
if mod(xcount,2)
    for i = 2:2:zcount
        for j = 1:xcount
            v(:,(i-1)*xcount*ycount+(j-1)*ycount+1:(i-1)*xcount*ycount+j*ycount)...
                = fliplr(v(:,(i-1)*xcount*ycount+(j-1)*ycount+1:(i-1)*xcount*ycount+j*ycount));
        end
    end
end
end