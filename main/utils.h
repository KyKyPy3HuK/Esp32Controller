#ifndef __UTILS_H__
#define __UTILS_H__

#define DELAY(x) vTaskDelay(x / portTICK_PERIOD_MS)   

typedef struct Point2 {
    int x;
    int y;
} Point2;

#endif