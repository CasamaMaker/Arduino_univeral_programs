// bq25756_registers.h
#pragma once

#define REG_CHARGE_VOLTAGE_LIMIT                  0x00
#define REG_CHARGE_CURRENT_LIMIT                  0x02
#define REG_INPUT_CURRENT_DPM_LIMIT               0x06
#define REG_INPUT_VOLTAGE_DPM_LIMIT               0x08
#define REG_REVERSE_MODE_INPUT_CURRENT_LIMIT      0x0A
#define REG_REVERSE_MODE_INPUT_VOLTAGE_LIMIT      0x0C

#define REG_PRECHARGE_CURRENT_LIMIT               0x10
#define REG_TERMINATION_CURRENT_LIMIT             0x12

#define REG_PRECHARGE_AND_TERMINATION_CONTROL     0x14
#define REG_TIMER_CONTROL                         0x15
#define REG_THREE_STAGE_CHARGE_CONTROL            0x16
#define REG_CHARGER_CONTROL                       0x17
#define REG_PIN_CONTROL                           0x18
#define REG_POWER_PATH_AND_REVERSE_MODE_CONTROL   0x19
#define REG_MPPT_CONTROL                          0x1A
#define REG_TS_CHARGING_THRESHOLD_CONTROL         0x1B
#define REG_TS_CHARGING_REGION_BEHAVIOR_CONTROL   0x1C
#define REG_TS_REVERSE_MODE_THRESHOLD_CONTROL     0x1D
#define REG_REVERSE_UNDERVOLTAGE_CONTROL          0x1E
#define REG_VAC_MAX_POWER_POINT_DETECTED          0x1F

#define REG_CHARGER_STATUS_1                      0x21
#define REG_CHARGER_STATUS_2                      0x22
#define REG_CHARGER_STATUS_3                       0x23

#define REG_FAULT_STATUS                           0x24
#define REG_CHARGER_FLAG_1                         0x25
#define REG_CHARGER_FLAG_2                         0x26
#define REG_FAULT_FLAG                             0x27

#define REG_CHARGER_MASK_1                         0x28
#define REG_CHARGER_MASK_2                         0x29
#define REG_FAULT_MASK                             0x2A
#define REG_ADC_CONTROL                            0x2B
#define REG_ADC_CHANNEL_CONTROL                    0x2C
#define REG_IAC_ADC                                0x2D
#define REG_IBAT_ADC                               0x2F
#define REG_VAC_ADC                                0x31

#define REG_VBAT_ADC                               0x33
#define REG_TS_ADC                                 0x37
#define REG_VFB_ADC                                0x39

#define REG_GATE_DRIVER_STRENGTH_CONTROL           0x3B
#define REG_GATE_DRIVER_DEAD_TIME_CONTROL          0x3C

#define REG_PART_INFORMATION                       0x3D

#define REG_REVERSE_MODE_BATTERY_DISCHARGE_CURRENT 0x62
