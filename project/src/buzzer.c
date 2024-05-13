#include "buzzer.h"

const uint16_t note_frequency_table[] = {
//Low  do    do#   re    re#   mi    fa    fa#   so    so#   ra    ra#   si
        262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
//Med  do    do#   re    re#   mi    fa    fa#   so    so#   ra    ra#   si
        523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988,
//High do    do#   re    re#   mi    fa    fa#   so    so#   ra    ra#   si
        1046, 1109, 1175, 1245, 1318, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
//     Beep
        2700,
};

static uint8_t bz_volume_percent1;
static uint8_t bz_volume_percent2;

void BZ_Init(uint8_t volume_percent1, uint8_t volume_percent2)
{
    bz_volume_percent1 = volume_percent1;
    bz_volume_percent2 = volume_percent2;

    /* 定时器总线使能 */
    crm_periph_clock_enable(CRM_TMR15_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_TMR14_PERIPH_CLOCK, TRUE);

    /* GPIO总线使能 */
    crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

    /* GPIO初始化 */
    gpio_init_type gpio_init_struct;

    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_pins = GPIO_PINS_2 | GPIO_PINS_4;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init(GPIOA, &gpio_init_struct);

    gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE8, GPIO_MUX_1);
    gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE5, GPIO_MUX_1);

    /* 定时器15初始化 */
    uint32_t prescaler_value = (uint16_t) (SystemCoreClock / 1000000) - 1;

    tmr_base_init(BZ1, 0, prescaler_value);
    tmr_cnt_dir_set(BZ1, TMR_COUNT_UP);
    tmr_clock_source_div_set(BZ1, TMR_CLOCK_DIV1);
    tmr_repetition_counter_set(BZ1, 0);
    tmr_period_buffer_enable(BZ1, FALSE);

    /* configure channel 1 output settings */
    tmr_output_config_type tmr_oc_init_structure;
    tmr_output_default_para_init(&tmr_oc_init_structure);
    tmr_oc_init_structure.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
    tmr_oc_init_structure.oc_idle_state = FALSE;
    tmr_oc_init_structure.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
    tmr_oc_init_structure.oc_output_state = TRUE;
    tmr_output_channel_config(BZ1, TMR_SELECT_CHANNEL_1, &tmr_oc_init_structure);
    tmr_channel_value_set(BZ1, TMR_SELECT_CHANNEL_1, 0);

    tmr_output_channel_buffer_enable(BZ1, TMR_SELECT_CHANNEL_1, FALSE);

    /* tmr enable counter */
    tmr_counter_enable(BZ1, TRUE);



    tmr_base_init(BZ2, 0, prescaler_value);
    tmr_cnt_dir_set(BZ2, TMR_COUNT_UP);
    tmr_clock_source_div_set(BZ2, TMR_CLOCK_DIV1);
    tmr_repetition_counter_set(BZ2, 0);
    tmr_period_buffer_enable(BZ2, FALSE);

    tmr_output_channel_config(BZ2, TMR_SELECT_CHANNEL_1, &tmr_oc_init_structure);
    tmr_channel_value_set(BZ2, TMR_SELECT_CHANNEL_1, 0);

    tmr_output_channel_buffer_enable(BZ2, TMR_SELECT_CHANNEL_1, FALSE);

    /* tmr enable counter */
    tmr_counter_enable(BZ2, TRUE);
}

void BZ_Set_Key(tmr_type *bz_id, bz_key key)
{
    if (key == STOP)
    {
        tmr_output_enable(bz_id, FALSE);
    } else
    {
        uint32_t psc = 1000000 / note_frequency_table[key];
        uint32_t oc;
        if(bz_id == BZ1)
        {
            oc = psc * bz_volume_percent1 / 100;
        } else{
            oc = psc * bz_volume_percent2 / 100;
        }
        tmr_period_value_set(bz_id, psc);
        tmr_channel_value_set(bz_id, TMR_SELECT_CHANNEL_1, oc);
        tmr_output_enable(bz_id, TRUE);
    }
}