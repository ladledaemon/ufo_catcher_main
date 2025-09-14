#include "ACM1602K-NLW-BBW.h"
#include <memory>

class ACM1602K{
public:
    ACM1602K(ACM1602K_Config_t& config): interface_(config.interface), entry_mode_(ENTRY_MODE_RIGHT){

    }
    void InitDisplay(){
        interface_.set_rs(0);
        interface_.set_e(0);

        interface_.delay_ms(40);

        send_4bits(SET_8BIT_MODE);
        interface_.delay_us(5);
        send_4bits(SET_8BIT_MODE);
        interface_.delay_us(100);
        send_4bits(SET_8BIT_MODE);
        interface_.delay_us(100);
        is_4bit_mode = false;

        send_4bits(SET_4BIT_MODE);
        interface_.delay_us(100);
        is_4bit_mode = true;

        send_8bits(SET_2LINE_MODE_4BIT);
        interface_.delay_us(50);

        send_8bits(DISPLAY_OFF);
        interface_.delay_us(50);

        send_8bits(CLEAR_DISPLAY);
        interface_.delay_ms(2);

        send_8bits(entry_mode_);
        interface_.delay_us(50);

        send_8bits(DISPLAY_ON);
        interface_.delay_us(50);

        write_command(0x0F);
    }
    void ChangeEntryMode(uint8_t new_entry_mode){
        interface_.set_rs(0);
        write_command(new_entry_mode);
        interface_.delay_us(50);
        entry_mode_ = new_entry_mode;
    }
    void Print(char data){
        interface_.set_rs(1);
        send_8bits((uint8_t)data);
    }
    void Print(const char* str){
        while (*str) {
            Print(*str++);
        }
    }
    void SetCursor(uint8_t row, uint8_t col){
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
        interface_.set_rs(0);

        // "Set DDRAM Address" コマンド (0x80 | address) を送信
        write_command(0x80 | address);
    }
private:
    acm1602k_interface_t interface_;
    bool is_4bit_mode;
    bool is_2line_mode;
    uint8_t entry_mode_;
    void send_4bits(uint8_t data){
        interface_.write_data_4bits(data);
        interface_.set_e(1);
        interface_.set_e(0);
    }
    void send_8bits(uint8_t data){
        interface_.write_data_4bits(data >> 4);
        interface_.set_e(1);
        interface_.delay_us(1);
        interface_.set_e(0);
        interface_.delay_us(1);
        interface_.write_data_4bits(data & 0x0F);
        interface_.set_e(1);
        interface_.delay_us(1);
        interface_.set_e(0);
        interface_.delay_us(50);
    }
    void write_command(uint8_t command){
        // RSピンをLOWに設定して、コマンドモードにする
        interface_.set_rs(0);
        // 8ビットのコマンドを送信する
        send_8bits(command);
    }
};

struct ACM1602K_Handle{
    std::unique_ptr<ACM1602K> instance;
};

extern "C" {
    ACM1602K_Handle_t* ACM1602K_Create(ACM1602K_Config_t* config){
        ACM1602K_Handle_t* handle = new ACM1602K_Handle_t;
        handle->instance = std::make_unique<ACM1602K>(*config);
        return handle;
    }
    void ACM1602K_InitDisplay(ACM1602K_Handle_t* lcd){
        lcd->instance->InitDisplay();
    }
    void ACM1602K_PrintChar(ACM1602K_Handle_t* lcd, char data){
        lcd->instance->Print(data);
    }
    void ACM1602K_PrintString(ACM1602K_Handle_t* lcd, const char* str){
        lcd->instance->Print(str);
    }
    void ACM1602K_ChangeEntryMode(ACM1602K_Handle_t* lcd, uint8_t new_entry_mode){
        lcd->instance->ChangeEntryMode(new_entry_mode);
    }
    void ACM1602K_SetCursor(ACM1602K_Handle_t* lcd, uint8_t row, uint8_t col){
        lcd->instance->SetCursor(row, col);
    }
    void ACM1602K_Destroy(ACM1602K_Handle_t* lcd){
        if (lcd != nullptr){
            delete lcd;
        }
    }
}