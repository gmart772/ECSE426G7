function [ w ] = convertData( data )
%CONVERTDATA Summary of this function goes here
%   Detailed explanation goes here

x = zeros(length(data) / 3, 1);
y = zeros(length(data) / 3, 1);
z = zeros(length(data) / 3, 1);



for i = 1:(length(data) / 3)
   x(i) = data(3*i - 2);
   y(i) = data(3*i - 1);
   z(i) = data(3*i);

end

    averageX = mean(x)
    averageY = mean(y)
    averageZ = mean(z)
    

end

