#ifndef CONFIGURACIO_H
#define CONFIGURACIO_H

// Configuració HW
#define Rtop    249000.0
#define Rbot    30000.0
#define Rfbg    33.0

// Configuració SW
#define Vcharge         14.55       // segons HW: 14.27v - 14.86
#define Icharge         0.9
#define Iin_lim         3.00
#define Vin_lim         10.00
#define Iprecharge      0.25        // 0.25A - 10.0A
#define ITermination    0.25        // 0.25A - 10.0A

#define enable_precharge_control   1
#define enable_temination_control  11
#define enable_charge              1
#define watchdog_reset             1
#define enable_adc                 1

#define watchdog                   0    //  00b = Disable;          01b = 40s;              10b = 80s;                11b = 160s
#define Vbat_lowv                  0    //  00b = 30% x VFB_REG;    01b = 55% x VFB_REG;    10b = 66.7% x VFB_REG;    11b = 71.4% x VFB_REG

#endif
