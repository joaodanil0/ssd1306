//
//  Based in: https://embetronicx.com/tutorials/linux/device-drivers/ssd1306-i2c-linux-device-driver-using-raspberry-pi/
//  https://embetronicx.com/
//  SSD1306 I2C Linux Device Driver using Raspberry PI – Linux Device Driver Tutorial Part 41
//

#include "ssd1306_util.h"

uint8_t SSD1306_LineNum = 0;
uint8_t SSD1306_CursorPos = 0;

int SSD1306_DisplayInit(struct i2c_client *i2c_client)
{
	msleep(100); // delay

	pr_info("Display Init\n");
	// SSD1306_command_turnon(SSD1306_DISPLAY_OFF, i2c_client);
	
	SSD1306_command_mem_addr_mode(0x00, i2c_client);
    SSD1306_command_horizontal_flip(0xA1, i2c_client);
    SSD1306_command_vertical_flip(0xC8, i2c_client);
    SSD1306_command_charge_pump(0x14, i2c_client);
    SSD1306_command_turnon(SSD1306_DISPLAY_ON, i2c_client);
	// SSD1306_command_contrast(0x01, i2c_client);
    // SSD1306_command_entire_on(0xA4, i2c_client);
	// SSD1306_command_inverse_disp(0xA6, i2c_client);
	
	// SSD1306_command_lower_col_start(0x00, i2c_client);
	// SSD1306_command_higher_col_start(0x00, i2c_client);
	// SSD1306_command_col_addr(0x00, 0x7F, i2c_client);
	// SSD1306_command_page_addr(0x00, 0x07, i2c_client);	

    // SSD1306_command_start_line(0x40, i2c_client);

    // SSD1306_command_mux_ratio(0x3F, i2c_client);

	// SSD1306_command_display_offset(0x00, i2c_client);
   	// SSD1306_command_com_pins(0x02, i2c_client);

	// SSD1306_command_set_clock(0x10, i2c_client);
	// SSD1306_command_pre_charge(0x22, i2c_client);
	// SSD1306_command_VCOMH(0x20, i2c_client);a



	pr_info("Display Init done\n");

	return 0;
}

void SSD1306_String(unsigned char *str, struct i2c_client *i2c_client)
{
	while (*str) {
		SSD1306_PrintChar(*str++, i2c_client);
	}
}

void SSD1306_PrintChar(unsigned char character, struct i2c_client *i2c_client)
{
	uint8_t data_byte;
	uint8_t temp = 0;

	/*
  ** If we character is greater than segment len or we got new line charcter
  ** then move the cursor to the new line
  */
	if (((SSD1306_CursorPos + SSD1306_COL_LENGTH) >= SSD1306_MAX_SEG) ||
	    (character == '\n'))
		SSD1306_GoToNextLine(i2c_client);

	// print charcters other than new line
	if (character != '\n') {
		/*
    ** In our font array (SSD1306_font), space starts in 0th index.
    ** But in ASCII table, Space starts from 32 (0x20).
    ** So we need to match the ASCII table with our font table.
    ** We can subtract 32 (0x20) in order to match with our font table.
    */
		character -= 0x20; //or c -= ' ';

		do {
			data_byte = SSD1306_font[character][temp]; // Get the data to be displayed from LookUptable

			SSD1306_write(false, data_byte, i2c_client); // write data to the OLED
			SSD1306_CursorPos++;

			temp++;

		} while (temp < SSD1306_COL_LENGTH);

		SSD1306_write(false, 0x00, i2c_client); //Display the data
		SSD1306_CursorPos++;
	}
}

void SSD1306_GoToNextLine(struct i2c_client *i2c_client)
{
	/*
  ** Increment the current line number.
  ** roll it back to first line, if it exceeds the limit. 
  */
	SSD1306_LineNum++;
	SSD1306_LineNum = (SSD1306_LineNum & SSD1306_MAX_LINE);

	SSD1306_SetCursor(SSD1306_LineNum, 0,
			  i2c_client); /* Finally move it to next line */
}

void SSD1306_SetCursor(uint8_t lineNo, uint8_t cursorPos,
		       struct i2c_client *i2c_client)
{
	/* Move the Cursor to specified position only if it is in range */
	if ((lineNo <= SSD1306_MAX_LINE) && (cursorPos < SSD1306_MAX_SEG)) {
		SSD1306_LineNum = lineNo; // Save the specified line number
		SSD1306_CursorPos =
			cursorPos; // Save the specified cursor position

		SSD1306_write(true, 0x21, i2c_client); // cmd for the column start and end address
		SSD1306_write(true, cursorPos, i2c_client); // column start addr
		SSD1306_write(true, SSD1306_MAX_SEG - 1, i2c_client); // column end addr
		SSD1306_write(true, 0x22, i2c_client); // cmd for the page start and end address
		SSD1306_write(true, lineNo, i2c_client); // page start addr
		SSD1306_write(true, SSD1306_MAX_LINE, i2c_client); // page end addr
	}
}

void SSD1306_Fill(uint8_t data, struct i2c_client *i2c_client)
{
	unsigned int total = 128 * 8; // 8 pages x 128 segments x 8 bits of data
	unsigned int i = 0;

	//Fill the Display
	for (i = 0; i < total; i++) {
		SSD1306_write(false, data, i2c_client);
	}
}

MODULE_DESCRIPTION("FB driver for the Solomon SSD1306 OLED controller");
MODULE_AUTHOR("João Danilo <joaodanilo1992@gmail.com>");
MODULE_VERSION("1.0");
MODULE_LICENSE("GPL");