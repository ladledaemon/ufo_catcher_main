#ifndef ACM1602K_NLW_BBW_H
#define ACM1602K_NLW_BBW_H

#include <stdbool.h>
#include <stdint.h>

#define SET_8BIT_MODE 0x03
#define SET_4BIT_MODE 0x02
#define SET_2LINE_MODE_4BIT 0x28
#define DISPLAY_ON 0x0C
#define DISPLAY_OFF 0x08
#define CLEAR_DISPLAY 0x01

// 標準設定: カーソルは右へ移動、画面はシフトしない
#define ENTRY_MODE_RIGHT      0x06

// カーソルは左へ移動、画面はシフトしない
#define ENTRY_MODE_LEFT       0x04

// 画面全体が左へシフト（文字は右から現れるように見える）
#define ENTRY_MODE_SHIFT_LEFT 0x07

// 画面全体が右へシフト（文字は左から現れるように見える）
#define ENTRY_MODE_SHIFT_RIGHT 0x05

typedef void (*acm1602k_gpio_write_func_t)(uint8_t value);
typedef void (*acm1602k_gpio_write_4bits_func_t)(uint8_t value);
typedef void (*acm1602k_delay_ms_func_t)(uint32_t ms);
typedef void (*acm1602k_delay_us_func_t)(uint32_t us);

typedef struct{
    acm1602k_gpio_write_func_t set_rs;
    acm1602k_gpio_write_func_t set_e;
    acm1602k_gpio_write_4bits_func_t write_data_4bits;
    acm1602k_delay_ms_func_t delay_ms;
    acm1602k_delay_us_func_t delay_us;
}acm1602k_interface_t;

typedef struct{
    acm1602k_interface_t interface;
    bool is_4bit_mode;
    bool is_2line_mode;
    uint8_t entry_mode;
}acm1602k_handle_t;

void acm1602k_init(acm1602k_handle_t *lcd, uint8_t entry_mode);
void acm1602k_write_char(acm1602k_handle_t *lcd, char data);
void acm1602k_write_string(acm1602k_handle_t *lcd, const char *str);
void acm1602k_set_cursor(acm1602k_handle_t *lcd, uint8_t row, uint8_t col);
void acm1602k_change_entry_mode(acm1602k_handle_t *lcd, uint8_t new_entry_mode);

#endif // ACM1602K_NLW_BBW_H