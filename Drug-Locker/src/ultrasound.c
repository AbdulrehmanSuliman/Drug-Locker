#include <avr/io.h>
#include <util/delay.h>
#include "com_def.h"
#include "main.h"
#include "ultrasound.h"
#include "lcd.h"

unsigned int echo_duration;

void init_Ultrasonic_sensor(void)
{
  Ultrasonic_Triger_Port_Direction |= (1<<ULTRASONIC_TRIGGER);    // Ultrasonic Trigger pin
  Ultrasonic_Triger_Port &= ~(1<<ULTRASONIC_TRIGGER);   // Ultrasonic pin set to low
  
  Ultrasonic_Echo_Port_Direction &= ~(1<<ULTRASONIC_ECHO);   // Ultrasonic Echo pin set to input
  Ultrasonic_Echo_Port |= (1<<ULTRASONIC_ECHO);      // Enable pullup
}

void trigger_Ultrasonic_sensor(void)
{
    Ultrasonic_Triger_Port |= (1<<ULTRASONIC_TRIGGER);
    _delay_us(1);
    Ultrasonic_Triger_Port &= ~(1<<ULTRASONIC_TRIGGER);
}

unsigned int read_Ultrasonic_sensor(void)
{
    echo_duration = 0;   
    while((PINC&(1<<ULTRASONIC_ECHO)) != (1<<ULTRASONIC_ECHO));
    while((PINC&(1<<ULTRASONIC_ECHO)) == (1<<ULTRASONIC_ECHO) )
    {
      echo_duration++;
    }
    return echo_duration;
}


unsigned int get_distance_Ultrasonic_sensor(void)
{
  trigger_Ultrasonic_sensor();
  return read_Ultrasonic_sensor();
}