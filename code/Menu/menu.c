/*
 * menu.c
 *
 *  Created on: 2024年11月14日
 *      Author: minec
 */
#include "sys.h"

#include "menu.h"

#include "zf_device_ips200.h"

void ListPage_print(Page *this);
void ListPage_press(Page *this, uint8 pressed);
void IntPage_print(Page *this);
void IntPage_press(Page *this, uint8 pressed);
void FloatPage_print(Page *this);
void FloatPage_press(Page *this, uint8 pressed);
void DoublePage_print(Page *this);
void DoublePage_press(Page *this, uint8 pressed);
void BoolPage_print(Page *this);
void BoolPage_press(Page *this, uint8 pressed);

void ips200_reset_color();
void ips200_set_pencolor(const uint16 color);
int Int_pow(int this, int pow);
void Int_toString(int this, char *str, uint8 num);
void Double_toString(float this, char *str, uint8 num, uint8 point);

void Page_init(Page *this, char name[], enum PageExtendsType type){
    strncpy(this->name, name, sizeof(this->name) - 1);
//    this->name[sizeof(this->name) - 1] = '\0';
    this->type = type;
    this->open = 0;
}

void PageKey_print(Page *this){
    if(this->type!=LIST_TYPE || !this->extends.listValue.opened){
        ips200_show_string_color(8, 0, this->name, this->open<0 ? IPS200_DEFAULT_HIGHLIGHTCOLOR : IPS200_DEFAULT_PENCOLOR);
    }
    switch(this->type){
        case LIST_TYPE:
            ListPage_print(this);
            break;
        case INT_TYPE:
            IntPage_print(this);
            break;
        case FLOAT_TYPE:
            FloatPage_print(this);
            break;
        case DOUBLE_TYPE:
            DoublePage_print(this);
            break;
        case BOOL_TYPE:
            BoolPage_print(this);
            break;
    }
}
void PageKey_press(Page *this, uint8 pressed){
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
    }
}
Page *PageKey_getOpened(Page *this){
    if(this->type == LIST_TYPE && this->extends.listValue.opened){
        return PageKey_getOpened(this->extends.listValue.value[this->open]);
    }else if(this->parent != NULL && !this->parent->extends.listValue.opened){
        return PageKey_getOpened(this->parent);
    }else{
        return this;
    }
}
uint8 PageKey_back(Page *this){
    Page *opened = PageKey_getOpened(this);
    if(opened->parent == NULL){
        return 0;
    }else{
        opened->parent->extends.listValue.opened = 0;
    }
    ips200_clear();
    return 1;
}
void PageKey_home(Page *this){
    Page *opened = PageKey_getOpened(this);
    while(opened->parent != NULL){
        opened->parent->extends.listValue.opened = 0;
        opened = opened->parent;
    }
    ips200_clear();
}

void ListPage_init(Page *this, char name[], uint8 size, Page *key[]){
    Page_init(this, name, LIST_TYPE);
    if(size == 0){
        this->open = -1;
    }
    memcpy(&this->extends.listValue.value, key, sizeof(Page)*size);
    this->extends.listValue.size = size;
    for(int i=0; i<size; ++i){
        key[i]->parent = this;
    }
    this->extends.listValue.opened = 0;
}
void ListPage_print(Page *this){
    if(this->extends.listValue.opened){
        PageKey_print(this->extends.listValue.value[this->open]);
        return;
    }
    for(int i=0; i<this->extends.listValue.size; ++i){
        ips200_show_string_color(0, (i+1)*16, this->extends.listValue.value[i]->name, this->open==i ? IPS200_DEFAULT_HIGHLIGHTCOLOR : IPS200_DEFAULT_PENCOLOR);
    }
}
void ListPage_press(Page *this, uint8 pressed){
    if(this->extends.listValue.opened){
        PageKey_press(this->extends.listValue.value[this->open], pressed);
        return;
    }
    if(pressed&0x21){
        --this->open;
        if(this->open < -1){
            this->open = this->extends.listValue.size-1;
        }
    }
    if(pressed&0x42){
        ++this->open;
        if(this->open > this->extends.listValue.size-1){
            this->open = -1;
        }
    }
    if(pressed&0x04){
        PageKey_back(this);
    }
    if(pressed&0x18){
        if(this->open < 0){
            PageKey_back(this);
        }else{
            this->extends.listValue.opened = 1;
            ips200_clear();
        }
    }
}

void IntPage_init(Page *this, char name[], int32 *value, int32 min, int32 max){
    Page_init(this, name, INT_TYPE);
    this->extends.intValue.value = value;
    this->extends.intValue.min = min;
    this->extends.intValue.max = max;
    this->extends.intValue.opened = 0;
}
void IntPage_print(Page *this){
    char str[7] = {0};
    Int_toString(*this->extends.intValue.value, str, 9);
    for(int i=0; i<10; ++i){
        if(!str[i]){
            break;
        }
        ips200_show_char_color(i*8, 16, str[i], this->open==i ? IPS200_DEFAULT_HIGHLIGHTCOLOR : IPS200_DEFAULT_PENCOLOR);
    }
}
void IntPage_press(Page *this, uint8 pressed){
    if(pressed&0x04 || !this->extends.intValue.opened&&pressed&0x20){
        --this->open;
        if(this->open < -1){
            this->open = 9;
        }
    }
    if(pressed&0x08 || !this->extends.intValue.opened&&pressed&0x40){
        ++this->open;
        if(this->open > 9){
            this->open = -1;
        }
    }
    if(pressed&0x0C){
        this->extends.intValue.opened = 0;
    }
    if(pressed&0x63){
        if(this->open < 0){
            if(pressed&0x03 || this->extends.intValue.opened){
                PageKey_back(this);
            }
        }else if(this->open == 0){
            if(pressed&0x03 || this->extends.intValue.opened){
                *this->extends.intValue.value = -*this->extends.intValue.value;
                if(*this->extends.intValue.value < this->extends.intValue.min){
                    *this->extends.intValue.value = this->extends.intValue.min;
                }
                if(*this->extends.intValue.value > this->extends.intValue.max){
                    *this->extends.intValue.value = this->extends.intValue.max;
                }
            }
        }else{
            if(pressed&0x02 || this->extends.intValue.opened&&pressed&0x20){
                *this->extends.intValue.value -= Int_pow(10, 9-this->open);
                if(*this->extends.intValue.value < this->extends.intValue.min){
                    *this->extends.intValue.value = this->extends.intValue.min;
                }
            }
            if(pressed&0x01 || this->extends.intValue.opened&&pressed&0x40){
                *this->extends.intValue.value += Int_pow(10, 9-this->open);
                if(*this->extends.intValue.value > this->extends.intValue.max){
                    *this->extends.intValue.value = this->extends.intValue.max;
                }
            }
        }
    }
    if(pressed&0x10){
        if(this->open < 0){
            PageKey_back(this);
        }else{
            this->extends.intValue.opened = !this->extends.intValue.opened;
        }
    }
}

void FloatPage_init(Page *this, char name[], float *value, float min, float max){
    Page_init(this, name, FLOAT_TYPE);
    this->extends.floatValue.value = value;
    this->extends.floatValue.min = min;
    this->extends.floatValue.max = max;
    this->extends.floatValue.opened = 0;
}
void FloatPage_print(Page *this){
    char str[11] = {0};
    Double_toString(*this->extends.floatValue.value, str, 4, 2);
    for(int i=0; i<8; ++i){
        if(!str[i]){
            break;
        }
        ips200_show_char_color(i*8, 16, str[i], this->open==i ? IPS200_DEFAULT_HIGHLIGHTCOLOR : IPS200_DEFAULT_PENCOLOR);
    }
}
void FloatPage_press(Page *this, uint8 pressed){
    if(pressed&0x04 || !this->extends.floatValue.opened&&pressed&0x20){
        --this->open;
        if(this->open < -1){
            this->open = 7;
        }
    }
    if(pressed&0x08 || !this->extends.floatValue.opened&&pressed&0x40){
        ++this->open;
        if(this->open > 7){
            this->open = -1;
        }
    }
    if(pressed&0x63){
        if(this->open < 0){
            if(pressed&0x03 || this->extends.intValue.opened){
                PageKey_back(this);
            }
        }else if(this->open == 0){
            if(pressed&0x03 || this->extends.intValue.opened){
                *this->extends.floatValue.value = -*this->extends.floatValue.value;
                if(*this->extends.floatValue.value < this->extends.floatValue.min){
                    *this->extends.floatValue.value = this->extends.floatValue.min;
                }
                if(*this->extends.floatValue.value > this->extends.floatValue.max){
                    *this->extends.floatValue.value = this->extends.floatValue.max;
                }
            }
        }else if(this->open < 5){
            if(pressed&0x02 || this->extends.floatValue.opened&&pressed&0x20){
                *this->extends.floatValue.value -= powf(10, 4-this->open);
                if(*this->extends.floatValue.value < this->extends.floatValue.min){
                    *this->extends.floatValue.value = this->extends.floatValue.min;
                }
            }
            if(pressed&0x01 || this->extends.floatValue.opened&&pressed&0x40){
                *this->extends.floatValue.value += powf(10, 4-this->open);
                if(*this->extends.floatValue.value > this->extends.floatValue.max){
                    *this->extends.floatValue.value = this->extends.floatValue.max;
                }
            }
        }else if(this->open == 5){
            if(pressed&0x02 || this->extends.floatValue.opened&&pressed&0x20){
                *this->extends.floatValue.value/=10;
            }
            if(pressed&0x01 || this->extends.floatValue.opened&&pressed&0x40){
                *this->extends.floatValue.value*=10;
            }
            if(*this->extends.floatValue.value < this->extends.floatValue.min){
                *this->extends.floatValue.value = this->extends.floatValue.min;
            }
            if(*this->extends.floatValue.value > this->extends.floatValue.max){
                *this->extends.floatValue.value = this->extends.floatValue.max;
            }
        }else{
            if(pressed&0x02 || this->extends.floatValue.opened&&pressed&0x20){
                *this->extends.floatValue.value -= powf(10, 5-this->open);
                if(*this->extends.floatValue.value < this->extends.floatValue.min){
                    *this->extends.floatValue.value = this->extends.floatValue.min;
                }
            }
            if(pressed&0x01 || this->extends.floatValue.opened&&pressed&0x40){
                *this->extends.floatValue.value += powf(10, 5-this->open);
                if(*this->extends.floatValue.value > this->extends.floatValue.max){
                    *this->extends.floatValue.value = this->extends.floatValue.max;
                }
            }
        }
    }
    if(pressed&0x10){
        if(this->open < 0){
            PageKey_back(this);
        }else{
            this->extends.floatValue.opened = !this->extends.floatValue.opened;
        }
    }
}

void DoublePage_init(Page *this, char name[], double *value, float min, float max){
    Page_init(this, name, DOUBLE_TYPE);
    this->extends.doubleValue.value = value;
    this->extends.doubleValue.min = min;
    this->extends.doubleValue.max = max;
    this->extends.doubleValue.opened = 0;
}
void DoublePage_print(Page *this){
    char str[11] = {0};
    Double_toString(*this->extends.doubleValue.value, str, 5, 3);
    for(int i=0; i<10; ++i){
        if(!str[i]){
            break;
        }
        ips200_show_char_color(i*8, 16, str[i], this->open==i ? IPS200_DEFAULT_HIGHLIGHTCOLOR : IPS200_DEFAULT_PENCOLOR);
    }
}
void DoublePage_press(Page *this, uint8 pressed){
    if(pressed&0x04 || !this->extends.doubleValue.opened&&pressed&0x20){
        --this->open;
        if(this->open < -1){
            this->open = 9;
        }
    }
    if(pressed&0x08 || !this->extends.doubleValue.opened&&pressed&0x40){
        ++this->open;
        if(this->open > 9){
            this->open = -1;
        }
    }
    if(pressed&0x63){
        if(this->open < 0){
            if(pressed&0x03 || this->extends.intValue.opened){
                PageKey_back(this);
            }
        }else if(this->open == 0){
            if(pressed&0x03 || this->extends.intValue.opened){
                *this->extends.doubleValue.value = -*this->extends.doubleValue.value;
                if(*this->extends.doubleValue.value < this->extends.doubleValue.min){
                    *this->extends.doubleValue.value = this->extends.doubleValue.min;
                }
                if(*this->extends.doubleValue.value > this->extends.doubleValue.max){
                    *this->extends.doubleValue.value = this->extends.doubleValue.max;
                }
            }
        }else if(this->open < 6){
            if(pressed&0x02 || this->extends.doubleValue.opened&&pressed&0x20){
                *this->extends.doubleValue.value -= pow(10, 5-this->open);
                if(*this->extends.doubleValue.value < this->extends.doubleValue.min){
                    *this->extends.doubleValue.value = this->extends.doubleValue.min;
                }
            }
            if(pressed&0x01 || this->extends.doubleValue.opened&&pressed&0x40){
                *this->extends.doubleValue.value += pow(10, 5-this->open);
                if(*this->extends.doubleValue.value > this->extends.doubleValue.max){
                    *this->extends.doubleValue.value = this->extends.doubleValue.max;
                }
            }
        }else if(this->open == 6){
            if(pressed&0x02 || this->extends.doubleValue.opened&&pressed&0x20){
                *this->extends.doubleValue.value/=10;
            }
            if(pressed&0x01 || this->extends.doubleValue.opened&&pressed&0x40){
                *this->extends.doubleValue.value*=10;
            }
            if(*this->extends.doubleValue.value < this->extends.doubleValue.min){
                *this->extends.doubleValue.value = this->extends.doubleValue.min;
            }
            if(*this->extends.doubleValue.value > this->extends.doubleValue.max){
                *this->extends.doubleValue.value = this->extends.doubleValue.max;
            }
        }else{
            if(pressed&0x02 || this->extends.doubleValue.opened&&pressed&0x20){
                *this->extends.doubleValue.value -= powf(10, 6-this->open);
                if(*this->extends.doubleValue.value < this->extends.doubleValue.min){
                    *this->extends.doubleValue.value = this->extends.doubleValue.min;
                }
            }
            if(pressed&0x01 || this->extends.doubleValue.opened&&pressed&0x40){
                *this->extends.doubleValue.value += powf(10, 6-this->open);
                if(*this->extends.doubleValue.value > this->extends.doubleValue.max){
                    *this->extends.doubleValue.value = this->extends.doubleValue.max;
                }
            }
        }
    }
    if(pressed&0x10){
        if(this->open < 0){
            PageKey_back(this);
        }else{
            this->extends.doubleValue.opened = !this->extends.doubleValue.opened;
        }
    }
}

void BoolPage_init(Page *this, char name[], uint8 *value, uint8 dir){
    Page_init(this, name, BOOL_TYPE);
    this->extends.boolValue.value = value;
    this->extends.boolValue.dir = dir;
}
void BoolPage_print(Page *this){
    ips200_show_string_color(0, 16, *(this->extends.boolValue.value)?"true ":"false", this->open==0 ? IPS200_DEFAULT_HIGHLIGHTCOLOR : IPS200_DEFAULT_PENCOLOR);
}
void BoolPage_press(Page *this, uint8 pressed){
    if(pressed&0x63){
        this->open = -1-this->open;
    }
    if(pressed&0x1C){
        if(this->open < 0){
            PageKey_back(this);
        }else{
            if(this->extends.boolValue.dir&0x01&&!*this->extends.boolValue.value ||
                    this->extends.boolValue.dir&0x02&&*this->extends.boolValue.value){
                *this->extends.boolValue.value = !*this->extends.boolValue.value;
            }
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
