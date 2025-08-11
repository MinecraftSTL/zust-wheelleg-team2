/*
 * Menu.h
 *
 *  Created on: 2024Äê11ÔÂ14ÈÕ
 *      Author: minec
 */

#ifndef CODE_MENU_H_
#define CODE_MENU_H_

#include "zf_device_ips200.h"
#include "MyIps200.h"
#include "zf_driver_flash.h"

#define PAGE_ELEMENT_MAX 11
#define PAGE_NAME_MAX 20
#define PAGE_VALUE_MAX 10
#define PAGE_PATH_MAX 256

#define PAGE_FLASH_MOD 32768
#define FLASH_PAGE_USE 500

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
    ENUM_TYPE,
    FUNC_TYPE,
    ABOUT_TYPE,
};
typedef struct Page{
    char name[PAGE_NAME_MAX+1];
    struct Page *parent;
    int8 select;
    enum PageExtendsType type;
    union {
        struct {
            struct Page *value[PAGE_ELEMENT_MAX];
            int8 size;
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
            uint8 *value;
            int8 size;
            char names[PAGE_ELEMENT_MAX][PAGE_VALUE_MAX+1];
        } enumValue;
        struct {
            void (*value)();
        } funcValue;
        struct {
            const uint8 *software_buffer;
            uint8 software_number;
            const uint8 *hardware_buffer;
            uint8 hardware_number;
        } aboutValue;
    } extends;
    void (*update)(struct Page*);
} Page;

void Menu_init(char *exclude[]);
void Page_print(Page *this, uint8 row);
uint8 Page_press(Page *this, uint8 pressed[]);
uint8 Page_readFlash(Page *this);
uint8 Page_writeFlash(Page *this, uint8 check[(PAGE_FLASH_MOD-1)/(sizeof(uint8))+1]);
Page *Page_getRoot(Page *this);
Page *Page_getOpened(Page *this);
void Page_getPath(Page *this, char path[PAGE_PATH_MAX+1]);
Page *Page_getByPath(Page *this, char path[PAGE_PATH_MAX+1]);
void Page_allSubRun(Page *this, void (*func)(Page*));
uint8 Page_back(Page *this);
void Page_home(Page *this);
void Page_setRoot(Page *this);
Page *ListPage_init(Page *this, char name[], Page *key[]);
Page *IntPage_init(Page *this, char name[], int32 *value, int32 max, int32 min);
Page *FloatPage_init(Page *this, char name[], float *value, float max, float min);
Page *DoublePage_init(Page *this, char name[], double *value, double max, double min);
Page *BoolPage_init(Page *this, char name[], uint8 *value, uint8 dir);
Page *EnumPage_init(Page *this, char name[], uint8 *value, char *names[]);
Page *FuncPage_init(Page *this, char name[], void (*value)());
Page *AboutPage_init(Page *this, const uint8 *software_buffer, uint8 software_number, const uint8 *hardware_buffer, uint8 hardware_number);
void Page_send(Page *page);
void IntPage_readFlash(Page *this, flash_data_union value);
flash_data_union IntPage_writeFlash(Page *this);
void FloatPage_readFlash(Page *this, flash_data_union value);
flash_data_union FloatPage_writeFlash(Page *this);
void BoolPage_readFlash(Page *this, flash_data_union value);
flash_data_union BoolPage_writeFlash(Page *this);
void EnumPage_readFlash(Page *this, flash_data_union value);
flash_data_union EnumPage_writeFlash(Page *this);

#endif /* CODE_MENU_H_ */
