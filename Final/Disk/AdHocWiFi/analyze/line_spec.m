function [ spec ] = line_spec( i ,marker)
%return line specification for a specific number
%   used for looping and plotting with different style

ic = mod(i, 7);
switch (ic)
    case 0 
        col = 'r';
    case 1 
        col = 'g';
    case 2  
        col = 'b';
    case 3 
        col = 'c';
    case 4  
        col = 'm';
    case 5  
        col = 'y';
    case 6  
        col = 'k';
    otherwise 
        col = 'b';
end

t = floor(i/7); % after color
line = '-';
switch (mod(t,4)) 
    case 0  
        line = '-';
    case 1  
        line = '--';
    case 2  
        line = ':';
    case 3  
        line = '-.';
    otherwise
        line = '-';
end
    
m='+';
switch (floor(t/4))
    case 0  
        m='+';
    case 1  
        m='O';
    case 2  
        m='*';
    case 3 
        m='.';
    case 4  
        m='s';
    case 5  
        m='d';
    case 6  
        m='^';
    case 7  
        m='V';
    case 8 
        m='<';
    case 9  
        m='>';
    case 10  
        m='p';
    case 11 
        m='h';
    otherwise 
        m='h';       
end

if (marker==1)
  spec=sprintf('%s%s%s',col,line,m);
else
  spec=sprintf('%s%s',col,line);
end
end
