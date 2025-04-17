#ifndef ALIGN_UTILS_H
#define ALIGN_UTILS_H

#include "stdint.h"
#include "stdbool.h"
#include "tim.h"

// Simple delay function that takes microseconds 
// Configured around TIM2 with a 1MHz signal, with a overflow reset on UINT32_MAX
void Align_InitDelay(TIM_HandleTypeDef *htim);
void Align_DelayUs(TIM_HandleTypeDef *htim, uint16_t us);


#endif