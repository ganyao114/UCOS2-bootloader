void lcd_init(int lcd_type);
void lcd_draw_bmp(const unsigned char gImage_bmp[], int lcd_type);
void lcd_draw_pixel(int row, int col, int color);
void lcd_clear_screen(int color);
void lcd_draw_hline(int row, int col1, int col2, int color);
void lcd_draw_vline(int col, int row1, int row2, int color);
void lcd_draw_cross(int row, int col, int halflen, int color);
void lcd_draw_circle(void);
void lcd_draw_char(unsigned char c);
void Lcd_PutASCII(unsigned int x,unsigned int y,unsigned char ch,unsigned int c,unsigned int bk_c,unsigned int st);
void Lcd_PutHZ(unsigned int x,unsigned int y,unsigned short int QW,unsigned int c,unsigned int bk_c,unsigned int st);
void Lcd_Printf(unsigned int x,unsigned int y,unsigned int c,unsigned int bk_c,unsigned int st,char *fmt,...);
