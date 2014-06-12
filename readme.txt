
								Проект рабочий.(01.09.2013) v0.2

Используется ОС FreeRTOS.
Схема выделения памяти - heap1.

Размер кучи - 17 Кб (освобождение, динамически выделенной, памяти данной моделью (heap1) не предусмотрено).


Опрос клавиатур, вывод на экран, генерация звукового сигнала по нажатию на кнопки, меню, генерация сигналов и измерение напряжений.
	
 Структура папок будет и дальше поддерживаться такой.


 Проект собран для STM32F103RCT6 с 256 Кб FLASH и 48 Кб SRAM.

 Startup выбран для STM32F10X_HD (High-density performance line). - startup_stm32f10x_hd.s


 Сразу после Reset контроллер тактируется от внутреннего RC генератора (8 МГЦ). Затем вызывается функция SystemInit() (определение в файле startup_stm32f10x_hd.s)

 и происходит попытка "затактировать" контроллер внешним источником (HSE - High Speed External), для STM32F103RCT6 это может быть кварц с 8 Мгц max. Если это удается,

 то SYSCLK = 72 МГц, PCLK1 = 36 МГц, PCLK2 = 72 МГц.


Создано:
        7 потоков:  PB Thread, Main Thread, LCD Thread, Beep Thred, Osc Thread, Meas Thread и Calc Thread.
        6 очередей: между PB Thread и Main Thread, между Main Thread и LCD Thread, к Beep Thred к Osc Thread,
		    к Meas Thread и к Calc Thread.
        1 семафор:  для регулирования доступа к аппаратуре TIM2. 

Типы сообщений описаны в файле InterDefines.h.

Adds:

 - Опрос матричной клавиатуры отключен (ножки НЕ настраиваются, можно использовать в других целях), меню переведено на режим работы только с KeyPad.  
 - Введен режим измерения напряжений на 4-х входах АЦП и режим отображения их на LCD.
 - Переделано меню.
 - Введен сигнал exp вместо inv pulse.
 
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
git 3.