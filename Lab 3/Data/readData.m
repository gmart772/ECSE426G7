w1 = zeros(length(zbdown) / 3, 4);
y1 = zeros(length(zbdown) / 3, 3);

for i = 1:(length(zbdown)/3)
    w1(i, 1) = zbdown(3*i - 2);
    w1(i, 2) = zbdown(3*i - 1);
    w1(i, 3) = zbdown(3*i);
    w1(i, 4) = 1000;
    
    y1(i, 3) = 1000;
    
end

w2 = zeros(length(zbup) / 3, 4);
y2 = zeros(length(zbup) / 3, 3);

for i = 1:(length(zbup)/3)
    w2(i, 1) = zbup(3*i - 2);
    w2(i, 2) = zbup(3*i - 1);
    w2(i, 3) = zbup(3*i);
    w2(i, 4) = 1000;
    
    y2(i, 3) = -1000;
    
end

w3 = zeros(length(ybdown) / 3, 4);
y3 = zeros(length(ybdown) / 3, 3);

for i = 1:(length(ybdown)/3)
    w3(i, 1) = ybdown(3*i - 2);
    w3(i, 2) = ybdown(3*i - 1);
    w3(i, 3) = ybdown(3*i);
    w3(i, 4) = 1000;
    
    y3(i, 2) = 1000;
    
end

w4 = zeros(length(ybup) / 3, 4);
y4 = zeros(length(ybup) / 3, 3);

for i = 1:(length(ybup)/3)
    w4(i, 1) = ybup(3*i - 2);
    w4(i, 2) = ybup(3*i - 1);
    w4(i, 3) = ybup(3*i);
    w4(i, 4) = 1000;
    
    y4(i, 2) = -1000;
    
end

w5 = zeros(length(xbdown) / 3, 4);
y5 = zeros(length(xbdown) / 3, 3);

for i = 1:(length(xbdown)/3)
    w5(i, 1) = xbdown(3*i - 2);
    w5(i, 2) = xbdown(3*i - 1);
    w5(i, 3) = xbdown(3*i);
    w5(i, 4) = 1000;
    
    y5(i, 1) = 1000;
    
end

w6 = zeros(length(xbup) / 3, 4);
y6 = zeros(length(xbup) / 3, 3);

for i = 1:(length(xbup)/3)
    w6(i, 1) = xbup(3*i - 2);
    w6(i, 2) = xbup(3*i - 1);
    w6(i, 3) = xbup(3*i);
    w6(i, 4) = 1000;
    
    y6(i, 1) = -1000;
    
end

Y = [y1; y2; y3; y4; y5; y6];

w = [w1; w2; w3; w4; w5; w6];2

X = inv((transpose(w) * w)) * transpose(w) * Y


