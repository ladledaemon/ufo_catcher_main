#include "ACM1602K-NLW-BBW.h"

static void lcd_send_4bits(acm1602k_handle_t *lcd, uint8_t data){
    lcd->interface.write_data_4bits(data);
    lcd->interface.set_e(1);
    lcd->interface.set_e(0);
}

static void lcd_send_8bits(acm1602k_handle_t *lcd, uint8_t data){
    lcd->interface.write_data_4bits(data >> 4);
    lcd->interface.set_e(1);
    lcd->interface.delay_us(1);
    lcd->interface.set_e(0);
    lcd->interface.delay_us(1);
    lcd->interface.write_data_4bits(data & 0x0F);
    lcd->interface.set_e(1);
    lcd->interface.delay_us(1);
    lcd->interface.set_e(0);
    lcd->interface.delay_us(50);
}

static void acm1602k_write_command(acm1602k_handle_t *lcd, uint8_t command)
{
    // RSピンをLOWに設定して、コマンドモードにする
    lcd->interface.set_rs(0);
    // 8ビットのコマンドを送信する
    lcd_send_8bits(lcd, command);
}

void acm1602k_init(acm1602k_handle_t *lcd, uint8_t entry_mode){
    lcd->interface.set_rs(0);
    lcd->interface.set_e(0);

    lcd->interface.delay_ms(40);

    lcd_send_4bits(lcd, SET_8BIT_MODE);
    lcd->interface.delay_us(5);
    lcd_send_4bits(lcd, SET_8BIT_MODE);
    lcd->interface.delay_us(100);
    lcd_send_4bits(lcd, SET_8BIT_MODE);
    lcd->interface.delay_us(100);
    lcd->is_4bit_mode = false;

    lcd_send_4bits(lcd, SET_4BIT_MODE);
    lcd->interface.delay_us(100);
    lcd->is_4bit_mode = true;

    lcd_send_8bits(lcd, SET_2LINE_MODE_4BIT);
    lcd->interface.delay_us(50);

    lcd_send_8bits(lcd, DISPLAY_OFF);
    lcd->interface.delay_us(50);

    lcd_send_8bits(lcd, CLEAR_DISPLAY);
    lcd->interface.delay_ms(2);

    lcd_send_8bits(lcd, entry_mode);
    lcd->interface.delay_us(50);
    lcd->entry_mode = entry_mode;

    lcd_send_8bits(lcd, DISPLAY_ON);
    lcd->interface.delay_us(50);

    acm1602k_write_command(lcd, 0x0F);
}

void acm1602k_write_char(acm1602k_handle_t *lcd, char data)
{
    // 1. RSピンをHIGHに設定して、データモードに切り替える
    lcd->interface.set_rs(1);

    // 2. 8ビットの文字コードを送信する
    lcd_send_8bits(lcd, (uint8_t)data);
}

void acm1602k_write_string(acm1602k_handle_t *lcd, const char *str)
{
    while (*str) {
        acm1602k_write_char(lcd, *str++);
    }
}

void acm1602k_set_cursor(acm1602k_handle_t *lcd, uint8_t row, uint8_t col)
{
    uint8_t address;

    // 行に応じてベースアドレスを決定
    switch (row)
    {
        case 0:
            address = 0x00; // 1行目の開始アドレス
            break;
        case 1:
            address = 0x40; // 2行目の開始アドレス
            break;
        default:
            address = 0x00;
            break;
    }

    // 列の位置を足して、最終的なアドレスを計算
    address += col;

    // RSピンをLOWにして、コマンドモードに設定
    lcd->interface.set_rs(0);

    // "Set DDRAM Address" コマンド (0x80 | address) を送信
    lcd_send_8bits(lcd, 0x80 | address);
}

void acm1602k_change_entry_mode(acm1602k_handle_t *lcd, uint8_t new_entry_mode){
    // 1. RSピンをLOWに設定して、コマンドモードに切り替える
    lcd->interface.set_rs(0);

    // 2. 新しいエントリーモードを送信する
    lcd_send_8bits(lcd, new_entry_mode);
    lcd->interface.delay_us(50);

    // 3. エントリーモードを更新
    lcd->entry_mode = new_entry_mode;
}