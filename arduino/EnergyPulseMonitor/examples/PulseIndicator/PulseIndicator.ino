#include <Arduino.h>
#include <EnergyPulseMonitor.h>
// Connect only SAFE, ISOLATED logic-level pulses. Never connect AC mains.
// A button to GND can simulate pulses, but bouncing may overcount.
#if defined(ARDUINO_ARCH_ESP32)
const int PULSE_PIN=27,ACTIVITY_LED=18;
#define PULSE_ISR_ATTR IRAM_ATTR
#else
const int PULSE_PIN=2,ACTIVITY_LED=8;
#define PULSE_ISR_ATTR
#endif
volatile uint32_t raw_pulses=0;
void PULSE_ISR_ATTR onPulse(){++raw_pulses;}
energy::Meter meter(1000,150.0);
void setup(){
 Serial.begin(115200);pinMode(PULSE_PIN,INPUT_PULLUP);
 pinMode(ACTIVITY_LED,OUTPUT);digitalWrite(ACTIVITY_LED,LOW);
 attachInterrupt(digitalPinToInterrupt(PULSE_PIN),onPulse,FALLING);
}
void loop(){
 noInterrupts();uint32_t count=raw_pulses;interrupts();
 energy::Report r=meter.record(millis()/1000UL,count);
 digitalWrite(ACTIVITY_LED,r.valid&&r.delta_pulses>0?HIGH:LOW);
 Serial.print("valid=");Serial.print(r.valid);
 Serial.print(" delta=");Serial.print(static_cast<unsigned long>(r.delta_pulses));
 Serial.print(" kWh=");Serial.print(r.energy_kwh,6);
 Serial.print(" estimated_cost=");Serial.println(r.estimated_cost,4);
 delay(1000);
}
