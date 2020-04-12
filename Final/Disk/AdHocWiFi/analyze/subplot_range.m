    function new_index = subplot_range(num,index,max_index);
        seg = num;
        ofs = mod(index-1,num);
        if (index+num>max_index+2)
            seg=max_index-floor((index-0.0001)/num)*num;
        end
        subplot(seg,1,ofs+1);
        hold on;
        new_index = index+1;
    end
    