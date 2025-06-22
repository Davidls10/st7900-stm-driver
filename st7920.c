#include "st7920.h"

/* To start a transmission: five 1's, followed by RW and RS bits and finish with 0
* The following 8-bit data is divided in two bytes: HHHHLLLL -> HHHH0000 LLLL0000
*/

void write_code(st7920_spi_dev *dev, unsigned char rs, unsigned char rw, unsigned char word) {
    unsigned char data[3];
    unsigned char mask_rs = rs ? 0b00000010 : 0b00000000;
    unsigned char mask_rw = rw ? 0b00000100 : 0b00000000;

    data[0] = 0b11111000 | mask_rs | mask_rw;
    data[1] = (word & 0b11110000);
    data[2] = (word & 0b00001111) << 4;

    HAL_GPIO_WritePin(dev->spics_port, dev->spics_pin, GPIO_PIN_SET);
    HAL_SPI_Transmit(dev->host_spi, data, 3, 1000);
    HAL_GPIO_WritePin(dev->spics_port, dev->spics_pin, GPIO_PIN_RESET);
    HAL_Delay(50);
}

void display_clear(st7920_spi_dev *dev) {
    write_code(dev, 0, 0, 0b00000001);
}

void return_home(st7920_spi_dev *dev) {
    write_code(dev, 0, 0, 0b00000010);
}

void entry_mode_set(st7920_spi_dev *dev,
				    unsigned char address_counter_control,
				    unsigned char display_shift_control) {
    unsigned char mask_addr_counter_control = address_counter_control ? 0b00000010 : 0b00000000;
    unsigned char mask_display_shift_control = display_shift_control ? 0b00000001 : 0b00000000;
    write_code(dev, 0, 0, 0b00000100 | mask_addr_counter_control | mask_display_shift_control);
}

void display_control(st7920_spi_dev *dev,
					 unsigned char display_on_off_control,
		             unsigned char cursor_on_off_control,
					 unsigned char character_blink_on_off_control) {
    unsigned char mask_display_on_off_control = display_on_off_control ? 0b00000100 : 0b00000000;
    unsigned char mask_cursor_on_off_control = cursor_on_off_control ? 0b00000010 : 0b00000000;
    unsigned char mask_character_blink_on_off_control = character_blink_on_off_control ? 0b00000001 : 0b00000000;
    write_code(dev, 0, 0, 0b00001000 | mask_display_on_off_control | mask_cursor_on_off_control | mask_character_blink_on_off_control);
}

void cursor_display_shift_control(st7920_spi_dev *dev,
								 unsigned char s_c,
								 unsigned char r_l) {
    unsigned char mask_s_c = s_c ? 0b00001000 : 0b00000000;
    unsigned char mask_r_l = r_l ? 0b00000100 : 0b00000000;
    write_code(dev, 0, 0, 0b00010000 | mask_s_c | mask_r_l);
}

void function_set(st7920_spi_dev *dev,
				 unsigned char dl,
				 unsigned char re) {
    unsigned char mask_dl = dl ? 0b00010000 : 0b00000000;
    unsigned char mask_re = re ? 0b00000100 : 0b00000000;
    write_code(dev, 0, 0, 0b00100000 | mask_dl | mask_re);
}

void set_cgram_address(st7920_spi_dev *dev,
					  unsigned char cgram_address) {
    write_code(dev, 0, 0, 0b01000000 | cgram_address);
}

void set_ddram_address(st7920_spi_dev *dev,
					   unsigned char ddram_address) {
    write_code(dev, 0, 0, 0b10000000 | ddram_address);
}

void read_busy_flag_and_address(st7920_spi_dev *dev) {
    write_code(dev, 0, 1, 0b00000000);
}

void write_data_to_ram(st7920_spi_dev *dev,
					   unsigned char *data_to_ram, unsigned int length) {
    unsigned char data[1+length*2];

    data[0] = 0b11111010;

    for (int i = 0; i < length; i++){
    	data[i*2 + 1] = (*data_to_ram & 0b11110000);
	    data[i*2 + 2] = (*data_to_ram & 0b00001111) << 4;
	    data_to_ram++;
    }

    HAL_GPIO_WritePin(dev->spics_port, dev->spics_pin, GPIO_PIN_SET);
    HAL_SPI_Transmit(dev->host_spi, data, 1 + length*2, 1000);
    HAL_GPIO_WritePin(dev->spics_port, dev->spics_pin, GPIO_PIN_RESET);
    HAL_Delay(50);
}

void read_ram_data(st7920_spi_dev *dev) {
    write_code(dev, 1, 1, 0b00000000);
}

void stand_by(st7920_spi_dev *dev) {
    write_code(dev, 0, 0, 0b00000001);
}

void vertical_scroll_or_ram_address_select(st7920_spi_dev *dev,
										  unsigned char sr) {
    unsigned char mask_sr = sr ? 0b00000001 : 0b00000000;
    write_code(dev, 0, 0, 0b00000010 | mask_sr);
}

void reverse(st7920_spi_dev *dev, unsigned char r1, unsigned char r0) {
    unsigned char mask_r1 = r1 ? 0b00000010 : 0b00000000;
    unsigned char mask_r0 = r0 ? 0b00000001 : 0b00000000;
    write_code(dev, 0, 0, 0b00000100 | mask_r1 | mask_r0);
}

void extended_function_set(st7920_spi_dev *dev,
						  unsigned char dl,
						  unsigned char re,
						  unsigned char g) {
    unsigned char mask_dl = dl ? 0b00010000 : 0b00000000;
    unsigned char mask_re = re ? 0b00000100: 0b00000000;
    unsigned char mask_g = g ? 0b00000010 : 0b00000000;
    write_code(dev, 0, 0, 0b00100000 | mask_dl | mask_re | mask_g);
}

void set_scroll_address(st7920_spi_dev *dev,
					   unsigned char scroll_address) {
    write_code(dev, 0, 0, 0b01000000 | scroll_address);
}

void set_graphic_ram_address(st7920_spi_dev *dev,
							unsigned char vertical_address,
							unsigned char horizontal_address) {
    write_code(dev, 0, 0, 0b10000000 | vertical_address);
    write_code(dev, 0, 0, 0b10000000 | horizontal_address);
}

void print_text(st7920_spi_dev *dev, unsigned char *text, unsigned int length) {
	if (*text != '\0' && length < 17) {
		write_data_to_ram(dev, text, length);
	}
}
