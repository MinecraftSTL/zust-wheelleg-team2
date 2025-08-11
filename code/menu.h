/*
 * menu.h
 *
 *  Created on: 2024Äê11ÔÂ14ÈÕ
 *      Author: minec
 */

#ifndef CODE_MENU_H_
#define CODE_MENU_H_
#include "Cpu/Std/Ifx_Types.h"
extern const int PAGE_PAGEKEY_MAX;

enum PageValueType{
    NULL_TYPE,
    PAGE_TYPE,
    INT_TYPE,
    FLOAT_TYPE,
    BOOL_TYPE,
    FUNC_TYPE
};

struct PageKey{
    char name[16];
    enum PageValueType type;
    void *value;
    int opened;
    int open;
};
struct PagePageValue{
    struct PageKey value[10];
    uint8 size;
};
struct IntPageValue{
    int *value;
    int max;
    int min;
    int step;
};
struct FloatPageValue{
    float *value;
    float max;
    float min;
    float step;
};
struct BooleanPageValue{
    int *value;
};
struct FunctionPageValue{
    void (*up)();
    void (*down)();
};

struct PageKey new_PageKey(char name[], enum PageValueType type, void *value);
void del_PageKey(struct PageKey *pageKey);
void PageKey_print(struct PageKey *this);
void PageKey_press(struct PageKey *this, uint8 pressed);
struct PagePageValue *new_PagePageValue(struct PageKey key[], uint8 size);
struct IntPageValue *new_IntPageValue(int *value, int max, int min, int step);
struct FloatPageValue *new_FloatPageValue(float *value, float max, float min, float step);
struct BooleanPageValue *new_BooleanPageValue(int *value);
struct FunctionPageValue *new_FunctionPageValue(void (*up)(), void (*down)());

#endif /* CODE_MENU_H_ */
