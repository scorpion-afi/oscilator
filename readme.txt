
								 .(01.09.2013) v0.2

  FreeRTOS.
   - heap1.

  - 17  (,  ,    (heap1)  ).


 ,   ,       , ,     .
	
       .


    STM32F103RCT6  256  FLASH  48  SRAM.

 Startup   STM32F10X_HD (High-density performance line). - startup_stm32f10x_hd.s


   Reset     RC  (8 ).    SystemInit() (   startup_stm32f10x_hd.s)

    ""    (HSE - High Speed External),  STM32F103RCT6      8  max.   ,

  SYSCLK = 72 , PCLK1 = 36 , PCLK2 = 72 .


:
        7 :  PB Thread, Main Thread, LCD Thread, Beep Thred, Osc Thread, Meas Thread  Calc Thread.
        6 :  PB Thread  Main Thread,  Main Thread  LCD Thread,  Beep Thred  Osc Thread,
		     Meas Thread   Calc Thread.
        1 :       TIM2. 

     InterDefines.h.

Adds:

 -     (  ,     ),        KeyPad.  
 -      4-        LCD.
 -  .
 -   exp  inv pulse.
 
Hardware usage:
 
 - TIM2,3,4,5
 - DMA1 Channel 1 and DMA2 Channel 3
 - DAC Channels 1,2
 - ADC1 Channels 10,11, 12,13
 - Some GPIO`s pins.


Memory usage:

  32 868 bytes of readonly  code memory   rest = 229 276 bytes
     432 bytes of readonly  data memory
  35 825 bytes of readwrite data memory   rest =  13 327 bytes

git now is used at home.
git 1.
git 2.