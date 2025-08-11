/*
 * menu.c
 *
 *  Created on: 2024年11月14日
 *      Author: minec
 */
#include "sys.h"

#include "menu.h"

#include "zf_device_ips200.h"

const int PAGE_PAGEKEY_MAX = 10;

void del_PagePageValue(struct PagePageValue *this);
void PagePageValue_print(struct PagePageValue *this, int opened, int open);
struct pressReturn PagePageValue_press(struct PagePageValue *this, uint8 pressed, int opened, int open);
void IntPageValue_print(struct IntPageValue *this, int opened, int open);
struct pressReturn IntPageValue_press(struct IntPageValue *this, uint8 pressed, int opened, int open);
void FloatPageValue_print(struct FloatPageValue *this, int opened, int open);
struct pressReturn FloatPageValue_press(struct FloatPageValue *this, uint8 pressed, int opened, int open);
void BooleanPageValue_print(struct BooleanPageValue *this, int opened, int open);
struct pressReturn BooleanPageValue_press(struct BooleanPageValue *this, uint8 pressed, int opened, int open);
void FunctionPageValue_print(struct FunctionPageValue *this, int opened, int open);
struct pressReturn FunctionPageValue_press(struct FunctionPageValue *this, uint8 pressed, int opened, int open);

struct PageKey new_PageKey(char name[], enum PageValueType type, void *value){
    struct PageKey ret;
    // 使用strncpy复制字符串到ret.name，避免越界，同时确保字符串结尾有'\0'
    strncpy(ret.name, name, sizeof(ret.name) - 1);
    ret.name[sizeof(ret.name) - 1] = '\0';
    ret.type = type;
    ret.value = value;
    ret.opened = 0;
    ret.open = 0;
    return ret;
}
void del_PageKey(struct PageKey *pageKey){
    if(pageKey->type == PAGE_TYPE){
        del_PagePageValue(pageKey->value);
    }
}
void ips200_tempClear(){
//    ips200_clear();
    for(int i=0; i<20; ++i){
        ips200_show_string(0, 16*i, "                              ");
    }
}
void PageKey_print(struct PageKey *this){
    switch(this->type){
        case PAGE_TYPE:
            ips200_tempClear();
            ips200_show_string(0, 0, this->name);
            PagePageValue_print(this->value, this->opened, this->open);
            break;
        case INT_TYPE:
            IntPageValue_print(this->value, this->opened, this->open);
            break;
        case FLOAT_TYPE:
            FloatPageValue_print(this->value, this->opened, this->open);
            break;
        case BOOL_TYPE:
            BooleanPageValue_print(this->value, this->opened, this->open);
            break;
        case FUNC_TYPE:
            FunctionPageValue_print(this->value, this->opened, this->open);
            break;
    }
}
struct pressReturn{
    int opened;
    int open;
};
void PageKey_press(struct PageKey *this, uint8 pressed){
    struct pressReturn ret;
    switch(this->type){
        case PAGE_TYPE:
            ret = PagePageValue_press(this->value, pressed, this->opened, this->open);
            break;
        case INT_TYPE:
            ret = IntPageValue_press(this->value, pressed, this->opened, this->open);
            break;
        case FLOAT_TYPE:
            ret = FloatPageValue_press(this->value, pressed, this->opened, this->open);
            break;
        case BOOL_TYPE:
            ret = BooleanPageValue_press(this->value, pressed, this->opened, this->open);
            break;
        case FUNC_TYPE:
            ret = FunctionPageValue_press(this->value, pressed, this->opened, this->open);
            break;
    }
    this->opened = ret.opened;
    this->open = ret.open;
}

struct PagePageValue *new_PagePageValue(struct PageKey key[], uint8 size) {
    struct PagePageValue *ret = (struct PagePageValue *)malloc(sizeof(struct PagePageValue));
    if (ret == NULL) {
        return NULL;
    }
    int i;
    for (i = 0; i < size; i++) {
        ret->value[i] = key[i];
    }
    // 如果传入的元素个数小于PAGE_PAGEKEY_MAX，将剩余元素初始化为一些默认值（比如全0等情况，具体按需求定）
    for (; i < PAGE_PAGEKEY_MAX; i++) {
        struct PageKey default_key = { "", NULL_TYPE, {((void *)0)}, 0, 0 };
        ret->value[i] = default_key;
    }
    ret->size = size;
    return ret;
}
void PagePageValue_init(struct PagePageValue *this, struct PageKey key[], uint8 size){
    memcpy(&this->value, key, sizeof(struct PageKey)*size);
    this->size = size;
}
void del_PagePageValue(struct PagePageValue *this){
    for(int i=0; i<this->size; ++i){
        struct PageKey value = this->value[i];
        if(value.type == PAGE_TYPE){
            del_PageKey(&value);
        }else{
            free(&value);
        }
    }
}
void PagePageValue_print(struct PagePageValue *this, int opened, int open){
    if(open && this->value[opened].type == PAGE_TYPE){
        PageKey_print(&(this->value[opened]));
        return;
    }else{
        ips200_show_string(230, 20*(opened+1), open ? "<" : "-");
    }
    for(int i=0; i<this->size; ++i){
        ips200_show_string(10, 20*(i+1), this->value[i].name);
        if(this->value[i].type != PAGE_TYPE){
            this->value[i].opened = i;
            PageKey_print(&(this->value[i]));
        }
    }
}
struct pressReturn PagePageValue_press(struct PagePageValue *this, uint8 pressed, int opened, int open){
    struct pressReturn ret = {0, 0};
    if(pressed&0x01){
        struct PageKey *pageKey = &(this->value[opened]);
        if(pageKey->type != PAGE_TYPE || !pageKey->open){
            ret.opened = opened;
            ret.open = 0;
            return ret;
        }
    }
    if(open){
        PageKey_press(&(this->value[opened]), pressed);
        ret.opened = opened;
        ret.open = 1;
        return ret;
    }else{
        if(pressed&0x02){
            opened-=1;
            if(opened < 0){
                opened = this->size-1;
            }
            ret.opened = opened;
            ret.open = 0;
            return ret;
        }
        if(pressed&0x04){
            opened+=1;
            if(opened >= this->size){
                opened = 0;
            }
            ret.opened = opened;
            ret.open = 0;
            return ret;
        }
        if(pressed&0x08){
            ret.opened = opened;
            ret.open = 1;
            return ret;
        }
    }
    return ret;
}

struct IntPageValue *new_IntPageValue(int *value, int max, int min, int step) {
    struct IntPageValue *ret = (struct IntPageValue *)malloc(sizeof(struct IntPageValue));
    if (ret!= NULL) {
        ret->value = value;
        ret->max = max;
        ret->min = min;
        ret->step = step;
    }
    return ret;
}
void IntPageValue_init(struct IntPageValue *this, int *value, int max, int min, int step){
    this->value = value;
    this->max = max;
    this->min = min;
    this->step = step;
}
void IntPageValue_print(struct IntPageValue *this, int opened, int open){
    ips200_show_int(160, 20*(opened+1), *(this->value), 8);
}
struct pressReturn IntPageValue_press(struct IntPageValue *this, uint8 pressed, int opened, int open){
    struct pressReturn ret = {0, 0};
    if(pressed&0x02){
        *(this->value) += this->step;
        if(*(this->value) > this->max){
            *(this->value) = this->max;
        }
        ret.opened = opened;
        ret.open = 0;
        return ret;
    }
    if(pressed&0x04){
        *(this->value) -= this->step;
        if(*(this->value) < this->min){
            *(this->value) = this->min;
        }
        ret.opened = opened;
        ret.open = 0;
        return ret;
    }
    if(pressed&0x08){
        ret.opened = opened;
        ret.open = 0;
        return ret;
    }
    return ret;
}

struct FloatPageValue *new_FloatPageValue(float *value, float min, float max, float step) {
    struct FloatPageValue *ret = (struct FloatPageValue *)malloc(sizeof(struct FloatPageValue));
    if (ret!= NULL) {
        ret->value = value;
        ret->min = min;
        ret->max = max;
        ret->step = step;
    }
    return ret;
}
void FloatPageValue_init(struct FloatPageValue *this, float *value, float min, float max, float step){
    this->value = value;
    this->min = min;
    this->max = max;
    this->step = step;
}
void FloatPageValue_print(struct FloatPageValue *this, int opened, int open){
    ips200_show_float(150, 20*(opened+1), *(this->value)+1e-6, 4,2);
}
struct pressReturn FloatPageValue_press(struct FloatPageValue *this, uint8 pressed, int opened, int open){
    struct pressReturn ret = {0, 0};
    if(pressed&0x02){
        *(this->value) += this->step;
        if(*(this->value) > this->max){
            *(this->value) = this->max;
        }
        ret.opened = opened;
        ret.open = 0;
        return ret;
    }
    if(pressed&0x04){
        *(this->value) -= this->step;
        if(*(this->value) < this->min){
            *(this->value) = this->min;
        }
        ret.opened = opened;
        ret.open = 0;
        return ret;
    }
    if(pressed&0x08){
        ret.opened = opened;
        ret.open = 0;
        return ret;
    }
    return ret;
}

struct BooleanPageValue *new_BooleanPageValue(int *value) {
    struct BooleanPageValue *ret = (struct BooleanPageValue *)malloc(sizeof(struct BooleanPageValue));
    if (ret!= NULL) {
        ret->value = value;
    }
    return ret;
}
void BooleanPageValue_init(struct BooleanPageValue *this, int *value){
    this->value = value;
}
void BooleanPageValue_print(struct BooleanPageValue *this, int opened, int open){
    ips200_show_string(150, 20*(opened+1), *(this->value)?"true":"false");
}
struct pressReturn BooleanPageValue_press(struct BooleanPageValue *this, uint8 pressed, int opened, int open){
    struct pressReturn ret = {0, 0};
    if(pressed&0x02){
        *(this->value) = 1;
        ret.opened = opened;
        ret.open = 0;
        return ret;
    }
    if(pressed&0x04){
        *(this->value) = 0;
        ret.opened = opened;
        ret.open = 0;
        return ret;
    }
    if(pressed&0x08){
        ret.opened = opened;
        ret.open = 0;
        return ret;
    }
    return ret;
}

struct FunctionPageValue *new_FunctionPageValue(void (*up)(), void (*down)()) {
    struct FunctionPageValue *ret = (struct FunctionPageValue *)malloc(sizeof(struct FunctionPageValue));
    if (ret!= NULL) {
        ret->up = up;
        ret->down = down;
    }
    return ret;
}
void FunctionPageValue_init(struct FunctionPageValue *this, void (*up)(), void (*down)()){
    this->up = up;
    this->down = down;
}
void FunctionPageValue_print(struct FunctionPageValue *this, int opened, int open){}
struct pressReturn FunctionPageValue_press(struct FunctionPageValue *this, uint8 pressed, int opened, int open){
    struct pressReturn ret = {0, 0};
    if(pressed&0x02){
        this->up();
        ret.opened = opened;
        ret.open = 0;
        return ret;
    }
    if(pressed&0x04){
        this->down();
        ret.opened = opened;
        ret.open = 0;
        return ret;
    }
    if(pressed&0x08){
        ret.opened = opened;
        ret.open = 0;
        return ret;
    }
    return ret;
}

