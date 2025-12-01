// FUNCIONS CORREGIDES PER BQ25756

#include <Wire.h>

#define BQ25756_I2C_ADDRESS  0x6B

// Escriu un registre de 8 bits (registres de control)
void escriuRegistre8(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(BQ25756_I2C_ADDRESS);
  Wire.write(reg);
  Wire.write(value);  // Només 1 byte
  Wire.endTransmission();
}

// Escriu un registre de 16 bits (límits i configuracions grans)
void escriuRegistre16(uint8_t reg, uint16_t value) {
  Wire.beginTransmission(BQ25756_I2C_ADDRESS);
  Wire.write(reg);
  Wire.write((uint8_t)(value & 0xFF));      // LSB primer!
  Wire.write((uint8_t)(value >> 8));        // MSB després!
  Wire.endTransmission();
}

uint8_t llegeixRegistre(uint8_t reg) {
    Wire.beginTransmission(BQ25756_I2C_ADDRESS);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0) {
        Serial.println("Error: No es pot comunicar");
        return 0xFF;
    }

    Wire.requestFrom((int)BQ25756_I2C_ADDRESS, 1);
    if (Wire.available() < 1) {
        Serial.println("Error: No ha arribat resposta");
        return 0xFF;
    }

    return Wire.read();
}

uint16_t llegeixRegistreDoble(uint8_t adresa_MSB) {
  Wire.beginTransmission(BQ25756_I2C_ADDRESS);
  Wire.write(adresa_MSB);
  if (Wire.endTransmission(false) != 0) {
    Serial.println("Error: No es pot comunicar (Doble)");
  return 0xFFFF;
  }

  Wire.requestFrom((int)BQ25756_I2C_ADDRESS, 2);

  if (Wire.available() < 2) {
  Serial.println("Error: No han arribat 2 bytes");
  return 0xFFFF;
  }

  //  uint8_t regAlt = Wire.read();
  //  uint8_t regBaix = Wire.read();
  uint8_t regBaix = Wire.read();
  uint8_t regAlt = Wire.read();

  return ((uint16_t)regAlt << 8) | regBaix;
}

bool setBitRegistre(uint8_t adresa, uint8_t bit, bool valor_bool) {
    if (bit > 7) {
        return false;
    }

    uint8_t regVal = llegeixRegistre(adresa);

    if (regVal == 0xFF) {
        return false; 
    }

    if (valor_bool) {
        regVal |= (1 << bit);
    } else {
        regVal &= ~(1 << bit);
    }

    // USA LA FUNCIÓ DE 8 BITS!
    escriuRegistre8(adresa, regVal);

    return true;
}

void llegeixTotsElsRegistres() {
  Serial.println("\n========== TOTS ELS REGISTRES ==========");
  
  Serial.print("Charge Voltage Limit :              ");
  Serial.println(llegeixRegistreDoble(0x00), BIN);

  Serial.print("Charge Current Limit :              ");
  Serial.println(llegeixRegistreDoble(0x02), BIN);

  Serial.print("Input Current DPM Limit :           ");
  Serial.println(llegeixRegistreDoble(0x06), BIN);

  Serial.print("Input Voltage DPM Limit :           ");
  Serial.println(llegeixRegistreDoble(0x08), BIN);

  Serial.print("Reverse Mode Input Current Limit :  ");
  Serial.println(llegeixRegistreDoble(0x0A), BIN);

  Serial.print("Reverse Mode Input Voltage Limit :  ");
  Serial.println(llegeixRegistreDoble(0x0C), BIN);

  Serial.print("Precharge Current Limit :           ");
  Serial.println(llegeixRegistreDoble(0x10), BIN);

  Serial.print("Termination Current Limit :         ");
  Serial.println(llegeixRegistreDoble(0x12), BIN);

  Serial.print("Precharge and Termination Control  :");
  Serial.println(llegeixRegistreDoble(0x14), BIN);

  Serial.print("Timer Control :                     ");
  Serial.println(llegeixRegistre(0x15), BIN);

  Serial.print("Three-Stage Charge Control :        ");
  Serial.println(llegeixRegistre(0x16), BIN);

  Serial.print("Charger Control :                   ");
  Serial.println(llegeixRegistre(0x17), BIN);

  Serial.print("Pin Control :                       ");
  Serial.println(llegeixRegistre(0x18), BIN);

  Serial.print("Power Path and Reverse Mode Control:");
  Serial.println(llegeixRegistre(0x19), BIN);

  Serial.print("MPPT Control :                      ");
  Serial.println(llegeixRegistre(0x1A), BIN);

  Serial.print("Charger Status 1 :                  ");
  Serial.println(llegeixRegistre(0x21), BIN);

  Serial.print("Charger Status 2 :                  ");
  Serial.println(llegeixRegistre(0x22), BIN);

  Serial.print("Charger Status 3 :                  ");
  Serial.println(llegeixRegistre(0x23), BIN);

  Serial.print("Fault Status :                      ");
  Serial.println(llegeixRegistre(0x24), BIN);

  Serial.print("Charger Flag 1 :                    ");
  Serial.println(llegeixRegistre(0x25), BIN);

  Serial.print("Charger Flag 2 :                    ");
  Serial.println(llegeixRegistre(0x26), BIN);

  Serial.print("Fault Flag :                        ");
  Serial.println(llegeixRegistre(0x27), BIN);

  Serial.print("Fault Mask :                        ");
  Serial.println(llegeixRegistre(0x2A), BIN);

  Serial.print("ADC Control :                       ");
  Serial.println(llegeixRegistre(0x2B), BIN);

  Serial.print("ADC Channel Control  :              ");
  Serial.println(llegeixRegistre(0x2C), BIN);

  Serial.print("IAC ADC :                           ");
  Serial.println(llegeixRegistreDoble(0x2D), BIN);

  Serial.print("IBAT ADC :                          ");
  Serial.println(llegeixRegistreDoble(0x2F), BIN);

  Serial.print("VAC ADC :                           ");
  Serial.println(llegeixRegistreDoble(0x31), BIN);
  
  Serial.print("VBAT ADC :                          ");
  Serial.println(llegeixRegistreDoble(0x33), BIN);
  
  Serial.print("TS ADC :                            ");
  Serial.println(llegeixRegistreDoble(0x37), BIN);
  
  Serial.print("VFB ADC :                           ");
  Serial.println(llegeixRegistreDoble(0x39), BIN);
  
  Serial.print("Gate Driver Strength Control :      ");
  Serial.println(llegeixRegistre(0x3B), BIN);
  
  Serial.print("Part Information :                  ");
  Serial.println(llegeixRegistre(0x3D), BIN);
  
  Serial.println("========================================\n");
}

void configuracioCarregaBateria(){
  Serial.println("\n========== CONFIGURACIÓ CÀRREGA BATERIA AGM 12V/7.2Ah ==========");

  // Configuració HW
  const float Rtop = 249000.0;
  const float Rbot = 30000.0;
  const float Rfbg = 33.0;

  //Configuració SW
  const float Vcharge = 14.25;    // Volts
  const float Icharge = 1.00;     // Amps
  const float Iin_lim = 3.00;     // Amps
  const float Vin_lim = 10.00;    // Volts
  const float Iprecharge = 0.30;  // Amps
  const float ITermination = 0.30;  // Amps

  const bool  enable_precharge_control = true;
  const bool  enable_temination_control = true;
  const bool  enable_charge = true;
  const bool  watchdog_reset = true;
  const bool  enable_adc = true;

  const int watchdog = 0;   //  00b = Disable;    01b = 40s;    10b = 80s;    11b = 160s

  // Desactivem els pins de control per utilitzar I2C
  setBitRegistre(0x18, 7, false);   // EN_ICHG_PIN = 0 (disable)
  setBitRegistre(0x18, 6, false);   // EN_ILIM_HIZ_PIN = 0 (disable)
  delay(10);

  // 1. CHARGE VOLTAGE LIMIT (Reg 0x00-0x01)
  // uint16_t charge_voltage = 31; // VBATREG = 14.55
  float Vfeedback  = Vcharge*(Rbot + Rfbg)/(Rtop + Rbot + Rfbg);
  int charge_voltage_steps = (int) round((Vfeedback - 1.504) * 1000.0 / 2.0);
  escriuRegistre16(0x00, charge_voltage_steps);
  Serial.print("Charge Voltage:           "); Serial.print(Vcharge, 2); Serial.print("V   [steps: "); Serial.print(charge_voltage_steps); Serial.print(", Vfb: "); Serial.print(Vfeedback, 3); Serial.println("v]");

  // 2. CHARGE CURRENT LIMIT (Reg 0x02-0x03)
  // uint16_t charge_current = (10 << 2);  // Per 0.8A: 800mA/50mA = 8, 2 bits shift
  int charge_current_steps = (int) round((Icharge - 0.4) /0.05);
  escriuRegistre16(0x02, (charge_current_steps << 2));
  // Serial.print("Charge Current:           "); Serial.print(((charge_current>>2)*50+400)/1000.0); Serial.println("A");
  Serial.print("Charge Current:           "); Serial.print(Icharge); Serial.print(" A    [steps: "); Serial.print(charge_current_steps); Serial.println("]");

  // 3. INPUT CURRENT LIMIT (Reg 0x06-0x07)
  // uint16_t input_current = (40 << 2);  // Per 3A: 3000mA/50mA = 60, 2 bits shift
  // escriuRegistre16(0x06, input_current);
  // Serial.print("Input Current DPM Limit:  "); Serial.print(((input_current>>2)*50+400)/1000.0); Serial.println("A");
  int input_current_steps = (int) round((Iin_lim - 0.4) /0.05);
  escriuRegistre16(0x06, (input_current_steps << 2));
  Serial.print("Input Current:            "); Serial.print(Iin_lim); Serial.print(" A    [steps: "); Serial.print(input_current_steps); Serial.println("]");

  // 4. INPUT VOLTAGE LIMIT (Reg 0x08-0x09) 
  // uint16_t input_voltage = (290 << 2);  // Per 10V: 10000mV-4100mV/20mV = 290, 2 bits shift
  // escriuRegistre16(0x08, input_voltage);
  // Serial.print("Input Voltage DPM Limit:  "); Serial.print(((input_voltage>>2)*20+4200)/1000.0); Serial.println("V");
  int input_voltage_steps = (int) round((Vin_lim - 4.20) /0.02);
  escriuRegistre16(0x08, (input_voltage_steps << 2));
  Serial.print("Input Current:            "); Serial.print(Vin_lim); Serial.print(" V   [steps: "); Serial.print(input_voltage_steps); Serial.println("]");

  // 5. PRECHARGE CURRENT (Reg 0x10-0x11)    >>> 10-20% del charge current
  // uint16_t precharge_current = (8 << 2);  // Per 0.4A: 400mA-250mA/50mA = 3, 2 bits shift
  // escriuRegistre16(0x10, precharge_current);
  // Serial.print("Precharge Current:        "); Serial.print(((precharge_current>>2)*50+250)/1000.0); Serial.println("A");
  int precharge_current_steps = (int) round((Iprecharge - 0.25) /0.05);
  escriuRegistre16(0x10, (precharge_current_steps << 2));
  Serial.print("Input Current:            "); Serial.print(Iprecharge); Serial.print(" A    [steps: "); Serial.print(precharge_current_steps); Serial.println("]");

  // 6. TERMINATION CURRENT (Reg 0x12-0x13)   >>> 5-10% del charge current
  // uint16_t term_current = (3 << 2);  // Per 250mA(min): 250mA-250mA/50mA = 0, 2 bits shift
  // escriuRegistre16(0x12, term_current);
  // Serial.print("Termination Current:      "); Serial.print(((term_current>>2)*50+250)/1000.0); Serial.println("A");
  int termination_current_steps = (int) round((ITermination - 0.25) /0.05);
  escriuRegistre16(0x12, (precharge_current_steps << 2));
  Serial.print("Input Current:            "); Serial.print(ITermination); Serial.print(" A    [steps: "); Serial.print(termination_current_steps); Serial.println("]");



  // 7. PRECHARGE/TERMINATION CONTROL (Reg 0x14)

  // uint8_t reg14 = llegeixRegistre(0x14);
  // if(enable_temination_control){reg14 |= 0x08;  // EN_TERM=1 (bit3)
  // }else{reg14 &= ~0x08;}                        // EN_TERM=0 (bit3)

  // if(enable_precharge_control){reg14 |= 0x01;  // EN_TERM=1 (bit3)
  // }else{reg14 &= ~0x01;}                        // EN_TERM=0 (bit3)

  // escriuRegistre8(0x14, reg14);

  setBitRegistre(0x14, 0, enable_precharge_control);
  setBitRegistre(0x14, 3, enable_temination_control);

  Serial.println(enable_precharge_control ? "Precharge: ENABLED" : "Precharge: DISABLED");
  Serial.println(enable_temination_control ? "Termination: ENABLED" : "Termination: DISABLED");


  // 8. TIMER CONTROL (Reg 0x15)

  uint8_t reg15 = llegeixRegistre(0x15);
  reg15 &= ~0x30;     // 1) Netejar bits 5:4 (màscara 0b110000 = 0x30)
  reg15 |= (watchdog << 4);     // 2) Escriure el valor del watchdog (0..3) desplaçat als bits 5:4
  escriuRegistre8(0x15, reg15);

  switch (watchdog) {
    case 0: Serial.println("Watchdog: DISABLED"); break;
    case 1: Serial.println("Watchdog: 40s");     break;
    case 2: Serial.println("Watchdog: 80s");     break;
    case 3: Serial.println("Watchdog: 160s");    break;
  }

  // 9. CHARGER CONTROL (Reg 0x17)
  // Habilitem càrrega (però CE pin encara HIGH)
  // uint8_t reg17 = llegeixRegistre(0x17);
  // // reg17 |= 0x21;   // EN_CHG=1 (bit0), WD_RST=1 (bit5)
  // if(enable_charge){reg17 |= 0x01;        // enable_charge=1 (bit1)
  // }else{reg17 &= ~0x01;}                  // enable_charge=0 (bit1)

  // if(watchdog_reset){reg17 |= 0x20;       // watchdog_reset=1 (bit1)
  // }else{reg17 &= ~0x20;}                  // watchdog_reset=0 (bit1)

  // escriuRegistre8(0x17, reg17);

  setBitRegistre(0x17, 0, enable_charge);
  setBitRegistre(0x17, 5, watchdog_reset);
  Serial.println(enable_charge ? "Charging Control: ENABLED" : "Charging Control: DISABLED");
  Serial.println(watchdog_reset ? "Watchdog reseted" : "");


  // 10. ADC CONTROL (Reg 0x2B)   enable_adc
  // setBitRegistre(0x2B, 7, true);   // ADC_EN = 1
  // Serial.println("ADC: ENABLED");
  // uint8_t reg2B = llegeixRegistre(0x2B);
  // if(enable_adc){reg2B |= 0x80;        // enable_charge=1 (bit1)
  // }else{reg2B &= ~0x80;}                  // enable_charge=0 (bit1)

  setBitRegistre(0x2B, 7, enable_adc);
  Serial.println(enable_adc ? "ADC: ENABLED" : "ADC: DISABLED");



  Serial.println("\n✓✓✓ CONFIGURACIÓ COMPLETADA ✓✓✓");
}

void configuracioInicial(){
  // Desactiva watchdog
  setBitRegistre(0x15, 4, false);
  setBitRegistre(0x15, 5, false);
  delay(10);

  // Activa càrrega
  setBitRegistre(0x17, 0, true);    // EN_CHG
  delay(10);

  // Activa ADC
  setBitRegistre(0x2B, 7, true);    // ADC_EN
  delay(10);

  // Activa tots els canals ADC (desactiva tots els bits _DIS)
  setBitRegistre(0x2C, 7, false);   // IAC_ADC_DIS = 0
  setBitRegistre(0x2C, 6, false);   // IBAT_ADC_DIS = 0
  setBitRegistre(0x2C, 5, false);   // VAC_ADC_DIS = 0
  setBitRegistre(0x2C, 4, false);   // VBAT_ADC_DIS = 0
  setBitRegistre(0x2C, 1, false);   // TS_ADC_DIS = 0
  setBitRegistre(0x2C, 0, false);   // VFB_ADC_DIS = 0
  delay(10);

  Serial.println("Configuració completa!");

  // // Verifica configuració
  // Serial.print("ADC Control (0x2B): 0b");
  // Serial.println(llegeixRegistre(0x2B), BIN);
  // Serial.print("ADC Channel (0x2C): 0b");
  // Serial.println(llegeixRegistre(0x2C), BIN);
}

void lecturaADCs(){
  setBitRegistre(0x2B, 7, true);    // ADC_EN
  delay(10);
  setBitRegistre(0x2B, 6, true);
  delay(50);
  
  // uint16_t iac_raw = llegeixRegistreDoble(0x2D);
  // uint16_t ibat_raw = llegeixRegistreDoble(0x2F);
  int16_t iac_raw = (int16_t) llegeixRegistreDoble(0x2D);
  delay(10);
  int16_t ibat_raw = (int16_t) llegeixRegistreDoble(0x2F);
  delay(10);
  uint16_t vac_raw = llegeixRegistreDoble(0x31);
  delay(10);
  uint16_t vbat_raw = llegeixRegistreDoble(0x33);
  delay(10);
  uint16_t ts_raw = llegeixRegistreDoble(0x37);
  delay(10);
  uint16_t vfb_raw = llegeixRegistreDoble(0x39);
  delay(10);
  
  Serial.print("IAC: "); Serial.print(iac_raw * 0.8f); Serial.print(" mA | IBAT: "); Serial.print(ibat_raw * 2.0f);
  Serial.print(" mA | VAC: "); Serial.print(vac_raw * 0.002f); Serial.print(" V | VBAT: "); Serial.print(vbat_raw * 0.002f);
  Serial.print(" V | TS: "); Serial.print(ts_raw); Serial.print(" | VFB: "); Serial.print(vfb_raw * 0.001); Serial.println(" V");

}

void lecturaStatus(){
  Serial.println("\n========== REGISTRES DE STATUS I ERRORS ==========");
  
  // CHARGER STATUS 1 (0x21)
  uint8_t status1 = llegeixRegistre(0x21);
  Serial.print("Charger Status 1 (0x21): 0b");
  Serial.println(status1, BIN);
  Serial.print("  [7] ADC_DONE_STAT: ");
  Serial.println((status1 & 0x80) ? "Conversion complete" : "Not complete");
  Serial.print("  [6] IAC_DPM_STAT: ");
  Serial.println((status1 & 0x40) ? "In Input Current regulation" : "Normal");
  Serial.print("  [5] VAC_DPM_STAT: ");
  Serial.println((status1 & 0x20) ? "In Input Voltage regulation" : "Normal");
  Serial.print("  [3] WD_STAT: ");
  Serial.println((status1 & 0x08) ? "WD timer expired" : "Normal");
  Serial.print("  [2:0] CHARGE_STAT: ");
  uint8_t chg_stat = status1 & 0x07;
  switch(chg_stat) {
    case 0b000: Serial.println("Not Charging"); break;
    case 0b001: Serial.println("Trickle Charge (VBAT < VBAT_SHORT)"); break;
    case 0b010: Serial.println("Pre-Charge (VBAT < VBAT_LOWV)"); break;
    case 0b011: Serial.println("Fast Charge (CC mode)"); break;
    case 0b100: Serial.println("Taper Charge (CV mode)"); break;
    case 0b101: Serial.println("Reserved"); break;
    case 0b110: Serial.println("Top-off Timer Charge"); break;
    case 0b111: Serial.println("Charge Termination Done"); break;
  }
  
  // CHARGER STATUS 2 (0x22)
  uint8_t status2 = llegeixRegistre(0x22);
  Serial.print("\nCharger Status 2 (0x22): 0b");
  Serial.println(status2, BIN);
  Serial.print("  [7] PG_STAT: ");
  Serial.println((status2 & 0x80) ? "Power Good" : "Not Power Good");
  Serial.print("  [6:4] TS_STAT: ");
  uint8_t ts_stat = (status2 >> 4) & 0x07;
  switch(ts_stat) {
    case 0b000: Serial.println("Normal"); break;
    case 0b001: Serial.println("TS Warm"); break;
    case 0b010: Serial.println("TS Cool"); break;
    case 0b011: Serial.println("TS Cold"); break;
    case 0b100: Serial.println("TS Hot"); break;
    default: Serial.println("Unknown"); break;
  }
  Serial.print("  [1:0] MPPT_STAT: ");
  uint8_t mppt_stat = status2 & 0x03;
  switch(mppt_stat) {
    case 0b00: Serial.println("MPPT Disabled"); break;
    case 0b01: Serial.println("MPPT Enabled, But Not Running"); break;
    case 0b10: Serial.println("Full Panel Sweep In Progress"); break;
    case 0b11: Serial.println("Max Power Voltage Detected"); break;
  }
  
  // CHARGER STATUS 3 (0x23)
  uint8_t status3 = llegeixRegistre(0x23);
  Serial.print("\nCharger Status 3 (0x23): 0b");
  Serial.println(status3, BIN);
  Serial.print("  [5:4] FSW_SYNC_STAT: ");
  uint8_t fsw_stat = (status3 >> 4) & 0x03;
  switch(fsw_stat) {
    case 0b00: Serial.println("Normal, no external clock"); break;
    case 0b01: Serial.println("Valid ext. clock detected"); break;
    case 0b10: Serial.println("Pin fault (freq out-of-range)"); break;
    case 0b11: Serial.println("Reserved"); break;
  }
  Serial.print("  [3] CV_TMR_STAT: ");
  Serial.println((status3 & 0x08) ? "CV Timer Expired" : "Normal");
  Serial.print("  [2] REVERSE_STAT: ");
  Serial.println((status3 & 0x04) ? "Reverse Mode ON" : "Reverse Mode OFF");
  
  // FAULT STATUS (0x24)
  uint8_t fault = llegeixRegistre(0x24);
  Serial.print("\nFault Status (0x24): 0b");
  Serial.println(fault, BIN);
  if (fault == 0) {
    Serial.println("  ✓ NO FAULTS");
  } else {
    Serial.println("  ⚠ FAULTS DETECTED:");
    if (fault & 0x80) Serial.println("    [7] VAC_UV_STAT: Input under-voltage");
    if (fault & 0x40) Serial.println("    [6] VAC_OV_STAT: Input over-voltage");
    if (fault & 0x20) Serial.println("    [5] IBAT_OCP_STAT: Battery over-current");
    if (fault & 0x10) Serial.println("    [4] VBAT_OV_STAT: Battery over-voltage");
    if (fault & 0x08) Serial.println("    [3] TSHUT_STAT: Thermal shutdown");
    if (fault & 0x04) Serial.println("    [2] CHG_TMR_STAT: Charge safety timer expired");
    if (fault & 0x02) Serial.println("    [1] DRV_OKZ_STAT: DRV_SUP pin out of range");
  }
  
  // CHARGER FLAG 1 (0x25) - Clear on read
  uint8_t flag1 = llegeixRegistre(0x25);
  Serial.print("\nCharger Flag 1 (0x25): 0b");
  Serial.println(flag1, BIN);
  if (flag1 != 0) {
    Serial.println("  Flags (Clear on Read):");
    if (flag1 & 0x80) Serial.println("    [7] ADC_DONE_FLAG: ADC conversion INT");
    if (flag1 & 0x40) Serial.println("    [6] IAC_DPM_FLAG: Entered Input Current regulation");
    if (flag1 & 0x20) Serial.println("    [5] VAC_DPM_FLAG: Entered Input Voltage regulation");
    if (flag1 & 0x08) Serial.println("    [3] WD_FLAG: WD_STAT rising edge");
    if (flag1 & 0x02) Serial.println("    [1] CV_TMR_FLAG: CV timer expired rising edge");
    if (flag1 & 0x01) Serial.println("    [0] CHARGE_FLAG: CHARGE_STAT changed");
  }
  
  // CHARGER FLAG 2 (0x26) - Clear on read
  uint8_t flag2 = llegeixRegistre(0x26);
  Serial.print("Charger Flag 2 (0x26): 0b");
  Serial.println(flag2, BIN);
  if (flag2 != 0) {
    Serial.println("  Flags (Clear on Read):");
    if (flag2 & 0x80) Serial.println("    [7] PG_FLAG: PG signal toggle");
    if (flag2 & 0x10) Serial.println("    [4] TS_FLAG: TS_STAT changed");
    if (flag2 & 0x08) Serial.println("    [3] REVERSE_FLAG: Reverse Mode toggle");
    if (flag2 & 0x02) Serial.println("    [1] FSW_SYNC_FLAG: FSW_SYNC status changed");
    if (flag2 & 0x01) Serial.println("    [0] MPPT_FLAG: MPPT_STAT changed");
  }
  
  // FAULT FLAG (0x27) - Clear on read
  uint8_t fault_flag = llegeixRegistre(0x27);
  Serial.print("Fault Flag (0x27): 0b");
  Serial.println(fault_flag, BIN);
  if (fault_flag != 0) {
    Serial.println("  ⚠ Fault Flags (Clear on Read):");
    if (fault_flag & 0x80) Serial.println("    [7] VAC_UV_FLAG: Input under-voltage fault");
    if (fault_flag & 0x40) Serial.println("    [6] VAC_OV_FLAG: Input over-voltage fault");
    if (fault_flag & 0x20) Serial.println("    [5] IBAT_OCP_FLAG: Battery over-current fault");
    if (fault_flag & 0x10) Serial.println("    [4] VBAT_OV_FLAG: Battery over-voltage fault");
    if (fault_flag & 0x08) Serial.println("    [3] TSHUT_FLAG: Thermal shutdown fault");
    if (fault_flag & 0x04) Serial.println("    [2] CHG_TMR_FLAG: Safety timer expired");
    if (fault_flag & 0x02) Serial.println("    [1] DRV_OKZ_FLAG: DRV_SUP pin fault");
  }
  
  Serial.println("==================================================\n");
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(1000);
  
  pinMode(11, OUTPUT);    // pin /ChargeEnable
  pinMode(A2, INPUT);     // pin Feedbac

  configuracioInicial();

  digitalWrite(11, HIGH);
  Serial.println("Pin CE = HIGH");

  configuracioCarregaBateria();

  // Serial.println("\nComandes disponibles:");
  // Serial.println("  0 - ON");
  // Serial.println("  1 - OFF");
  // Serial.println("  3 - Llegir registres");
  // Serial.println("  4 - Llegir ADC");
  // Serial.println("  5 - Configurar càrrega bateria AGM 12V/7.2Ah");
  // Serial.println("  6 - STATUS i ERRORS");
}

void loop() {
  if (Serial.available() > 0) {
    char dada = Serial.read();
    if (dada == '0') {
      digitalWrite(11, LOW);
      Serial.println("Pin CE = LOW");
    }
    if (dada == '1') {
      digitalWrite(11, HIGH);
      Serial.println("Pin CE = HIGH");
    }

    if (dada == '2') {
      configuracioInicial();
    }

    if (dada == '3') {
      llegeixTotsElsRegistres();
    }

    if (dada == '4') {
      lecturaADCs();
    }

    if (dada == '5') {
      configuracioCarregaBateria();
    }

    if (dada == '6') {
      lecturaStatus();
    }
  }

  // Watchdog reset periòdic
  static unsigned long previousTime = 0;
  if (millis() - previousTime > 1000) {
    // setBitRegistre(0x17, 5, true);    // WD_RST
    lecturaADCs();
    previousTime = millis();
  }
}