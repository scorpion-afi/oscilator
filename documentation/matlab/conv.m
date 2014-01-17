clc;
close all;
clear all;
fclose( 'all' );

f_sample = 5000;

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
  
  res = fseek( f_id, 0, 'eof' );    % move file pointer to end of file
  if( res == 0 )
     
    file_size = ftell( f_id );      % calculate file size in bytes
    res = fseek( f_id, 0, 'bof' );  % move file pointer to begin of file
 
    % /2, because of file_size keeps size of file in bytes
    adc_array = zeros( [floor( file_size/2 ), 1] );

    % read data from file to array 'adc_array' (two bytes at time)
    adc_array = fread( f_id, '*uint16' ); % file will be truncated, if it is necessary
    
    % truncation 4 high-order bits (may be not used)
    adc_array( 1:end ) = bitand( adc_array( 1:end ), 4095 );
    
    % create arrays to storing splitted data
    % incomplete measurement set will not be processed,
    % for example: if file_size = 16...23 only 8 half-words(16 bit) of
    % adc_array will be processed
    i_ch1 = zeros( [floor( file_size/8 ), 1] );
    v_ch1 = zeros( [floor( file_size/8 ), 1] );
    v_ch2 = zeros( [floor( file_size/8 ), 1] );
    i_ch2 = zeros( [floor( file_size/8 ), 1] );
         
    % extract data from half-word(16 bit) stream
    % and convert to V and mA
    i_1 = 1;
    i_2 = 2;
    i_3 = 3;
    i_4 = 4;

    for i = 1:size( i_ch1 )       
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
    x = 0:1:size( i_ch1 ) - 1;  % - 1, because of begin index is 0
    time = x * (1/f_sample);
    
    % drawing plots, penultimate parameter is decimation coeficient
    draw_plot( time, i_ch1, i_ch2, v_ch1, v_ch2, 1, 1 );
    draw_plot( time, i_ch1, i_ch2, v_ch1, v_ch2, 2, 2 );
    draw_plot( time, i_ch1, i_ch2, v_ch1, v_ch2, 4, 3 );
    draw_plot( time, i_ch1, i_ch2, v_ch1, v_ch2, 700, 4 );
    
  end % if( res == 0 )
  
  fclose( f_id );
end % if( f_id > 3 ) 

% clear all;
