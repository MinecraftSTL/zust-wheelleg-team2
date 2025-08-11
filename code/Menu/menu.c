/*
 * menu.c
 *
 *  Created on: 2024��11��14��
 *      Author: minec
 */
#include "sys.h"

#include "menu.h"

#include "zf_device_ips200.h"

void ListPage_print(Page *this, uint8 row);
void ListPage_press(Page *this, uint8 pressed);
void IntPage_print(Page *this, uint8 row);
void IntPage_press(Page *this, uint8 pressed);
void FloatPage_print(Page *this, uint8 row);
void FloatPage_press(Page *this, uint8 pressed);
void FFloatPage_print(Page *this, uint8 row);
void FFloatPage_press(Page *this, uint8 pressed);
void DoublePage_print(Page *this, uint8 row);
void DoublePage_press(Page *this, uint8 pressed);
void BoolPage_print(Page *this, uint8 row);
void BoolPage_press(Page *this, uint8 pressed);

void ips200_reset_color();
void ips200_set_pencolor(const uint16 color);
int Int_pow(int this, int pow);
void Int_toString(int this, char *str, uint8 num);
void Double_toString(float this, char *str, uint8 num, uint8 point);

void Page_init(Page *this, char name[], enum PageExtendsType type){
    strncpy(this->name, name, sizeof(this->name));
    this->type = type;
    this->open = 0;
}

void PageKey_print(Page *this, uint8 row){
    if(!row&&(this->type!=LIST_TYPE || !this->extends.listValue.opened)){
        ips200_show_string_color(8, 0, this->name, this->open<0 ? IPS200_DEFAULT_HIGHLIGHTCOLOR : IPS200_DEFAULT_PENCOLOR);
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
        case FFLOAT_TYPE:
            FFloatPage_print(this, row);
            break;
        case DOUBLE_TYPE:
            DoublePage_print(this, row);
            break;
        case BOOL_TYPE:
            BoolPage_print(this, row);
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
        case FFLOAT_TYPE:
            FFloatPage_press(this, pressed);
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
    memcpy(this->extends.listValue.value, key, sizeof(void*)*size);
    this->extends.listValue.size = size;
    for(int i=0; i<size; ++i){
        key[i]->parent = this;
    }
    this->extends.listValue.opened = 0;
}
void ListPage_setRoot(Page *this){
    this->parent=NULL;
}
void ListPage_print(Page *this, uint8 row){
    if(!row){
        if(this->extends.listValue.opened){
            PageKey_print(this->extends.listValue.value[this->open], row);
            return;
        }
        for(int i=0; i<this->extends.listValue.size; ++i){
            ips200_show_string_color(0, (i+1)*16, this->extends.listValue.value[i]->name, this->open==i ? IPS200_DEFAULT_HIGHLIGHTCOLOR : IPS200_DEFAULT_PENCOLOR);
            PageKey_print(this->extends.listValue.value[i], i+1);
        }
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
        return;
    }
    if(pressed&0x18){
        if(this->open < 0){
            PageKey_back(this);
            return;
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
void IntPage_print(Page *this, uint8 row){
    char str[7] = {0};
    Int_toString(*this->extends.intValue.value, str, 9);
    for(int i=0; i<10; ++i){
        if(!str[i]){
            break;
        }
        ips200_show_char_color((row?160:0)+i*8, row?row*16:16, str[i], !row&&this->open==i ? IPS200_DEFAULT_HIGHLIGHTCOLOR : IPS200_DEFAULT_PENCOLOR);
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
                return;
            }
        }else if(this->open == 0){
            if(pressed&0x03 || this->extends.intValue.opened){
                *this->extends.intValue.value = -*this->extends.intValue.value;
            }
        }else{
            if(pressed&0x02 || this->extends.intValue.opened&&pressed&0x20){
                *this->extends.intValue.value -= Int_pow(10, 9-this->open);
            }
            if(pressed&0x01 || this->extends.intValue.opened&&pressed&0x40){
                *this->extends.intValue.value += Int_pow(10, 9-this->open);
            }
        }
        if(*this->extends.intValue.value < this->extends.intValue.min){
            *this->extends.intValue.value = this->extends.intValue.min;
        }else if(*this->extends.intValue.value > this->extends.intValue.max){
            *this->extends.intValue.value = this->extends.intValue.max;
        }
    }
    if(pressed&0x10){
        if(this->open < 0){
            PageKey_back(this);
            return;
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
void FloatPage_print(Page *this, uint8 row){
    char str[11] = {0};
    Double_toString(*this->extends.floatValue.value, str, 4, 2);
    for(int i=0; i<8; ++i){
        if(!str[i]){
            break;
        }
        ips200_show_char_color((row?160:0)+i*8, row?row*16:16, str[i], !row&&this->open==i ? IPS200_DEFAULT_HIGHLIGHTCOLOR : IPS200_DEFAULT_PENCOLOR);
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
            if(pressed&0x03 || this->extends.floatValue.opened){
                PageKey_back(this);
                return;
            }
        }else if(this->open == 0){
            if(pressed&0x03 || this->extends.floatValue.opened){
                *this->extends.floatValue.value = -*this->extends.floatValue.value;
            }
        }else if(this->open < 5){
            if(pressed&0x02 || this->extends.floatValue.opened&&pressed&0x20){
                *this->extends.floatValue.value -= powf(10, 4-this->open);
            }
            if(pressed&0x01 || this->extends.floatValue.opened&&pressed&0x40){
                *this->extends.floatValue.value += powf(10, 4-this->open);
            }
        }else if(this->open == 5){
            if(pressed&0x02 || this->extends.floatValue.opened&&pressed&0x20){
                *this->extends.floatValue.value/=10;
            }
            if(pressed&0x01 || this->extends.floatValue.opened&&pressed&0x40){
                *this->extends.floatValue.value*=10;
            }
        }else{
            if(pressed&0x02 || this->extends.floatValue.opened&&pressed&0x20){
                *this->extends.floatValue.value -= powf(10, 5-this->open);
            }
            if(pressed&0x01 || this->extends.floatValue.opened&&pressed&0x40){
                *this->extends.floatValue.value += powf(10, 5-this->open);
            }
        }
        if(*this->extends.floatValue.value < this->extends.floatValue.min){
            *this->extends.floatValue.value = this->extends.floatValue.min;
        }else if(*this->extends.floatValue.value > this->extends.floatValue.max){
            *this->extends.floatValue.value = this->extends.floatValue.max;
        }
    }
    if(pressed&0x10){
        if(this->open < 0){
            PageKey_back(this);
            return;
        }else{
            this->extends.floatValue.opened = !this->extends.floatValue.opened;
        }
    }
}

void FFloatPage_init(Page *this, char name[], float *value, float min, float max){
    Page_init(this, name, FFLOAT_TYPE);
    this->extends.fFloatValue.value = value;
    this->extends.fFloatValue.min = min;
    this->extends.fFloatValue.max = max;;
    this->extends.fFloatValue.dot = 4;
    this->extends.fFloatValue.opened = 0;
}
void FFloatPage_print(Page *this, uint8 row){
    char str[11] = {0};
    Double_toString(*this->extends.fFloatValue.value, str, this->extends.fFloatValue.dot, 6-this->extends.fFloatValue.dot);
    for(int i=0; i<8; ++i){
        if(!str[i]){
            break;
        }
        ips200_show_char_color((row?160:0)+i*8, row?row*16:16, str[i], !row&&this->open==i ? IPS200_DEFAULT_HIGHLIGHTCOLOR : IPS200_DEFAULT_PENCOLOR);
    }
}
void FFloatPage_press(Page *this, uint8 pressed){
    if(pressed&0x04 || !this->extends.fFloatValue.opened&&pressed&0x20){
        --this->open;
        if(this->open < -1){
            this->open = 7;
        }
    }
    if(pressed&0x08 || !this->extends.fFloatValue.opened&&pressed&0x40){
        ++this->open;
        if(this->open > 7){
            this->open = -1;
        }
    }
    if(pressed&0x63){
        if(this->open < 0){
            if(pressed&0x03 || this->extends.fFloatValue.opened){
                PageKey_back(this);
                return;
            }
        }else if(this->open == 0){
            if(pressed&0x03 || this->extends.fFloatValue.opened){
                *this->extends.fFloatValue.value = -*this->extends.fFloatValue.value;
            }
        }else if(this->open < this->extends.fFloatValue.dot+1){
            if(pressed&0x02 || this->extends.fFloatValue.opened&&pressed&0x20){
                *this->extends.fFloatValue.value -= powf(10, this->extends.fFloatValue.dot-this->open);
            }
            if(pressed&0x01 || this->extends.fFloatValue.opened&&pressed&0x40){
                *this->extends.fFloatValue.value += powf(10, this->extends.fFloatValue.dot-this->open);
            }
        }else if(this->open == this->extends.fFloatValue.dot+1){
            if(pressed&0x01 || this->extends.fFloatValue.opened&&pressed&0x20){
                if(this->extends.fFloatValue.dot>0){
                    --this->extends.fFloatValue.dot;
                    --this->open;
                }
            }
            if(pressed&0x02 || this->extends.fFloatValue.opened&&pressed&0x40){
                if(this->extends.fFloatValue.dot<6){
                    ++this->extends.fFloatValue.dot;
                    ++this->open;
                }
            }
        }else{
            if(pressed&0x02 || this->extends.fFloatValue.opened&&pressed&0x20){
                *this->extends.fFloatValue.value -= powf(10, this->extends.fFloatValue.dot+1-this->open);
            }
            if(pressed&0x01 || this->extends.fFloatValue.opened&&pressed&0x40){
                *this->extends.fFloatValue.value += powf(10, this->extends.fFloatValue.dot+1-this->open);
            }
        }
        if(*this->extends.fFloatValue.value < this->extends.fFloatValue.min){
            *this->extends.fFloatValue.value = this->extends.fFloatValue.min;
        }else if(*this->extends.fFloatValue.value > this->extends.fFloatValue.max){
            *this->extends.fFloatValue.value = this->extends.fFloatValue.max;
        }
    }
    if(pressed&0x10){
        if(this->open < 0){
            PageKey_back(this);
            return;
        }else{
            this->extends.fFloatValue.opened = !this->extends.fFloatValue.opened;
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
void DoublePage_print(Page *this, uint8 row){
    char str[11] = {0};
    Double_toString(*this->extends.doubleValue.value, str, 5, 3);
    for(int i=0; i<10; ++i){
        if(!str[i]){
            break;
        }
        ips200_show_char_color((row?160:0)+i*8, row?row*16:16, str[i], !row&&this->open==i ? IPS200_DEFAULT_HIGHLIGHTCOLOR : IPS200_DEFAULT_PENCOLOR);
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
            if(pressed&0x03 || this->extends.doubleValue.opened){
                PageKey_back(this);
                return;
            }
        }else if(this->open == 0){
            if(pressed&0x03 || this->extends.doubleValue.opened){
                *this->extends.doubleValue.value = -*this->extends.doubleValue.value;
            }
        }else if(this->open < 6){
            if(pressed&0x02 || this->extends.doubleValue.opened&&pressed&0x20){
                *this->extends.doubleValue.value -= pow(10, 5-this->open);
            }
            if(pressed&0x01 || this->extends.doubleValue.opened&&pressed&0x40){
                *this->extends.doubleValue.value += pow(10, 5-this->open);
            }
        }else if(this->open == 6){
            if(pressed&0x02 || this->extends.doubleValue.opened&&pressed&0x20){
                *this->extends.doubleValue.value/=10;
            }
            if(pressed&0x01 || this->extends.doubleValue.opened&&pressed&0x40){
                *this->extends.doubleValue.value*=10;
            }
        }else{
            if(pressed&0x02 || this->extends.doubleValue.opened&&pressed&0x20){
                *this->extends.doubleValue.value -= powf(10, 6-this->open);
            }
            if(pressed&0x01 || this->extends.doubleValue.opened&&pressed&0x40){
                *this->extends.doubleValue.value += powf(10, 6-this->open);
            }
        }
        if(*this->extends.doubleValue.value < this->extends.doubleValue.min){
            *this->extends.doubleValue.value = this->extends.doubleValue.min;
        }else if(*this->extends.doubleValue.value > this->extends.doubleValue.max){
            *this->extends.doubleValue.value = this->extends.doubleValue.max;
        }
    }
    if(pressed&0x10){
        if(this->open < 0){
            PageKey_back(this);
            return;
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
void BoolPage_print(Page *this, uint8 row){
    ips200_show_string_color(row?160:0, row?row*16:16, *(this->extends.boolValue.value)?"true ":"false", !row&&this->open==0 ? IPS200_DEFAULT_HIGHLIGHTCOLOR : IPS200_DEFAULT_PENCOLOR);
}
void BoolPage_press(Page *this, uint8 pressed){
    if(pressed&0x63){
        this->open = -1-this->open;
    }
    if(pressed&0x1C){
        if(this->open < 0){
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
    for(int i = num - 1; i >= 0; --i){  // �Ӹ�λ����λ��ȡ����
        this %= Int_pow(10, i+1);  // ȥ������ȡ�����λ����
        int digit = this / Int_pow(10, i);  // ��ȡ��ǰλ����
        *(str + num - i) = digit + '0';  // �����ַ�����Ӧλ��
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
    int intThis = (int)this;  // �Ƚ�������ת��Ϊ����������С�����֣���Ȼ���ھ������⵫�Ⱦ۽��߼���
    for(int i = num - 1; i >= 0; --i){  // �Ӹ�λ����λ��ȡ����
        intThis %= lround(pow(10, i+1));  // ȥ������ȡ�����λ����
        int digit = intThis / lround(pow(10, i));  // ��ȡ��ǰλ����
        *(str + num - i) = digit + '0';  // �����ַ�����Ӧλ��
    }
    *(str+1+num) = '.';
    double decimalPart = this - (int)this;  // ��ȡС������
    for(int i = 0; i < point; ++i){
        decimalPart *= 10;  // ����һλС�������ƶ�������λ
        int digit = (int)decimalPart;  // ��ȡ��ǰλ����
        *(str + 1 + num + 1 + i) = digit + '0';  // �����ַ�����Ӧλ��
        decimalPart -= digit;  // ȥ������ȡ�����֣�׼����ȡ��һλ
    }
    *(str+1+num+1+point) = '\0';
}
