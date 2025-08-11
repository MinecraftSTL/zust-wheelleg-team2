/*
 * Filter.c
 *
 *  Created on: 2025Äê3ÔÂ3ÈÕ
 *      Author: minec
 */

#include "Filter.h"

Filter0 Filter0_xAx={0, NAN};
Filter1 Filter1_turn={0.9, NAN, NAN}, Filter1_speed={0.9, NAN, NAN}, Filter1_xAx = {0, NAN, NAN};
float Filter0_xAx_delta = 0.2, Filter1_xAx_alpha=0.03, Filter0_xAxE_delta = 0.3, Filter1_xAxE_alpha = 0.04;

void Filter_param_init(){
}
