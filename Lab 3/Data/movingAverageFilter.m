function [ filteredData ] = movingAverageFilter( data, N )

filteredData = data;
%x = zeros(length(data));
for i = 1:(length(data) - (N))
    data(i:(i+N));
    filteredData(i) = sum(data(i:(i+N-1)))/N;
    x(i) = filteredData(i);
    
end

end

