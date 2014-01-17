
% this function draws 6 plots from input parameters
% time - abscissa
% i_ch1, i_ch2, u_ch1, u_ch2 - ordinates
% decimation_coef - decimation coefficient
% num - number of figure
function draw_plot( time, i_ch1, i_ch2, u_ch1, u_ch2, decimation_coef, num )
    
    if decimation_coef < 0 || num < 0
      return;
    end
    
    [row, ~] = size( i_ch1 );

    if row == 0
      return;
    end
    
	if decimation_coef == 0
		array_size = row;
	else
		array_size = floor( ( row - 1 ) / ( 1 + decimation_coef ) ) + 1;
	end

    % memory allocation for storing decimated data
    i_ch1_local  = zeros( [array_size, 1] );
    i_ch2_local  = zeros( [array_size, 1] );
    u_ch1_local  = zeros( [array_size, 1] );
    u_ch2_local  = zeros( [array_size, 1] );
    pw_ch1_local = zeros( [array_size, 1] );
    pw_ch2_local = zeros( [array_size, 1] );
    time_local   = zeros( [array_size, 1] );
      
    index = 0;
    
    % filling decimated arrays
    for i = 1:array_size
      index = ( i - 1 ) * ( decimation_coef + 1 ) + 1;
      
      if i == array_size         % only for debuging
          array_size = 0;
      end
      i_ch1_local( i ) = i_ch1( index );
      i_ch2_local( i ) = i_ch2( index );
      u_ch1_local( i ) = u_ch1( index );
      u_ch2_local( i ) = u_ch2( index );
      time_local ( i ) = time( index );
      
      pw_ch1_local( i ) = i_ch1_local( i ) * u_ch1_local( i );
      pw_ch2_local( i ) = i_ch2_local( i ) * u_ch2_local( i );     
    end

    % drawing plots
    figure( num ); 
    
    subplot( 3, 2, 1 );
    plot( time_local, i_ch1_local );
    title( 'Current plot' );
    xlabel( 'time, ms' );
    ylabel( 'IoutCh1, mA' )
        
    subplot( 3, 2, 2 );
    plot( time_local, i_ch2_local ); 
    title( 'Current plot' );
    xlabel( 'time, ms' );
    ylabel( 'IoutCh2, mA' );
      
    subplot( 3, 2, 3 );
    plot( time_local, u_ch1_local );
    title( 'Voltage plot' );
    xlabel( 'time, ms' );
    ylabel( 'VoutCh1, V' );
      
    subplot( 3, 2, 4 );
    plot( time_local, u_ch2_local ); 
    title( 'Voltage plot' );
    xlabel( 'time, ms' );
    ylabel( 'VoutCh2, V' );
    
    subplot( 3, 2, 5 );
    plot( time_local, pw_ch1_local );
    title( 'Power plot' );
    xlabel( 'time, ms' );
    ylabel( 'power ch1, mW' ); 
    
    subplot( 3, 2, 6 );
    plot( time_local, pw_ch2_local ); 
    title( 'Power plot' );
    xlabel( 'time, ms' );
    ylabel( 'power ch2, mW' );
    
