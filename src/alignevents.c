#include "alignevents.h"
#include "string.h"

Align_Events_HandleTypeDef align_events = {0};

Align_Events_EventTypeDef *Align_Events_AddEvent(uint16_t cycle_time, void *data, uint16_t data_size, uint8_t buffer_size, bool (*callback)(void *data))
{
  if (align_events.EventCount >= 8)
    Error_Handler(); // Check if the event handler is full
  align_events.Events[align_events.EventCount].CycleTime = cycle_time;
  align_events.Events[align_events.EventCount].Data = data;
  align_events.Events[align_events.EventCount].DataIndex = 0;
  align_events.Events[align_events.EventCount].BufferSize = buffer_size;
  align_events.Events[align_events.EventCount].DataSize = data_size;
  align_events.Events[align_events.EventCount].Fptr = callback;
  align_events.Events[align_events.EventCount].Active = true;
  align_events.EventCount++;
  return &align_events.Events[align_events.EventCount - 1]; // Return the event handle
}

void Align_Events_EventCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM4)
  {
    align_events.EventTimer++;
    for (uint8_t i = 0; i < align_events.EventCount; i++)
    {
      if (align_events.Events[i].Active == true)
      {
        if (align_events.Events[i].LastTime + align_events.Events[i].CycleTime <= align_events.EventTimer)
        {
          bool done = align_events.Events[i].Fptr(((unsigned char *)align_events.Events[i].Data) + (align_events.Events[i].DataIndex * align_events.Events[i].DataSize)); // Call the function pointer with the data
          if(done){
            align_events.Events[i].LastTime = align_events.EventTimer; // Update the last time the event was triggered
            align_events.Events[i].DataIndex++;
            align_events.Events[i].DataIndex %= 2;
          }
        }
      }
    }
  }
}

void Align_Events_Init(TIM_HandleTypeDef *htim)
{
  HAL_TIM_RegisterCallback(htim, HAL_TIM_PERIOD_ELAPSED_CB_ID, &Align_Events_EventCallback);
  HAL_TIM_Base_Start_IT(htim);
}

void Align_Events_UpdateEventData(Align_Events_EventTypeDef *event, void *data)
{

  uint8_t new_index = event->DataIndex + 1;
  new_index %= event->BufferSize; // Get the next index
  memcpy(((unsigned char *)event->Data) + (event->DataSize * new_index), data, event->DataSize); // Copy the data to the second buffer

  event->DataIndex = new_index;
}

void Align_Events_SetEventActive(Align_Events_EventTypeDef *event, bool active)
{
  event->Active = active; // Set the event active or not
}