#include <Arduino.h>
#include "led_rgb.h"
#include "config.h"
#include "pins.h"
#include "led_status_logic.h"

namespace {
 bool ready=false;
 WA::State current=WA::State::BLUE;

 bool candidate(int pin) {
   // Solo candidatos de salida del ESP32 clasico. Es necesario verificar
   // ademas la placa UAI, strapping, UART y perifericos ya ocupados.
   return pin>=12 && pin<=33 && pin!=12 && pin!=15 && pin!=16 && pin!=17;
 }
 bool wiringOK() {
   const int r=WAPins::PIN_LED_R,g=WAPins::PIN_LED_G,y=WAPins::PIN_LED_B;
   // Este controlador usa tres LEDs discretos activos en HIGH, no RGB de anodo comun.
   if (!WAPins::PINOUT_CONFIRMED || !WAPins::LED_TYPE_CONFIRMED || WAPins::LED_COMMON_ANODE) return false;
   if (!candidate(r)||!candidate(g)||!candidate(y)) return false;
   if (r==g||r==y||g==y) return false;
   if (r==WAPins::PIN_TEMP||g==WAPins::PIN_TEMP||y==WAPins::PIN_TEMP) return false;
   if (r==WAPins::PIN_PH_ADC||g==WAPins::PIN_PH_ADC||y==WAPins::PIN_PH_ADC) return false;
   return true;
 }
 void write(int pin,bool on) {digitalWrite(pin,on?HIGH:LOW);}
}

namespace WALed {
 void begin() {
   ready=false;
   if (!WAConfig::USE_REAL_LED) {
     Serial.println("[LED] Tres indicadores: solo Serial; hardware inactivo");
     return;
   }
   if (!wiringOK()) {
     Serial.println("[LED] BLOQUEO: GPIO/polaridad/resistencias sin verificar");
     return;
   }
   const int ps[]={WAPins::PIN_LED_R,WAPins::PIN_LED_G,WAPins::PIN_LED_B};
   for (int pin:ps) {digitalWrite(pin,LOW);pinMode(pin,OUTPUT);}
   ready=true;
   update();
 }
 void setState(WA::State state) {current=state;update();}
 void update() {
   if (!ready)return;
   const auto output=WALedLogic::fromState(current,millis());
   write(WAPins::PIN_LED_R,output.red);
   write(WAPins::PIN_LED_G,output.green);
   write(WAPins::PIN_LED_B,output.yellow);
 }
}
