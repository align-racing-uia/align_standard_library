#ifndef ALIGN_CAN_H
#define ALIGN_CAN_H
#include "fdcan.h"
#include "stdint.h"
#include "align.h"
#include "stdbool.h"



typedef enum {
    ALIGN_CAN_SPEED_500KBPS,
    ALIGN_CAN_SPEED_1MBPS,
} Align_CAN_SpeedTypeDef;

Align_StatusTypeDef Align_CAN_Init(FDCAN_HandleTypeDef *hfdcan, Align_CAN_SpeedTypeDef speed,  FDCAN_GlobalTypeDef* fdcan_instance);

Align_StatusTypeDef Align_CAN_Send(FDCAN_HandleTypeDef *hfdcan, uint32_t id, uint8_t *data, uint8_t len, bool ext);

bool Align_CAN_Receive(FDCAN_HandleTypeDef *hfdcan, FDCAN_RxHeaderTypeDef *rxHeader, uint8_t *rxData);
void Align_SplitCanId(uint32_t can_id, uint16_t *packet_id, uint16_t *node_id, bool is_extended);
uint32_t Align_CombineCanId(uint16_t packet_id, uint16_t node_id, bool is_extended);


// For this to work, you need to enable callbacks in the FDCAN peripheral
// To enable it, add #define ALIGN_CAN_USE_BUFFER to the PFP of main.h
#if defined(ALIGN_CAN_USE_BUFFER)

#ifndef ALIGN_CAN_BUFFER_SIZE
#define ALIGN_CAN_BUFFER_SIZE 16
#endif

typedef struct {
    FDCAN_TxHeaderTypeDef header;
    uint8_t data[8];

} Align_CAN_BufferTypeDef ;

extern Align_CAN_BufferTypeDef Align_CAN_Buffer[ALIGN_CAN_BUFFER_SIZE];
extern uint8_t Align_CAN_BufferReadIndex;
extern uint8_t Align_CAN_BufferWriteIndex;
extern uint8_t Align_CAN_BufferCount;

void Align_CAN_MessageSentCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t buffers);
bool Align_CAN_AddToBuffer(FDCAN_HandleTypeDef *hfdcan, uint32_t id, uint8_t *data, uint8_t len, bool ext);


#endif

#endif // ALIGN_CAN_H