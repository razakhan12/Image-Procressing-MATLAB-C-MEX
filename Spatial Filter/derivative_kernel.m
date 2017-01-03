function [ kernel ] = derivative_kernel( select )
%DERIVATIVE_KERNEL Outputs the user's choice of  derivative kernels
%   1 for Central Difference
%   2 for Forward Difference
%   3 for Prewitt
%   4 for Sobel
%   Defaults to Prewitt

    switch select
        case 1
            % Central Difference
            kernel = [1 0 -1];
        case 2
            % Forward Difference
            kernel = [0 1 -1];
        case 3
            % Prewitt
            kernel = [1 0 -1; 1 0 -1; 1 0 -1];
        case 4
            % Sobel
            kernel = [1 0 -1; 2 0 -2; 1 0 -1];
        otherwise
            % default to Prewitt
            kernel = [1 0 -1; 1 0 -1; 1 0 -1];
    end

end

