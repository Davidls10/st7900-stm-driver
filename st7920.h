/**
  **********************************************************************************************************************
  * @file    st7920.h
  * @author  David Lima
  * @brief   ST7920 Driver.
  * @note    This is not a full version driver; its intention is just to validate the basic functions of the hardware
  *
  **********************************************************************************************************************
  */

#ifndef __ST7920_H
#define __ST7920_H

#include "stm32f3xx_hal.h"

typedef struct{
    SPI_HandleTypeDef *host_spi;
    GPIO_TypeDef *spics_port;
    uint16_t spics_pin;
} st7920_spi_dev;

void write_code(st7920_spi_dev *dev, unsigned char rs, unsigned char rw, unsigned char word); // ok
void display_clear(st7920_spi_dev *dev); // ok
void return_home(st7920_spi_dev *dev); // ok
void entry_mode_set(st7920_spi_dev *dev,
	    unsigned char address_counter_control,
	    unsigned char display_shift_control); // ok
void display_control(st7920_spi_dev *dev,
		 unsigned char display_on_off_control,
        unsigned char cursor_on_off_control,
		 unsigned char character_blink_on_off_control); // ok
void cursor_display_shift_control(st7920_spi_dev *dev,
		 	 	 	 	 	 	  unsigned char s_c,
								  unsigned char r_l); //
void function_set(st7920_spi_dev *dev, unsigned char dl, unsigned char re); // ok
//void set_cgram_address();
void set_ddram_address(st7920_spi_dev *dev, unsigned char ddram_address); // ok 0x80 - 0x8F  0x90 - 0x9F
//void read_busy_flag_and_address();
void write_data_to_ram(st7920_spi_dev *dev,
					   unsigned char *data_to_ram, unsigned int length); // ok
//void read_ram_data();
//void stand_by();
void vertical_scroll_or_ram_address_select(st7920_spi_dev *dev,
		  unsigned char sr); // ok
void reverse(st7920_spi_dev *dev, unsigned char r1, unsigned char r0); // ok
//void extended_function_set();
void set_scroll_address(st7920_spi_dev *dev,
		   unsigned char scroll_address); // ok
//void set_graphic_ram_address();
void print_text(st7920_spi_dev *dev, unsigned char *text, unsigned int length);

#endif
