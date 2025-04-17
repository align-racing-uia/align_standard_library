#include "alignutils.h"
#include "main.h"


void Align_InitDelay(TIM_HandleTypeDef *htim){
  HAL_TIM_Base_Start(htim);
}

void Align_DelayUs(TIM_HandleTypeDef *htim, uint16_t us){

  uint32_t begin = htim->Instance->CNT;
  uint32_t target = begin + us;
  if(target < begin){
    // Overflow
    // Check if the timer is using a 16 bit timer or 32 bit timer
    if(begin > UINT16_MAX){
      target = target - UINT32_MAX;
    }else{
      target = target - UINT16_MAX;
    }
  }
  while(htim->Instance->CNT < target){}

}
