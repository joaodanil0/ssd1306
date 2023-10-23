#include <linux/module.h>
#include <linux/delay.h>

#include "ssd1306_commands.h"
#include "font.h"

#define SSD1306_MAX_SEG       (128)
#define SSD1306_MAX_LINE      (7)

int SSD1306_DisplayInit(struct i2c_client  *i2c_client);

void SSD1306_String(unsigned char *str, struct i2c_client  *i2c_client);
void SSD1306_PrintChar(unsigned char character, struct i2c_client  *i2c_client);
void SSD1306_GoToNextLine(struct i2c_client  *i2c_client);
void SSD1306_SetCursor(uint8_t lineNo, uint8_t cursorPos, struct i2c_client  *i2c_client);
void SSD1306_Fill(unsigned char data, struct i2c_client *i2c_client);

