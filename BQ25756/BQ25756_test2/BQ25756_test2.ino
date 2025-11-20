// LLEGEIX TOTS ELS REGISTRES DE COP

#include <Wire.h>

#define BQ25756_I2C_ADDRESS  0x6B   // Exempe: revisar l’adreça real del dispositiu

void escriuRegistre(uint8_t reg, uint16_t value) {
  Wire.beginTransmission(BQ25756_I2C_ADDRESS);
  Wire.write(reg);
  Wire.write((uint8_t)(value >> 8));    // MSB
  Wire.write((uint8_t)(value & 0xFF));  // LSB
  Wire.endTransmission();
}

uint8_t llegeixRegistre(uint8_t reg) {
    Wire.beginTransmission(BQ25756_I2C_ADDRESS);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0) {  // Manté el bus actiu
        Serial.println("Error: No es pot comunicar");
        return 0xFF;
    }

    Wire.requestFrom(BQ25756_I2C_ADDRESS, (uint8_t)1);
    if (Wire.available() < 1) {
        Serial.println("Error: No ha arribat resposta");
        return 0xFF;
    }

    return Wire.read();
}

uint16_t llegeixRegistreDoble(uint8_t regAlt, uint8_t regBaix) {
  regAlt = llegeixRegistre(0x31);
  regBaix = llegeixRegistre(0x32);
  uint16_t code = ((uint16_t)regAlt << 8) | regBaix;
  return code;
}

bool setBitRegistre(uint8_t adresa, uint8_t bit, bool valor_bool) {
    
    // 1. Validació de l'índex del bit
    if (bit > 7) {
        // 
        return false; // Índex del bit fora de rang
    }

    // 2. Lectura del valor actual del registre
    uint8_t regVal = llegeixRegistre(adresa);

    // 3. Comprovació d'error de lectura (si 0xFF indica un error)
    if (regVal == 0xFF) {
        return false; 
    }

    // 4. Modificació del bit
    if (valor_bool) {
        // Estableix el bit: OR lògic amb una màscara (1 << bit)
        // La màscara (1 << bit) genera un '1' a la posició del bit i '0' a la resta.
        regVal |= (1 << bit);
    } else {
        // Neteja el bit: AND lògic amb la inversió d'una màscara (~(1 << bit))
        // ~(1 << bit) genera un '0' a la posició del bit i '1' a la resta.
        regVal &= ~(1 << bit);
    }

    // 5. Escriptura del valor modificat al registre
    escriuRegistre(adresa, regVal);

    return true;
}


void llegeits_tots_els_registres(){
  for(int i=0; i < 62; i++){
    Serial.print("> 0x");
    Serial.print(i, HEX);
    Serial.print(" ");
    uint8_t valor = llegeixRegistre(i);
    Serial.print(valor, BIN);
    delay(10);
    Serial.print(" ");
    Serial.println(valor, HEX);
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(1000);
  

  llegeits_tots_els_registres();


  pinMode(11, OUTPUT);
  // pinMode(12, OUTPUT);
  pinMode(A2, INPUT);

  Serial.println("configurat!");
}


void loop() {

  digitalWrite(11, LOW);

  

  Serial.println(llegeixRegistre(0x21), BIN);   // Charger Status 1 
  Serial.println(llegeixRegistre(0x24), BIN);   // Fault Status

  Serial.println(llegeixRegistreDoble(0x31, 0x32)); //VAC ADC
  Serial.println(llegeixRegistreDoble(0x33, 0x34)); //VBAT ADC
  
  Serial.println("--");

  delay(1000);

}
