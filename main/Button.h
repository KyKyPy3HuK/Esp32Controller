#ifndef __BUTTON_H__

#define __BUTTON_H__
#include <stdint.h>
#include "pinConfig.h"


#define BUTTON_UP PIN_BTN_UP
#define BUTTON_RIGHT PIN_BTN_RIGHT
#define BUTTON_DOWN PIN_BTN_DOWN
#define BUTTON_LEFT PIN_BTN_LEFT

#define BUTTON_SELECT PIN_BTN_FNC_RIGHT
#define BUTTON_MENU 
#define BUTTON_RETURN PIN_BTN_FNC_LEFT
#define BUTTON_START 



// Структура для хранения состояния кнопок
typedef struct {
    int pin;
    uint64_t last_press_time;
} Button;



#endif //!__BUTTON_H__