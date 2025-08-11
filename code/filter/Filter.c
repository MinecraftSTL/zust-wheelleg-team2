/*
 * Filter.c
 *
 *  Created on: 2025Äê3ÔÂ3ÈÕ
 *      Author: minec
 */

#include "Filter.h"

Filter0 Filter0_xAx={0.3, NAN};
Filter1 Filter1_turn={0.9, NAN, NAN}, Filter1_speed={0.9, NAN, NAN}, Filter1_xAx = {0.04, NAN, NAN};

void Filter_param_init(){
}
