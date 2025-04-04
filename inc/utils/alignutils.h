#ifndef ALIGN_UTILS_H
#define ALIGN_UTILS_H

#include "stdint.h"
#include "stdbool.h"
#include "tim.h"

// Simple delay function that takes microseconds 
// Configured around TIM2 with a 1MHz signal, with a overflow reset on UINT32_MAX

void Align_DelayUs(uint32_t us);
void Align_SplitCanId(uint32_t can_id, uint8_t *packet_id, uint8_t *node_id, bool is_extended);

#endif