#include "alignutils.h"
#include "main.h"


void Align_DelayUs(uint32_t us){

  uint32_t begin = htim2.Instance->CNT;
  uint32_t target = begin + us;
  if(target < begin){
    // Overflow
    target = target - UINT32_MAX;
  }
  while(htim2.Instance->CNT < target){}

}

void Align_SplitCanId(uint32_t can_id, uint8_t *packet_id, uint8_t *node_id, bool is_extended){
  if(is_extended){
    *packet_id = (can_id >> 8);
    *node_id = can_id & 0xFF;
  }else{
    *packet_id = (can_id >> 5);
    *node_id = can_id & 0x1F;
  }
}