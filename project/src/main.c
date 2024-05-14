/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

#include "at32f421_board.h"
#include "at32f421_clock.h"
#include "buzzer.h"
#include "track1.h"
#include "track2.h"

#define SPEED 15
#define NOPTIME 1

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
    system_clock_config();

    delay_init();
    BZ_Init(50, 40);

    while (1)
    {
        uint32_t total_time = 0;

        uint32_t track1_num = 0, track2_num = 0;
        uint32_t track1_clicks = 0, track2_clicks = 0;

        uint8_t track1_aviliable = 1, track2_aviliable = 1;

        while (1)
        {
            if (track1_aviliable)
            {
                if (track1_num > 0)
                {
                    switch (track1_nop[track1_num - 1])
                    {
                        case NOP:
                            BZ_Set_Key(BZ1, STOP);
                            break;
                        case CONTINUE:
                            break;
                    }
                    delay_ms(NOPTIME * 5);
                }

                BZ_Set_Key(BZ1, track1_music[track1_num]);
                track1_clicks += track1_time[track1_num];

                track1_num++;
                track1_aviliable = 0;
            }

            if (track2_aviliable)
            {
                if (track2_num > 0)
                {
                    switch (track2_nop[track2_num - 1])
                    {
                        case NOP:
                            BZ_Set_Key(BZ2, STOP);
                            break;
                        case CONTINUE:
                            break;
                    }
                    delay_ms(NOPTIME * 5);
                }

                BZ_Set_Key(BZ2, track2_music[track2_num]);
                track2_clicks += track2_time[track2_num];

                track2_num++;
                track2_aviliable = 0;
            }

            if(total_time == track1_clicks)
            {
                track1_aviliable = 1;
            }


            if(total_time == track2_clicks)
            {
                track2_aviliable = 1;
            }


            if(total_time == 12 * 61)
            {
                total_time = 0;

                track1_num = track2_num = 0;
                track1_clicks = track2_clicks = 0;

                track1_aviliable = track2_aviliable = 1;

                BZ_Set_Key(BZ1, STOP);
                BZ_Set_Key(BZ2, STOP);

                delay_ms(2000);
            } else
            {
                total_time++;
            }
            delay_ms(SPEED * 10 * (uint8_t) DEMIQUAVER);
        }
    }
}

/**
  * @}
  */

/**
  * @}
  */
