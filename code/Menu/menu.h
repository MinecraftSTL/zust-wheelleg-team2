/*
 * menu.h
 *
 *  Created on: 2024Äê11ÔÂ14ÈÕ
 *      Author: minec
 */

#ifndef CODE_MENU_H_
#define CODE_MENU_H_
#include "Cpu/Std/Ifx_Types.h"

#define IPS200_DEFAULT_HIGHLIGHTCOLOR         (0x7BFF  )

extern const int Max_page_pageKey;

enum PageExtendsType{
    LIST_TYPE,
    INT_TYPE,
    FLOAT_TYPE,
    DOUBLE_TYPE,
    BOOL_TYPE,
};
typedef struct Page{
    char name[16];
    struct Page *parent;
    int8 open;
    enum PageExtendsType type;
    union {
        struct {
            uint8 size;
            struct Page *value[16];
            uint8 opened;
        } listValue;
        struct {
            int32 *value;
            int32 min;
            int32 max;
            uint8 opened;
        } intValue;
        struct {
            float *value;
            float min;
            float max;
            uint8 opened;
        } floatValue;
        struct {
            double *value;
            double min;
            double max;
            uint8 opened;
        } doubleValue;
        struct {
            uint8 *value;
            uint8 dir;
        } boolValue;
    } extends;
} Page;

void PageKey_init(Page this, char name[], enum PageExtendsType type, void *value);
void PageKey_print(Page *this);
void PageKey_press(Page *this, uint8 pressed);
Page *PageKey_getOpened(Page *this);
uint8 PageKey_back(Page *this);
void PageKey_home(Page *this);
void ListPage_init(Page *this, char name[], uint8 size, struct Page *key[]);
void IntPage_init(Page *this, char name[], int32 *value, int32 max, int32 min);
void FloatPage_init(Page *this, char name[], float *value, float max, float min);
void DoublePage_init(Page *this, char name[], double *value, double max, double min);
void BoolPage_init(Page *this, char name[], uint8 *value, uint8 dir);

#endif /* CODE_MENU_H_ */
