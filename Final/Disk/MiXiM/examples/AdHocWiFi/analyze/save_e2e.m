    function  save_e2e(e2e_sp_fig,e2e_sp_hist_fig,e2e_sp_idx,max_idx,subplot_num);
        figure(e2e_sp_fig);
        orient tall;
        e2e_sp_idx_r = subplot_range(subplot_num,e2e_sp_idx,max_idx);
        xlabel('Packet ID  []');
        save_fig(e2e_sp_fig,sprintf('e2e_delay_multi_%d',floor((e2e_sp_idx-1)/subplot_num)+1),0);        
        figure(e2e_sp_hist_fig);
        orient tall;
        subplot_range(subplot_num,e2e_sp_idx,max_idx);
	    xlabel('End to End delay [sec]');
        save_fig(e2e_sp_hist_fig,sprintf('e2e_delay_hist_multi_%d',floor((e2e_sp_idx-1)/subplot_num)+1),1);
    end
