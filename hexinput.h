#ifndef HEXINPUT
#define HEXINPUT

#include <QString>

class HexInput
{
public:
    void enter_string(QString str);
    void set_key_delay(int delay);

private:
    void do_hex_input(uint32_t value);
    void num_key_press(uint8_t digit);
    void send_keybd_event(uint8_t key_code, bool is_down);
    int key_delay = 5;
};

#endif // HEXINPUT

