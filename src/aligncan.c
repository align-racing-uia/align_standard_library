#include "aligncan.h"

#if defined(ALIGN_CAN_USE_BUFFER)
Align_CAN_BufferTypeDef Align_CAN_Buffer[ALIGN_CAN_BUFFER_SIZE] = {0};
uint8_t Align_CAN_BufferWriteIndex = 0;
uint8_t Align_CAN_BufferReadIndex = 0;
uint8_t Align_CAN_BufferCount = 0;

#endif

// Needs FDCAN to be configured to use the HSE, and the HSE needs to be either a 12MHz crystal, or a 20MHz crystal
Align_StatusTypeDef Align_CAN_Init(FDCAN_HandleTypeDef *hfdcan, Align_CAN_SpeedTypeDef can_speed, FDCAN_GlobalTypeDef *fdcan_instance)
{
    HAL_FDCAN_DeInit(hfdcan); // This overwrites earlier settings

    hfdcan->Instance = fdcan_instance;
    hfdcan->Init.FrameFormat = FDCAN_FRAME_CLASSIC;
    hfdcan->Init.Mode = FDCAN_MODE_NORMAL;
    hfdcan->Init.AutoRetransmission = DISABLE;
    hfdcan->Init.TransmitPause = DISABLE;
    hfdcan->Init.ProtocolException = DISABLE;
    hfdcan->Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;

    // We dont currently use FDCAN
    hfdcan->Init.NominalPrescaler = 16;
    hfdcan->Init.NominalSyncJumpWidth = 1;
    hfdcan->Init.NominalTimeSeg1 = 1;
    hfdcan->Init.NominalTimeSeg2 = 1;

    hfdcan->Init.DataPrescaler = 16;
    hfdcan->Init.DataSyncJumpWidth = 1;
    hfdcan->Init.DataTimeSeg1 = 1;
    hfdcan->Init.DataTimeSeg2 = 1;

#if defined(ALIGN_CAN_USE_BUFFER)

    // We need to override the default TX Complete Callback
    // HAL_FDCAN_RegisterCallback(hfdcan, HAL_FDCAN_TX_FIFO_EMPTY_CB_ID, Align_CAN_MessageSentCallback);
    HAL_FDCAN_RegisterTxBufferCompleteCallback(hfdcan, Align_CAN_MessageSentCallback);

#endif

    switch (HSE_VALUE)
    {
    case 12000000:
        switch (can_speed)
        {
        case ALIGN_CAN_SPEED_500KBPS:
            hfdcan->Init.ClockDivider = FDCAN_CLOCK_DIV1;
            hfdcan->Init.NominalPrescaler = 3;
            hfdcan->Init.NominalSyncJumpWidth = 1;
            hfdcan->Init.NominalTimeSeg1 = 6;
            hfdcan->Init.NominalTimeSeg2 = 1;
            break;

        case ALIGN_CAN_SPEED_1MBPS:
            hfdcan->Init.ClockDivider = FDCAN_CLOCK_DIV1;
            hfdcan->Init.NominalPrescaler = 1;
            hfdcan->Init.NominalSyncJumpWidth = 1;
            hfdcan->Init.NominalTimeSeg1 = 10;
            hfdcan->Init.NominalTimeSeg2 = 1;
            break;

        default:
            while (true)
            {
                // We dont support this can speed yet. Please implement it! :-)
            }
            break;
        }
        break;
    case 20000000:
        switch (can_speed)
        {
        case ALIGN_CAN_SPEED_500KBPS:
            hfdcan->Init.ClockDivider = FDCAN_CLOCK_DIV1;
            hfdcan->Init.NominalPrescaler = 5;
            hfdcan->Init.NominalSyncJumpWidth = 1;
            hfdcan->Init.NominalTimeSeg1 = 6;
            hfdcan->Init.NominalTimeSeg2 = 1;
            break;

        case ALIGN_CAN_SPEED_1MBPS:
            hfdcan->Init.ClockDivider = FDCAN_CLOCK_DIV1;
            hfdcan->Init.NominalPrescaler = 2;
            hfdcan->Init.NominalSyncJumpWidth = 1;
            hfdcan->Init.NominalTimeSeg1 = 8;
            hfdcan->Init.NominalTimeSeg2 = 1;
            break;

        default:
            while (true)
            {
                // We dont support this can speed yet. Please implement it! :-)
            }
            break;
        }
        break;
    default:
        while (true)
        {
            // We dont support this clock speed yet. Please implement it! :-)
        }
        break;
    }

    HAL_StatusTypeDef ret = HAL_FDCAN_Init(hfdcan);
    if (ret != HAL_OK)
    {
        return ALIGN_ERROR;
    }

    if (HAL_FDCAN_Start(hfdcan) != HAL_OK)
    {
        return ALIGN_ERROR;
    }

    return ALIGN_OK;
}

Align_StatusTypeDef Align_CAN_Send(FDCAN_HandleTypeDef *hfdcan, uint32_t id, uint8_t *data, uint8_t len, bool ext)
{
    FDCAN_TxHeaderTypeDef txHeader;
    txHeader.Identifier = id;
    txHeader.DataLength = len;
    txHeader.TxFrameType = FDCAN_DATA_FRAME;
    txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    txHeader.BitRateSwitch = FDCAN_BRS_OFF;
    txHeader.FDFormat = FDCAN_CLASSIC_CAN;
    txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    txHeader.MessageMarker = 0;
    if (ext)
    {
        txHeader.IdType = FDCAN_EXTENDED_ID;
    }
    else
    {
        txHeader.IdType = FDCAN_STANDARD_ID;
    }

    HAL_StatusTypeDef ret = HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &txHeader, data);
    if (ret != HAL_OK)
    {
        return ALIGN_ERROR;
    }
    return ALIGN_OK;
}

bool Align_CAN_Receive(FDCAN_HandleTypeDef *hfdcan, FDCAN_RxHeaderTypeDef *rxHeader, uint8_t *rxData)
{
    if (HAL_FDCAN_GetRxFifoFillLevel(hfdcan, FDCAN_RX_FIFO0) > 0)
    {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, rxHeader, rxData);
        return true;
    }
    else if (HAL_FDCAN_GetRxFifoFillLevel(hfdcan, FDCAN_RX_FIFO1) > 0)
    {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, rxHeader, rxData);
        return true;
    }
    return false;
}

void Align_SplitCanId(uint32_t can_id, uint16_t *packet_id, uint16_t *node_id, bool is_extended)
{
    if (is_extended)
    {
        *packet_id = (can_id >> 8);
        *node_id = can_id & 0xFF;
    }
    else
    {
        *packet_id = (can_id >> 5);
        *node_id = can_id & 0x1F;
    }
}

uint32_t Align_CombineCanId(uint16_t packet_id, uint16_t node_id, bool is_extended)
{
    if (is_extended)
    {
        return ((uint32_t)packet_id << 8) | ((uint32_t)node_id & 0xFF);
    }
    else
    {
        return ((uint32_t)packet_id << 5) | ((uint32_t)node_id & 0x1F);
    }
}

#if defined(ALIGN_CAN_USE_BUFFER)


bool Align_CAN_AddToBuffer(FDCAN_HandleTypeDef *hfdcan, uint32_t id, uint8_t *data, uint8_t len, bool ext)
{

    FDCAN_TxHeaderTypeDef txHeader;
    txHeader.Identifier = id;
    txHeader.DataLength = len;
    txHeader.TxFrameType = FDCAN_DATA_FRAME;
    txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    txHeader.BitRateSwitch = FDCAN_BRS_OFF;
    txHeader.FDFormat = FDCAN_CLASSIC_CAN;
    txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    txHeader.MessageMarker = 0;
    if (ext)
    {
        txHeader.IdType = FDCAN_EXTENDED_ID;
    }
    else
    {
        txHeader.IdType = FDCAN_STANDARD_ID;
    }

    // Try to send the message immediately
    if (HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &txHeader, data) == HAL_OK)
    {
        return true; // Message sent immediately
    }

    // Buffer is full, message will be not be sent
    if (Align_CAN_BufferCount >= ALIGN_CAN_BUFFER_SIZE)
    {
        return false; // Buffer is full
    }

    Align_CAN_Buffer[Align_CAN_BufferWriteIndex].header = txHeader;
    memcpy(Align_CAN_Buffer[Align_CAN_BufferWriteIndex].data, data, len);

    Align_CAN_BufferCount++;
    Align_CAN_BufferWriteIndex++;
    Align_CAN_BufferWriteIndex %= ALIGN_CAN_BUFFER_SIZE;

    return true; // Message will be sent eventually
}

void Align_CAN_MessageSentCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t buffers)
{
    UNUSED(buffers);
    // If anything remains in our own buffer, try to send it
    if (Align_CAN_BufferCount > 0)
    {
        // Send the next message in the buffer
        if(HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &Align_CAN_Buffer[Align_CAN_BufferReadIndex].header, Align_CAN_Buffer[Align_CAN_BufferReadIndex].data) == HAL_OK){
            Align_CAN_BufferCount--;
            Align_CAN_BufferReadIndex++;
            Align_CAN_BufferReadIndex %= ALIGN_CAN_BUFFER_SIZE;
        }
    }
}

#endif