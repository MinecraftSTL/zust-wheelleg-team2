/*
 * Menu.c
 *
 *  Created on: 2024年11月14日
 *      Author: minec
 */
#include "Menu.h"
#include "Sys.h"

#include "zf_device_ips200.h"

void ListPage_print(Page *this, uint8 row);
void ListPage_press(Page *this, uint8 pressed[]);
void IntPage_print(Page *this, uint8 row);
void IntPage_press(Page *this, uint8 pressed[]);
void FloatPage_print(Page *this, uint8 row);
void FloatPage_press(Page *this, uint8 pressed[]);
void DoublePage_print(Page *this, uint8 row);
void DoublePage_press(Page *this, uint8 pressed[]);
void BoolPage_print(Page *this, uint8 row);
void BoolPage_press(Page *this, uint8 pressed[]);
void EnumPage_print(Page *this, uint8 row);
void EnumPage_press(Page *this, uint8 pressed[]);
void FuncPage_print(Page *this, uint8 row);
void FuncPage_press(Page *this, uint8 pressed[]);

void ips200_reset_color();
void ips200_set_pencolor(const uint16 color);
int Int_pow(int this, int pow);
void Int_toString(int this, char *str, uint8 num);
void Double_toString(float this, char *str, uint8 num, uint8 point);

void Page_init(Page *this, char name[], enum PageExtendsType type){
    strncpy(this->name, name, sizeof(this->name));
    this->type = type;
    this->select = 0;
}

void PageKey_print(Page *this, uint8 row){
    if(!row&&(this->type!=LIST_TYPE || !this->extends.listValue.open)){
        ips200_show_string_color((240-8*strlen(this->name))/2, 0, this->name, this->select<0 ? IPS200_DEFAULT_SELECTCOLOR : IPS200_DEFAULT_PENCOLOR);
    }
    switch(this->type){
        case LIST_TYPE:
            ListPage_print(this, row);
            break;
        case INT_TYPE:
            IntPage_print(this, row);
            break;
        case FLOAT_TYPE:
            FloatPage_print(this, row);
            break;
        case DOUBLE_TYPE:
            DoublePage_print(this, row);
            break;
        case BOOL_TYPE:
            BoolPage_print(this, row);
            break;
        case ENUM_TYPE:
            EnumPage_print(this, row);
            break;
        case FUNC_TYPE:
            FuncPage_print(this, row);
            break;
    }
}
uint8 PageKey_press(Page *this, uint8 pressed[]){
    switch(this->type){
        case LIST_TYPE:
            ListPage_press(this, pressed);
            break;
        case INT_TYPE:
            IntPage_press(this, pressed);
            break;
        case FLOAT_TYPE:
            FloatPage_press(this, pressed);
            break;
        case DOUBLE_TYPE:
            DoublePage_press(this, pressed);
            break;
        case BOOL_TYPE:
            BoolPage_press(this, pressed);
            break;
        case ENUM_TYPE:
            EnumPage_press(this, pressed);
            break;
        case FUNC_TYPE:
            FuncPage_press(this, pressed);
            break;
    }
    if(pressed[HOME_KEY]){
        PageKey_home(this);
    }else if(pressed[BACK_KEY]){
        PageKey_back(this);
    }
    uint8 ret = 0;
    for(int i=0; i<KEY_NUM; ++i){
        if(pressed[i]){
            beepShort();
            --pressed[i];
            ret = 1;
        }
    }
    return ret;
}
Page *PageKey_getRoot(Page *this){
    if(this->parent != NULL){
        return PageKey_getOpened(this->parent);
    }else{
        return this;
    }
}
Page *PageKey_getOpened(Page *this){
    if(this->type == LIST_TYPE && this->extends.listValue.open){
        return PageKey_getOpened(this->extends.listValue.value[this->select]);
    }else if(this->parent != NULL && !this->parent->extends.listValue.open){
        return PageKey_getOpened(this->parent);
    }else{
        return this;
    }
}
Page *PageKey_getByPath(Page *this, char *path){
    char *dot = strchr(path, '.');
    if(dot == NULL){
        dot = path + strlen(path);
    }
    if(dot == path){
        return NULL;
    }
    for(uint8 i=0; i<this->extends.listValue.size; ++i){
        Page *next = this->extends.listValue.value[i];
        if(!strncmp(path, next->name, dot-path)){
            if(*dot){
                return PageKey_getByPath(next, dot+1);
            }else{
                return next;
            }
        }
    }
    return NULL;
}
uint8 PageKey_back(Page *this){
    Page *open = PageKey_getOpened(this);
    if(open->parent == NULL){
        return 0;
    }else{
        open->parent->extends.listValue.open = 0;
    }
    ips200_clear();
    return 1;
}
void PageKey_home(Page *this){
    Page *open = PageKey_getOpened(this);
    while(open->parent != NULL){
        open->parent->extends.listValue.open = 0;
        open = open->parent;
    }
    ips200_clear();
}

void ListPage_init(Page *this, char name[], Page *key[]){
    Page_init(this, name, LIST_TYPE);
    for(this->extends.listValue.size=0; this->extends.listValue.size<PAGE_ELEMENT_MAX; ++this->extends.listValue.size){
        if(!key[this->extends.listValue.size]){
            break;
        }
    }
    if(this->extends.listValue.size == 0){
        this->select = -1;
    }
    memcpy(this->extends.listValue.value, key, sizeof(Page*)*this->extends.listValue.size);
    for(int i=0; i<this->extends.listValue.size; ++i){
        key[i]->parent = this;
    }
    this->extends.listValue.open = 0;
}
void ListPage_setRoot(Page *this){
    this->parent=NULL;
}
void ListPage_print(Page *this, uint8 row){
    if(!row){
        if(this->extends.listValue.open){
            PageKey_print(this->extends.listValue.value[this->select], row);
            return;
        }
        for(int i=0; i<this->extends.listValue.size; ++i){
            ips200_show_string_color(0, (i+1)*16, this->extends.listValue.value[i]->name, this->select==i ? IPS200_DEFAULT_SELECTCOLOR : IPS200_DEFAULT_PENCOLOR);
            PageKey_print(this->extends.listValue.value[i], i+1);
        }
    }
}
void ListPage_press(Page *this, uint8 pressed[]){
    if(this->extends.listValue.open){
        PageKey_press(this->extends.listValue.value[this->select], pressed);
        return;
    }
    if(pressed[UP_KEY] || pressed[PERV_KEY]){
        --this->select;
        if(this->select < -1){
            this->select = this->extends.listValue.size-1;
        }
    }
    if(pressed[DOWN_KEY] || pressed[NEXT_KEY]){
        ++this->select;
        if(this->select > this->extends.listValue.size-1){
            this->select = -1;
        }
    }
    if(pressed[LEFT_KEY]){
        PageKey_back(this);
        return;
    }
    if(pressed[RIGHT_KEY] || pressed[CENTER_KEY]){
        if(this->select < 0){
            PageKey_back(this);
            return;
        }else{
            this->extends.listValue.open = 1;
            ips200_clear();
        }
    }
}

void IntPage_init(Page *this, char name[], int32 *value, int32 min, int32 max){
    Page_init(this, name, INT_TYPE);
    this->extends.intValue.value = value;
    this->extends.intValue.min = min;
    this->extends.intValue.max = max;
    this->extends.intValue.open = 0;
}
void IntPage_print(Page *this, uint8 row){
    char str[7] = {0};
    Int_toString(*this->extends.intValue.value, str, 9);
    for(int i=0; i<10; ++i){
        if(!str[i]){
            break;
        }
        ips200_show_char_color((row?160:0)+i*8, row?row*16:16, str[i], !row&&this->select==i?this->extends.intValue.open?IPS200_DEFAULT_OPENCOLOR:IPS200_DEFAULT_SELECTCOLOR:IPS200_DEFAULT_PENCOLOR);
    }
}
void IntPage_press(Page *this, uint8 pressed[]){
    if(pressed[LEFT_KEY] || !this->extends.intValue.open&&pressed[PERV_KEY]){
        --this->select;
        if(this->select < -1){
            this->select = 9;
        }
    }
    if(pressed[RIGHT_KEY] || !this->extends.intValue.open&&pressed[NEXT_KEY]){
        ++this->select;
        if(this->select > 9){
            this->select = -1;
        }
    }
    if(pressed[UP_KEY] || pressed[DOWN_KEY] || pressed[LEFT_KEY] || pressed[RIGHT_KEY]){
        this->extends.intValue.open = 0;
    }
    if(pressed[UP_KEY] || pressed[DOWN_KEY] || pressed[PERV_KEY] || pressed[NEXT_KEY]){
        if(this->select < 0){
            if(pressed[LEFT_KEY] || this->extends.intValue.open){
                PageKey_back(this);
                return;
            }
        }else if(this->select == 0){
            if(pressed[UP_KEY] || pressed[DOWN_KEY] || this->extends.intValue.open&&(pressed[PERV_KEY]||pressed[NEXT_KEY])){
                *this->extends.intValue.value = -*this->extends.intValue.value;
            }
        }else{
            if(pressed[DOWN_KEY] || this->extends.intValue.open&&pressed[PERV_KEY]){
                *this->extends.intValue.value -= Int_pow(10, 9-this->select);
            }
            if(pressed[UP_KEY] || this->extends.intValue.open&&pressed[NEXT_KEY]){
                *this->extends.intValue.value += Int_pow(10, 9-this->select);
            }
        }
        if(*this->extends.intValue.value < this->extends.intValue.min){
            *this->extends.intValue.value = this->extends.intValue.min;
        }else if(*this->extends.intValue.value > this->extends.intValue.max){
            *this->extends.intValue.value = this->extends.intValue.max;
        }
    }
    if(pressed[CENTER_KEY]){
        if(this->select < 0){
            PageKey_back(this);
            return;
        }else{
            this->extends.intValue.open = !this->extends.intValue.open;
        }
    }
    if(pressed[UP_KEY]){
        if(this->select < 0){
            PageKey_back(this);
            return;
        }
    }
}

void FloatPage_init(Page *this, char name[], float *value, float min, float max){
    Page_init(this, name, FLOAT_TYPE);
    this->extends.floatValue.value = value;
    this->extends.floatValue.min = min;
    this->extends.floatValue.max = max;;
    this->extends.floatValue.dot = 4;
    this->extends.floatValue.open = 0;
}
void FloatPage_print(Page *this, uint8 row){
    char str[11] = {0};
    Double_toString(*this->extends.floatValue.value, str, this->extends.floatValue.dot, 6-this->extends.floatValue.dot);
    for(int i=0; i<8; ++i){
        if(!str[i]){
            break;
        }
        ips200_show_char_color((row?160:0)+i*8, row?row*16:16, str[i], !row&&this->select==i?this->extends.floatValue.open?IPS200_DEFAULT_OPENCOLOR:IPS200_DEFAULT_SELECTCOLOR:IPS200_DEFAULT_PENCOLOR);
    }
}
void FloatPage_press(Page *this, uint8 pressed[]){
    if(pressed[LEFT_KEY] || !this->extends.floatValue.open&&pressed[PERV_KEY]){
        --this->select;
        if(this->select < -1){
            this->select = 7;
        }
    }
    if(pressed[RIGHT_KEY] || !this->extends.floatValue.open&&pressed[NEXT_KEY]){
        ++this->select;
        if(this->select > 7){
            this->select = -1;
        }
    }
    if(pressed[UP_KEY] || pressed[DOWN_KEY] || pressed[LEFT_KEY] || pressed[RIGHT_KEY]){
        this->extends.floatValue.open = 0;
    }
    if(pressed[UP_KEY] || pressed[DOWN_KEY] || pressed[PERV_KEY] || pressed[NEXT_KEY]){
        if(this->select < 0){
            if(pressed[UP_KEY]){
                PageKey_back(this);
                return;
            }
        }else if(this->select == 0){
            if(pressed[UP_KEY] || pressed[DOWN_KEY] || this->extends.floatValue.open&&(pressed[PERV_KEY]||pressed[NEXT_KEY])){
                *this->extends.floatValue.value = -*this->extends.floatValue.value;
            }
        }else if(this->select < this->extends.floatValue.dot+1){
            if(pressed[DOWN_KEY] || this->extends.floatValue.open&&pressed[PERV_KEY]){
                *this->extends.floatValue.value -= powf(10, this->extends.floatValue.dot-this->select);
            }
            if(pressed[UP_KEY] || this->extends.floatValue.open&&pressed[NEXT_KEY]){
                *this->extends.floatValue.value += powf(10, this->extends.floatValue.dot-this->select);
            }
        }else if(this->select == this->extends.floatValue.dot+1){
            if(pressed[DOWN_KEY] || this->extends.floatValue.open&&pressed[PERV_KEY]){
                if(this->extends.floatValue.dot>0){
                    --this->extends.floatValue.dot;
                    --this->select;
                }
            }
            if(pressed[UP_KEY] || this->extends.floatValue.open&&pressed[NEXT_KEY]){
                if(this->extends.floatValue.dot<6){
                    ++this->extends.floatValue.dot;
                    ++this->select;
                }
            }
        }else{
            if(pressed[DOWN_KEY] || this->extends.floatValue.open&&pressed[PERV_KEY]){
                *this->extends.floatValue.value -= powf(10, this->extends.floatValue.dot+1-this->select);
            }
            if(pressed[UP_KEY] || this->extends.floatValue.open&&pressed[NEXT_KEY]){
                *this->extends.floatValue.value += powf(10, this->extends.floatValue.dot+1-this->select);
            }
        }
        if(*this->extends.floatValue.value < this->extends.floatValue.min){
            *this->extends.floatValue.value = this->extends.floatValue.min;
        }else if(*this->extends.floatValue.value > this->extends.floatValue.max){
            *this->extends.floatValue.value = this->extends.floatValue.max;
        }
    }
    if(pressed[CENTER_KEY]){
        if(this->select < 0){
            PageKey_back(this);
            return;
        }else{
            this->extends.floatValue.open = !this->extends.floatValue.open;
        }
    }
}

void DoublePage_init(Page *this, char name[], float *value, float min, float max){
    Page_init(this, name, DOUBLE_TYPE);
    this->extends.doubleValue.value = value;
    this->extends.doubleValue.min = min;
    this->extends.doubleValue.max = max;;
    this->extends.doubleValue.dot = 5;
    this->extends.doubleValue.open = 0;
}
void DoublePage_print(Page *this, uint8 row){
    char str[11] = {0};
    Double_toString(*this->extends.doubleValue.value, str, this->extends.doubleValue.dot, 10-this->extends.doubleValue.dot);
    for(int i=0; i<10; ++i){
        if(!str[i]){
            break;
        }
        ips200_show_char_color((row?160:0)+i*8, row?row*16:16, str[i], !row&&this->select==i?this->extends.doubleValue.open?IPS200_DEFAULT_OPENCOLOR:IPS200_DEFAULT_SELECTCOLOR:IPS200_DEFAULT_PENCOLOR);
    }
}
void DoublePage_press(Page *this, uint8 pressed[]){
    if(pressed[LEFT_KEY] || !this->extends.doubleValue.open&&pressed[PERV_KEY]){
        --this->select;
        if(this->select < -1){
            this->select = 9;
        }
    }
    if(pressed[RIGHT_KEY] || !this->extends.doubleValue.open&&pressed[NEXT_KEY]){
        ++this->select;
        if(this->select > 9){
            this->select = -1;
        }
    }
    if(pressed[UP_KEY] || pressed[DOWN_KEY] || pressed[LEFT_KEY] || pressed[RIGHT_KEY]){
        this->extends.doubleValue.open = 0;
    }
    if(pressed[UP_KEY] || pressed[DOWN_KEY] || pressed[PERV_KEY] || pressed[NEXT_KEY]){
        if(this->select < 0){
            if(pressed[UP_KEY]){
                PageKey_back(this);
                return;
            }
        }else if(this->select == 0){
            if(pressed[UP_KEY] || pressed[DOWN_KEY] || this->extends.doubleValue.open&&(pressed[PERV_KEY]||pressed[NEXT_KEY])){
                *this->extends.doubleValue.value = -*this->extends.doubleValue.value;
            }
        }else if(this->select < this->extends.doubleValue.dot+1){
            if(pressed[DOWN_KEY] || this->extends.doubleValue.open&&pressed[PERV_KEY]){
                *this->extends.doubleValue.value -= powf(10, this->extends.doubleValue.dot-this->select);
            }
            if(pressed[UP_KEY] || this->extends.doubleValue.open&&pressed[NEXT_KEY]){
                *this->extends.doubleValue.value += powf(10, this->extends.doubleValue.dot-this->select);
            }
        }else if(this->select == this->extends.doubleValue.dot+1){
            if(pressed[DOWN_KEY] || this->extends.doubleValue.open&&pressed[PERV_KEY]){
                if(this->extends.doubleValue.dot>0){
                    --this->extends.doubleValue.dot;
                    --this->select;
                }
            }
            if(pressed[UP_KEY] || this->extends.doubleValue.open&&pressed[NEXT_KEY]){
                if(this->extends.doubleValue.dot<8){
                    ++this->extends.doubleValue.dot;
                    ++this->select;
                }
            }
        }else{
            if(pressed[DOWN_KEY] || this->extends.doubleValue.open&&pressed[PERV_KEY]){
                *this->extends.doubleValue.value -= powf(10, this->extends.doubleValue.dot+1-this->select);
            }
            if(pressed[UP_KEY] || this->extends.doubleValue.open&&pressed[NEXT_KEY]){
                *this->extends.doubleValue.value += powf(10, this->extends.doubleValue.dot+1-this->select);
            }
        }
        if(*this->extends.doubleValue.value < this->extends.doubleValue.min){
            *this->extends.doubleValue.value = this->extends.doubleValue.min;
        }else if(*this->extends.doubleValue.value > this->extends.doubleValue.max){
            *this->extends.doubleValue.value = this->extends.doubleValue.max;
        }
    }
    if(pressed[CENTER_KEY]){
        if(this->select < 0){
            PageKey_back(this);
            return;
        }else{
            this->extends.doubleValue.open = !this->extends.doubleValue.open;
        }
    }
}

void BoolPage_init(Page *this, char name[], uint8 *value, uint8 dir){
    Page_init(this, name, BOOL_TYPE);
    this->extends.boolValue.value = value;
    this->extends.boolValue.dir = dir;
}
void BoolPage_print(Page *this, uint8 row){
    ips200_show_string_color(row?160:0, row?row*16:16, *(this->extends.boolValue.value)?"true ":"false", !row&&this->select==0 ? IPS200_DEFAULT_SELECTCOLOR : IPS200_DEFAULT_PENCOLOR);
}
void BoolPage_press(Page *this, uint8 pressed[]){
    if(pressed[UP_KEY] || pressed[DOWN_KEY] || pressed[PERV_KEY] || pressed[NEXT_KEY]){
        this->select = -1-this->select;
    }
    if(pressed[LEFT_KEY] || pressed[RIGHT_KEY] || pressed[CENTER_KEY]){
        if(this->select < 0){
            PageKey_back(this);
            return;
        }else{
            if(this->extends.boolValue.dir&0x01&&!*this->extends.boolValue.value ||
                    this->extends.boolValue.dir&0x02&&*this->extends.boolValue.value){
                *this->extends.boolValue.value = !*this->extends.boolValue.value;
            }
        }
    }
}

void EnumPage_init(Page *this, char name[], uint32 *value, char *names[]){
    Page_init(this, name, ENUM_TYPE);
    this->extends.enumValue.value = value;
    for(this->extends.enumValue.size=0; this->extends.enumValue.size<PAGE_ELEMENT_MAX; ++this->extends.enumValue.size){
        if(!*names[this->extends.enumValue.size]){
            break;
        }
    }
    if(this->extends.enumValue.size == 0){
        this->select = -1;
    }
    memcpy(this->extends.enumValue.names, names, sizeof(char*)*this->extends.enumValue.size);
}
void EnumPage_print(Page *this, uint8 row){
    if(!row){
        for(uint8 i=0; i<this->extends.enumValue.size; ++i){
            ips200_show_string_color(0, i*16+16, this->extends.enumValue.names[i], i==*this->extends.enumValue.value?IPS200_DEFAULT_OPENCOLOR:i==this->select?IPS200_DEFAULT_SELECTCOLOR:IPS200_DEFAULT_PENCOLOR);
        }
    }else{
        ips200_show_string_color(160, row*16, this->extends.enumValue.names[*this->extends.enumValue.value], IPS200_DEFAULT_PENCOLOR);
    }
}
void EnumPage_press(Page *this, uint8 pressed[]){
    if(pressed[UP_KEY] || pressed[PERV_KEY]){
        --this->select;
        if(this->select < -1){
            this->select = this->extends.enumValue.size-1;
        }
    }
    if(pressed[DOWN_KEY] || pressed[NEXT_KEY]){
        ++this->select;
        if(this->select > this->extends.enumValue.size-1){
            this->select = -1;
        }
    }
    if(pressed[LEFT_KEY]){
        PageKey_back(this);
        return;
    }
    if(pressed[RIGHT_KEY] || pressed[CENTER_KEY]){
        if(this->select < 0){
            PageKey_back(this);
            return;
        }else{
            *this->extends.enumValue.value=this->select;
        }
    }
}

void FuncPage_init(Page *this, char name[], void (*value)()){
    Page_init(this, name, FUNC_TYPE);
    this->extends.funcValue.value = value;
}
void FuncPage_print(Page *this, uint8 row){
    ips200_show_string_color(row?160:0, row?row*16:16, "run", !row&&this->select==0 ? IPS200_DEFAULT_SELECTCOLOR : IPS200_DEFAULT_PENCOLOR);
}
void FuncPage_press(Page *this, uint8 pressed[]){
    if(pressed[UP_KEY] || pressed[DOWN_KEY] || pressed[PERV_KEY] || pressed[NEXT_KEY]){
        this->select = -1-this->select;
    }
    if(pressed[LEFT_KEY] || pressed[RIGHT_KEY] || pressed[CENTER_KEY]){
        if(this->select < 0){
            PageKey_back(this);
            return;
        }else{
            this->extends.funcValue.value();
        }
    }
}

void ips200_reset_color(){
    ips200_set_pencolor(IPS200_DEFAULT_PENCOLOR);
}

void ips200_set_pencolor(const uint16 color){
    ips200_set_color(color, IPS200_DEFAULT_BGCOLOR);
}

int Int_pow(int this, int pow){
    int ret = 1;
    while(pow--){
        ret*=this;
    }
    return ret;
}
void Int_toString(int this, char *str, uint8 num){
    if(this >= 0){
        *str = '+';
    }else{
        *str = '-';
        this = -this;
    }
    for(int i = num - 1; i >= 0; --i){  // 从高位到低位提取数字
        this %= Int_pow(10, i+1);  // 去掉已提取的最高位数字
        int digit = this / Int_pow(10, i);  // 获取当前位数字
        *(str + num - i) = digit + '0';  // 存入字符串对应位置
    }
    *(str+1+num) = '\0';
}

void Double_toString(double this, char *str, uint8 num, uint8 point){
    if(this >= 0){
        *str = '+';
    }else{
        *str = '-';
        this = -this;
    }
    this+=5*pow(10, -point-1);
    int intThis = (int)this;  // 先将浮点数转换为整数（忽略小数部分，虽然存在精度问题但先聚焦逻辑）
    for(int i = num - 1; i >= 0; --i){  // 从高位到低位提取数字
        intThis %= lround(pow(10, i+1));  // 去掉已提取的最高位数字
        int digit = intThis / lround(pow(10, i));  // 获取当前位数字
        *(str + num - i) = digit + '0';  // 存入字符串对应位置
    }
    *(str+1+num) = '.';
    double decimalPart = this - (int)this;  // 获取小数部分
    for(int i = 0; i < point; ++i){
        decimalPart *= 10;  // 将下一位小数数字移动到整数位
        int digit = (int)decimalPart;  // 获取当前位数字
        *(str + 1 + num + 1 + i) = digit + '0';  // 存入字符串对应位置
        decimalPart -= digit;  // 去掉已提取的数字，准备提取下一位
    }
    *(str+1+num+1+point) = '\0';
}
