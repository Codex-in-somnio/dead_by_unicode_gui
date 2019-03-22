#include <stdint.h>
#include <windows.h>
#include <QThread>
#include "hexinput.h"

uint8_t vkey_to_scan_code(uint8_t key_code)
{
    return MapVirtualKeyA(key_code, 0);
}

void HexInput::send_keybd_event(uint8_t key_code, bool is_down)
{
    uint8_t scan_code = vkey_to_scan_code(key_code);
    keybd_event(key_code, scan_code, is_down ? 0 : 2, 0);
    QThread::msleep(key_delay);
}

void HexInput::num_key_press(uint8_t digit) // 输入0x0到0xf的整型，'+'是加号
{
    uint8_t key_code = 0;
    if (digit == '+')
        key_code = 0x6B;
    else if (digit < 10)
        key_code = digit | 0x60; // 小键盘数字范围 0x60到0x69
    else if (digit >= 10 && digit < 16)
        key_code = (digit - 9) | 0x40; // A到F键：0x41到0x46
    send_keybd_event(key_code, true);
    send_keybd_event(key_code, false);
}

void HexInput::do_hex_input(uint32_t value)
{
    // l_alt: 0xa4
    send_keybd_event(0xa4, true);
    num_key_press('+');
    bool found_non_zero = false;
    for (int i = 0; i < 4; ++i)     // alt+unicode 最高支持16位
    {
        uint8_t digit = (value >> 4 * (4 - 1 - i)) & 0xf;
        if (!found_non_zero)
        {
            if (digit == 0)
                continue;
            else
                found_non_zero = true;
        }
        num_key_press(digit);
    }
    send_keybd_event(0xa4, false);
}

void HexInput::enter_string(QString str)
{
    for (int i = 0; i < str.length(); ++i)
    {
        QChar c = str.at(i);
        uint16_t c_value = c.unicode();
        if (c_value == '\n')
        {
            send_keybd_event(0x0d, true);
            send_keybd_event(0x0d, false);
        }
        else if (c_value == '\r')
        {
            continue;
        }
        else
        {
            do_hex_input(c_value);
        }
    }
}

void HexInput::set_key_delay(int delay)
{
    key_delay = delay;
}
