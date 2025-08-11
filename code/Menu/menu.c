/*
 * Menu.c
 *
 *  Created on: 2024年11月14日
 *      Author: minec
 */
#include "Menu.h"
#include "Stl.h"

char Menu_exclude[256][PAGE_PATH_MAX];
uint8 Menu_excludeLen;

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
void AboutPage_print(Page *this, uint8 row);
void AboutPage_press(Page *this, uint8 pressed[]);

void ips200_reset_color();
void ips200_set_pencolor(const uint16 color);
void ips200_show_char_color(uint16 x, uint16 y, const char dat, const uint16 color);
void ips200_show_string_color(uint16 x, uint16 y, const char dat[], const uint16 color);
int Int_pow(int this, int pow);
void Int_toString(int this, char *str, uint8 num);
void Double_toString(float this, char *str, uint8 num, uint8 point);

void Menu_init(char *exclude[]){
    for(Menu_excludeLen=0; Menu_excludeLen<256; ++Menu_excludeLen){
        if(exclude[Menu_excludeLen][0] == '\0'){
            break;
        }
    }
    for(int i=0; i<Menu_excludeLen; ++i){
        strncpy(Menu_exclude[i], exclude[i], PAGE_PATH_MAX);
    }
}

void Page_init(Page *this, char name[], enum PageExtendsType type){
    zf_assert(this && name);
    strncpy(this->name, name, PAGE_NAME_MAX);
    this->type = type;
    this->select = 0;
    this->update = NULL;
}

void Page_print(Page *this, uint8 row){
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
        case ABOUT_TYPE:
            AboutPage_print(this, row);
            break;
    }
}
uint8 Page_press(Page *this, uint8 pressed[]){
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
        case ABOUT_TYPE:
            AboutPage_press(this, pressed);
            break;
    }
    if(pressed[HOME_KEY]){
        Page_home(this);
    }else if(pressed[BACK_KEY]){
        Page_back(this);
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
uint8 Page_readFlash(Page *this){
    char path[PAGE_PATH_MAX+1];
    Page_getPath(this, path);
    for(uint8 i=0; i<Menu_excludeLen; ++i){
        if(String_startWith(path, Menu_exclude[i])){
            return 1;
        }
    }
    uint32 hash = String_hash(path, PAGE_FLASH_MOD);
    uint32 page_num = hash/FLASH_PAGE_USE;
    uint16 page_index = hash%FLASH_PAGE_USE;
    if(!flash_check(0,page_num)){
        return 2;
    }
    flash_data_union flash_union_buffer[EEPROM_PAGE_LENGTH];
    flash_read_page(0, page_num, flash_union_buffer, EEPROM_PAGE_LENGTH);
    if(flash_union_buffer[EEPROM_PAGE_LENGTH-1].uint32_type != (FLASH_KEY^page_num)){
        return 2;
    }
    flash_data_union flash_union = flash_union_buffer[page_index];
    switch(this->type){
        case INT_TYPE:
            IntPage_readFlash(this, flash_union);
            break;
        case FLOAT_TYPE:
            FloatPage_readFlash(this, flash_union);
            break;
        case BOOL_TYPE:
            BoolPage_readFlash(this, flash_union);
            break;
        case ENUM_TYPE:
            EnumPage_readFlash(this, flash_union);
            break;
        default:
            return 1;
    }
    return 0;
}
uint8 Page_writeFlash(Page *this, uint8 check[(PAGE_FLASH_MOD-1)/(sizeof(uint8))+1]){
    char path[PAGE_PATH_MAX+1];
    Page_getPath(this, path);
    for(uint8 i=0; i<Menu_excludeLen; ++i){
        if(String_startWith(path, Menu_exclude[i])){
            return 1;
        }
    }
    uint32 hash = String_hash(path, PAGE_FLASH_MOD);
    uint32 page_num = hash/FLASH_PAGE_USE;
    uint16 page_index = hash%FLASH_PAGE_USE;
    flash_data_union flash_union_buffer[EEPROM_PAGE_LENGTH];
    flash_read_page(0, page_num, flash_union_buffer, EEPROM_PAGE_LENGTH);
    if(check){
        if((check[hash/sizeof(uint8)])&(0x01<<hash%sizeof(uint8))){
            switch(this->type){
                case INT_TYPE:
                case FLOAT_TYPE:
                case BOOL_TYPE:
                case ENUM_TYPE:
                    return 2;
            }
        }
    }else{
        if(flash_union_buffer[EEPROM_PAGE_LENGTH-1].uint32_type != (FLASH_KEY^page_num)){
            return 2;
        }
    }
    flash_data_union flash_union;
    switch(this->type){
        case INT_TYPE:
            flash_union = IntPage_writeFlash(this);
            break;
        case FLOAT_TYPE:
            flash_union = FloatPage_writeFlash(this);
            break;
        case BOOL_TYPE:
            flash_union = BoolPage_writeFlash(this);
            break;
        case ENUM_TYPE:
            flash_union = EnumPage_writeFlash(this);
            break;
        default:
            return 1;
    }
    flash_union_buffer[page_index] = flash_union;
    flash_write_page(0, page_num, flash_union_buffer, EEPROM_PAGE_LENGTH);
    if(check){
        check[hash/sizeof(uint8)] |= 0x01<<hash%sizeof(uint8);
    }
    return 0;
}
void Page_send(Page *page){
    char path[PAGE_PATH_MAX+1];
    Page_getPath(page, path);
    switch(page->type){
        case INT_TYPE:
            printf("^%s=%d$", path, *page->extends.intValue.value);
            break;
        case FLOAT_TYPE:
            printf("^%s=%f$", path, *page->extends.floatValue.value);
            break;
        case DOUBLE_TYPE:
            printf("^%s=%lf$", path, *page->extends.doubleValue.value);
            break;
        case BOOL_TYPE:
            printf("^%s=%d$", path, *page->extends.boolValue.value);
            break;;
        case ENUM_TYPE:
            printf("^%s=%d$", path, *page->extends.enumValue.value);
            break;
        case FUNC_TYPE:
            printf("^%s$", path);
            break;
    }
}
Page *Page_getRoot(Page *this){
    if(this->parent != NULL){
        return Page_getRoot(this->parent);
    }else{
        return this;
    }
}
Page *Page_getOpened(Page *this){
    if(this->type == LIST_TYPE && this->extends.listValue.open){
        return Page_getOpened(this->extends.listValue.value[this->select]);
    }else if(this->parent != NULL && !this->parent->extends.listValue.open){
        return Page_getOpened(this->parent);
    }else{
        return this;
    }
}
void Page_getPath(Page *this, char path[PAGE_PATH_MAX+1]){
    char *end;
    for(end = path+PAGE_PATH_MAX;;){
        size_t len = strlen(this->name);
        end -= len;
        if(end < path){
            path[0] = '\0';
            return;
        }
        strncpy(end, this->name, len);
        if(this->parent == NULL){
            break;
        }
        this = this->parent;
        --end;
        if(end < path){
            path[0] = '\0';
            return;
        }
        *end = '.';
    }
    size_t len = path + PAGE_PATH_MAX - end;
    memmove(path, end, len);
    path[len] = '\0';
}
Page *Page_getByPath(Page *this, char path[PAGE_PATH_MAX+1]){
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
                return Page_getByPath(next, dot+1);
            }else{
                return next;
            }
        }
    }
    return NULL;
}
void Page_allSubRun(Page *this, void (*func)(Page*)){
    func(this);
    if(this->type == LIST_TYPE){
        for(uint8 i=0; i<this->extends.listValue.size; ++i){
            Page_allSubRun(this->extends.listValue.value[i], func);
        }
    }
}
uint8 Page_back(Page *this){
    Page *open = Page_getOpened(this);
    if(open->parent == NULL){
        return 0;
    }else{
        open->parent->extends.listValue.open = 0;
    }
    ips200_clear();
    return 1;
}
void Page_home(Page *this){
    Page *open = Page_getOpened(this);
    while(open->parent != NULL){
        open->parent->extends.listValue.open = 0;
        open = open->parent;
    }
    ips200_clear();
}

void Page_setRoot(Page *this){
    this->parent=NULL;
}
Page *ListPage_init(Page *this, char name[], Page *key[]){
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
    return this;
}
void ListPage_print(Page *this, uint8 row){
    if(!row){
        if(this->extends.listValue.open){
            Page_print(this->extends.listValue.value[this->select], row);
            return;
        }
        for(int8 i=0; i<this->extends.listValue.size; ++i){
            ips200_show_string_color(0, (i+1)*16, this->extends.listValue.value[i]->name, this->select==i ? IPS200_DEFAULT_SELECTCOLOR : IPS200_DEFAULT_PENCOLOR);
            Page_print(this->extends.listValue.value[i], i+1);
        }
    }
}
void ListPage_press(Page *this, uint8 pressed[]){
    if(this->extends.listValue.open){
        Page_press(this->extends.listValue.value[this->select], pressed);
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
        Page_back(this);
        return;
    }
    if(pressed[RIGHT_KEY] || pressed[CENTER_KEY]){
        if(this->select < 0){
            Page_back(this);
            return;
        }else{
            this->extends.listValue.open = 1;
            if(this->update){
                this->update(this);
            }
            ips200_clear();
        }
    }
}

Page *IntPage_init(Page *this, char name[], int32 *value, int32 min, int32 max){
    Page_init(this, name, INT_TYPE);
    this->extends.intValue.value = value;
    this->extends.intValue.min = min;
    this->extends.intValue.max = max;
    this->extends.intValue.open = 0;
    if(flashStatus){
        Page_readFlash(this);
    }
    return this;
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
            if(pressed[UP_KEY] || this->extends.intValue.open){
                Page_back(this);
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
        if(flashStatus){
            Page_writeFlash(this, NULL);
        }
        Page_send(this);
        printf("\r\n");
        if(this->update){
            this->update(this);
        }
    }
    if(pressed[CENTER_KEY]){
        if(this->select < 0){
            Page_back(this);
            return;
        }else{
            this->extends.intValue.open = !this->extends.intValue.open;
        }
    }
    if(pressed[UP_KEY]){
        if(this->select < 0){
            Page_back(this);
            return;
        }
    }
}
void IntPage_readFlash(Page *this, flash_data_union value){
    *this->extends.intValue.value = value.int32_type;
}
flash_data_union IntPage_writeFlash(Page *this){
    flash_data_union ret = {.int32_type=*this->extends.intValue.value};
    return ret;
}

Page *FloatPage_init(Page *this, char name[], float *value, float min, float max){
    Page_init(this, name, FLOAT_TYPE);
    this->extends.floatValue.value = value;
    this->extends.floatValue.min = min;
    this->extends.floatValue.max = max;
    this->extends.floatValue.dot = 1;
    this->extends.floatValue.open = 0;
    if(flashStatus){
        Page_readFlash(this);
    }
    return this;
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
                Page_back(this);
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
        if(flashStatus){
            Page_writeFlash(this, NULL);
        }
        Page_send(this);
        printf("\r\n");
        if(this->update){
            this->update(this);
        }
    }
    if(pressed[CENTER_KEY]){
        if(this->select < 0){
            Page_back(this);
            return;
        }else{
            this->extends.floatValue.open = !this->extends.floatValue.open;
        }
    }
}
void FloatPage_readFlash(Page *this, flash_data_union value){
    *this->extends.floatValue.value = value.float_type;
}
flash_data_union FloatPage_writeFlash(Page *this){
    flash_data_union ret = {.float_type=*this->extends.floatValue.value};
    return ret;
}

Page *DoublePage_init(Page *this, char name[], float *value, float min, float max){
    Page_init(this, name, DOUBLE_TYPE);
    this->extends.doubleValue.value = value;
    this->extends.doubleValue.min = min;
    this->extends.doubleValue.max = max;
    this->extends.doubleValue.dot = 1;
    this->extends.doubleValue.open = 0;
    return this;
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
                Page_back(this);
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
        Page_send(this);
        printf("\r\n");
        if(this->update){
            this->update(this);
        }
    }
    if(pressed[CENTER_KEY]){
        if(this->select < 0){
            Page_back(this);
            return;
        }else{
            this->extends.doubleValue.open = !this->extends.doubleValue.open;
        }
    }
}

Page *BoolPage_init(Page *this, char name[], uint8 *value, uint8 dir){
    Page_init(this, name, BOOL_TYPE);
    this->extends.boolValue.value = value;
    this->extends.boolValue.dir = dir;
    if(flashStatus){
        Page_readFlash(this);
    }
    return this;
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
            Page_back(this);
            return;
        }else{
            if(this->extends.boolValue.dir&0x01&&!*this->extends.boolValue.value ||
                    this->extends.boolValue.dir&0x02&&*this->extends.boolValue.value){
                *this->extends.boolValue.value = !*this->extends.boolValue.value;
            }
            if(flashStatus){
                Page_writeFlash(this, NULL);
            }
            Page_send(this);
            printf("\r\n");
            if(this->update){
                this->update(this);
            }
        }
    }
}
void BoolPage_readFlash(Page *this, flash_data_union value){
    *this->extends.boolValue.value = value.uint8_type;
}
flash_data_union BoolPage_writeFlash(Page *this){
    flash_data_union ret = {.uint8_type=*this->extends.boolValue.value};
    return ret;
}

Page *EnumPage_init(Page *this, char name[], uint8 *value, char *names[]){
    Page_init(this, name, ENUM_TYPE);
    this->extends.enumValue.value = value;
    for(this->extends.enumValue.size=0; this->extends.enumValue.size<PAGE_ELEMENT_MAX; ++this->extends.enumValue.size){
        if(names[this->extends.enumValue.size][0] == '\0'){
            break;
        }
    }
    if(this->extends.enumValue.size == 0){
        this->select = -1;
    }
    for(int i=0; i<this->extends.enumValue.size; ++i){
        strncpy(this->extends.enumValue.names[i], names[i], PAGE_VALUE_MAX);
    }
    if(flashStatus){
        Page_readFlash(this);
    }
    return this;
}
void EnumPage_print(Page *this, uint8 row){
    if(!row){
        for(uint8 i=0; i<this->extends.enumValue.size; ++i){
            ips200_show_string_color(0, i*16+16, this->extends.enumValue.names[i], i==*this->extends.enumValue.value?IPS200_DEFAULT_OPENCOLOR:i==this->select?IPS200_DEFAULT_SELECTCOLOR:IPS200_DEFAULT_PENCOLOR);
        }
    }else{
        if(*this->extends.enumValue.value < this->extends.enumValue.size){
            ips200_show_string_color(160, row*16, this->extends.enumValue.names[*this->extends.enumValue.value], IPS200_DEFAULT_PENCOLOR);
        }
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
        Page_back(this);
        return;
    }
    if(pressed[RIGHT_KEY] || pressed[CENTER_KEY]){
        if(this->select < 0){
            Page_back(this);
            return;
        }else{
            *this->extends.enumValue.value=this->select;
            if(flashStatus){
                Page_writeFlash(this, NULL);
            }
            Page_send(this);
            printf("\r\n");
            if(this->update){
                this->update(this);
            }
        }
    }
}
void EnumPage_readFlash(Page *this, flash_data_union value){
    *this->extends.enumValue.value = value.uint8_type;
}
flash_data_union EnumPage_writeFlash(Page *this){
    flash_data_union ret = {.uint8_type=*this->extends.enumValue.value};
    return ret;
}

Page *FuncPage_init(Page *this, char name[], void (*value)()){
    Page_init(this, name, FUNC_TYPE);
    this->extends.funcValue.value = value;
    return this;
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
            Page_back(this);
            return;
        }else{
            this->extends.funcValue.value();
            Page_send(this);
            printf("\r\n");
            if(this->update){
                this->update(this);
            }
        }
    }
}
Page *AboutPage_init(Page *this, const uint8 *chinese_buffer, uint8 number){
    Page_init(this, "about", ABOUT_TYPE);
    this->extends.aboutValue.chinese_buffer = chinese_buffer;
    this->extends.aboutValue.number = number;
    return this;
}
void AboutPage_print(Page *this, uint8 row){
    if(!row){
        uint16 y = 16;
        ips200_show_string_color(0, y, "Program by", IPS200_DEFAULT_PENCOLOR);
        y += 16;
        ips200_show_chinese(ips200_width_max-this->extends.aboutValue.number*16, y,
                16, this->extends.aboutValue.chinese_buffer, this->extends.aboutValue.number, IPS200_DEFAULT_PENCOLOR);
        y += 16;
        ips200_show_string_color(0, y, "Menu by", IPS200_DEFAULT_PENCOLOR);
        y += 16;
        ips200_show_chinese(ips200_width_max-StlNumber*16, y,16, Stl, StlNumber, IPS200_DEFAULT_PENCOLOR);
    }
}
void AboutPage_press(Page *this, uint8 pressed[]){
    if(pressed[UP_KEY] || pressed[DOWN_KEY] || pressed[LEFT_KEY] || pressed[RIGHT_KEY] || pressed[CENTER_KEY] || pressed[PERV_KEY] || pressed[NEXT_KEY]){
        Page_back(this);
        return;
    }
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
