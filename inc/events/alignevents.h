#ifndef ALIGN_EVENTS_H
#define ALIGN_EVENTS_H

#include "tim.h"
#include "stdbool.h"

// The timer used for events should autoreload on a 1ms interval

typedef struct
{
  void* Data; // Pointer to two data buffers
  bool (*Fptr)(void* Data); // Function pointer to the function that will be called when the event is triggered
  uint8_t DataIndex; // Index to the current data buffer
  uint8_t BufferSize;
  uint16_t DataSize;
  uint16_t CycleTime;
  uint32_t LastTime; // The last time the event was triggered
  bool Active; // Is the event active or not
} Align_Events_EventTypeDef;

typedef struct {
  Align_Events_EventTypeDef Events[8]; // Pointer to the event
  uint8_t EventCount; // The number of events
  uint32_t EventTimer; // The timer for the events
} Align_Events_HandleTypeDef;

Align_Events_EventTypeDef* Align_Events_AddEvent(uint16_t cycle_time, void* data, uint16_t data_size, uint8_t buffer_size, bool (*callback)(void* data));
void Align_Events_UpdateData(uint8_t event_index, void* data, uint16_t data_size);
void Align_Events_EventCallback(TIM_HandleTypeDef *htim);
void Align_Events_UpdateEventData(Align_Events_EventTypeDef *event, void *data);
void Align_Events_SetEventActive(Align_Events_EventTypeDef *event, bool active);
void Align_Events_Init(TIM_HandleTypeDef *htim);

extern Align_Events_HandleTypeDef align_events; // The event handler

#endif // ALIGN_EVENTS_H