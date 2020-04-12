function  out_str = str_conv( str,direction )
% convert string with spaces and special characters to one without or the
% other direction
%   Since can't hold special characters in field names need to convert

original=[' ';':';'(';')';'[';']';'#';'-';'>';'''';'=';';'];
changed=['SPC';'COL';'CLB';'CRB';'SLB';'SRB';'HSH';'DSH';'GRE';'APS';'EQU';'SMC'];
if (direction==1)
    temp = changed;
    changed = original;
    original = temp;
end
for (i= 1:length(original)) 
 str = strrep(str,original(i,:),changed(i,:));
end
out_str = str;
end

