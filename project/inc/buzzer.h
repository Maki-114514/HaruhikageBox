#ifndef BUZZER_H
#define BUZZER_H

#include "at32f421.h"
#include "at32f421_tmr.h"

#define BZ1 TMR15
#define BZ2 TMR14

typedef enum
{
    L1 = 0U, L1$, L2, L2$, L3, L4, L4$, L5, L5$, L6, L6$, L7,
    M1, M1$, M2, M2$, M3, M4, M4$, M5, M5$, M6, M6$, M7,
    H1, H1$, H2, H2$, H3, H4, H4$, H5, H5$, H6, H6$, H7,
    BEEP, STOP
} bz_key;

typedef enum
{
    WHOLE = 16U,
    HALF = 8U,
    QURATER = 4U,
    QUAVER = 2U,
    DEMIQUAVER = 1U
}bz_time;

typedef enum
{
    NOP,
    CONTINUE,
}bz_nop;

void BZ_Init(uint8_t volume_percent1, uint8_t volume_percent2);
void BZ_Set_Key(tmr_type *bz_id, bz_key key);

#endif //BUZZER_H
