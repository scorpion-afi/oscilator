clc;
close all;
clear all;
fclose( 'all' );

% this is frequency of ADC in Hz
f_sample = 5000;

% gui open file dialog
[filename, pathname] = uigetfile( '*.txt', 'Open MeasGeneratorData-file' );
FILE_NAME = strcat( pathname, filename );

% open file, with path 'FILE_NAME' to read in binary mode
f_id = fopen( FILE_NAME, 'r' );

%==========================================================================
%==========================================================================
%==========================================================================

% structure of data in file (binary):
% 0xaa 0xaa 0xbb 0xbb 0xcc 0xcc 0xdd 0xdd 0xaa 0xaa ...
% little-endian (low byte first)
% aa - IoutCh1 (only 12 least bits are significant)
% bb - VoutCh1 (only 12 least bits are significant)
% cc - VoutCh2 (only 12 least bits are significant)
% dd - IoutCh2 (only 12 least bits are significant)

% measurement set is set of measured data for 4 channel (8 bytes)

if( f_id > 2 ) 
    
    % read data from file to array 'adc_array' (two bytes at time) (little-endian)
    adc_array = fread( f_id, '*uint16' ); % file will be truncated, if it is necessary
    
    % adc_array - is column-vector of items (16 bit)
    
    % item_num - is amount of full (16 bit) samples
    [item_num, ~] = size( adc_array );
  
    % truncation 4 high-order bits (may be not used)
    adc_array( 1:end ) = bitand( adc_array( 1:end ), 4095 );
    
    % create arrays to storing splitted data
    % incomplete measurement set will not be processed,
    % for example: if item_num = 8...11 only 8 items (16 bit) of adc_array
    % will be processed (3-rd measurement set will be drop out) 
    i_ch1 = zeros( [floor( item_num/4 ), 1] );
    v_ch1 = zeros( [floor( item_num/4 ), 1] );
    v_ch2 = zeros( [floor( item_num/4 ), 1] );
    i_ch2 = zeros( [floor( item_num/4 ), 1] );
    
    % i_ch1, v_ch1, v_ch2, i_ch2 - are column-vectors of items (16 bit)
    
    % split_size - is size of arrays, which will store splitted data
    [split_size, ~] = size( i_ch1 );
    
    % extract data from half-word(16 bit) stream
    % and convert to V and mA
    i_1 = 1;
    i_2 = 2;
    i_3 = 3;
    i_4 = 4;
      
    for i = 1:split_size       
      i_ch1( i ) = 0.1943    * double( adc_array( i_1 ) ) - 0.3238;
      v_ch1( i ) = 0.0011848 * double( adc_array( i_2 ) ) - 0.0119;
      v_ch2( i ) = 0.0009407 * double( adc_array( i_3 ) ) - 0.005;
      i_ch2( i ) = 0.1943    * double( adc_array( i_4 ) ) - 0.4829;

      i_1 = i_1 + 4;
      i_2 = i_2 + 4;
      i_3 = i_3 + 4;
      i_4 = i_4 + 4;      
    end

    % creation time array, for abscissa of plots
    x = 0:1:split_size - 1;  % - 1, because of begin index is 0
    time = x * ( 1/f_sample );          
    
    decimation_coef_array = [ 0 1 3 5 511 ];
    
    % drawing plots, penultimate parameter is decimation coeficient   
    parfor i = 1:5
        draw_plot( time, i_ch1, i_ch2, v_ch1, v_ch2, decimation_coef_array( i ), i ); 
    end
          
    fclose( f_id );
end % if( f_id > 3 ) 

% clear all;
