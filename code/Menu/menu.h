/*
 * Menu.h
 *
 *  Created on: 2024Äê11ÔÂ14ÈÕ
 *      Author: minec
 */

#ifndef CODE_MENU_H_
#define CODE_MENU_H_
#include "Cpu/Std/Ifx_Types.h"

#define IPS200_DEFAULT_HIGHLIGHTCOLOR         (0x7BFF  )

#define LIST_PAGE_ELEMENT_MAX 11

enum KeyType{
    NULL_KEY,
    UP_KEY,
    DOWN_KEY,
    LEFT_KEY,
    RIGHT_KEY,
    CENTER_KEY,
    PERV_KEY,
    NEXT_KEY,
    BACK_KEY,
    HOME_KEY,
    KEY_NUM,
};
enum PageExtendsType{
    LIST_TYPE,
    INT_TYPE,
    FLOAT_TYPE,
    DOUBLE_TYPE,
    BOOL_TYPE,
    FUNC_TYPE,
};
typedef struct Page{
    char name[16];
    struct Page *parent;
    int8 select;
    enum PageExtendsType type;
    union {
        struct {
            uint8 size;
            struct Page *value[LIST_PAGE_ELEMENT_MAX];
            uint8 open;
        } listValue;
        struct {
            int32 *value;
            int32 min;
            int32 max;
            uint8 open;
        } intValue;
        struct {
            float *value;
            float min;
            float max;
            uint8 dot;
            uint8 open;
        } floatValue;
        struct {
            double *value;
            double min;
            double max;
            uint8 dot;
            uint8 open;
        } doubleValue;
        struct {
            uint8 *value;
            uint8 dir;
        } boolValue;
        struct {
            void (*value)();
        } funcValue;
    } extends;
} Page;

void PageKey_init(Page this, char name[], enum PageExtendsType type, void *value);
void PageKey_print(Page *this, uint8 row);
void PageKey_press(Page *this, uint8 pressed[]);
Page *PageKey_getRoot(Page *this);
Page *PageKey_getOpened(Page *this);
Page *PageKey_getByPath(Page *this, char *path);
uint8 PageKey_back(Page *this);
void PageKey_home(Page *this);
void ListPage_init(Page *this, char name[], uint8 size, Page *key[]);
void ListPage_setRoot(Page *this);
void IntPage_init(Page *this, char name[], int32 *value, int32 max, int32 min);
void FloatPage_init(Page *this, char name[], float *value, float max, float min);
void DoublePage_init(Page *this, char name[], double *value, double max, double min);
void BoolPage_init(Page *this, char name[], uint8 *value, uint8 dir);
void FuncPage_init(Page *this, char name[], void (*value)());

#endif /* CODE_MENU_H_ */
