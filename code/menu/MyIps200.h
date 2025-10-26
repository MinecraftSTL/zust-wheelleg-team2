/*
 * myIps200.h
 *
 *  Created on: 2025Äê4ÔÂ5ÈÕ
 *      Author: MinecraftSTL
 */

#ifndef CODE_MENU_MYIPS200_H_
#define CODE_MENU_MYIPS200_H_

#include "zf_device_ips200.h"
#include "zf_driver_gpio.h"
#include "zf_common_debug.h"

#define IPS200_DEFAULT_SELECTCOLOR         (0x7BFF  )
#define IPS200_DEFAULT_OPENCOLOR           (0x001F  )

void ips200_init_spi();

void ips200_reset_color();
void ips200_set_pencolor(const uint16 color);
void ips200_show_char_color(uint16 x, uint16 y, const char dat, const uint16 color);
void ips200_show_string_color(uint16 x, uint16 y, const char dat[], const uint16 color);

void ips200_show_char_invert(uint16 x, uint16 y, const char dat, uint8 x0, uint8 x1, uint8 y0, uint8 y1);
void ips200_show_string_invert (uint16 x, uint16 y, const char dat[], uint8 x0, uint8 x1, uint8 y0, uint8 y1);

#endif /* CODE_MENU_MYIPS200_H_ */
