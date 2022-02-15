#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include"lcd.h"
#include"micro_config.h"
#include"std_types.h"
#include "common_macros.h"
#include "keypad.h"
#include "ultrasound.h"
#define M2_READY 0x10
#define M1_READY 0x20
#define RESET 0x30
#define motor_control0 3
#define motor_control1 4
#define motor_DIR DDRB
#define motor_PRT PORTB
#define Motot_Delay 30

int Height= 70;
int DrugWidth= 13;
int Error= 3;

unsigned char PASSWORD[5] = {0,0,0,0,0};
char data_buffer[4];
int distance = 0,c_m=0;

void ResetPassword(void){
	unsigned char keyy;
	uint8 k[5];
	LCD_displayString("enter pass");
	LCD_goToRowColumn(1,0);
	for(int i=0;i<5; i++)
	{
		keyy = KeyPad_getPressedKey();
		k[i]=keyy;
		LCD_displayString("*");
	}
	_delay_ms(30);
	unsigned char is_true = 1;
	LCD_clearScreen();
	for(int i=0;i<5; i++){
		if (PASSWORD[i]!=k[i]){
			is_true=0;
			break;
		}
	}
	if (is_true==0){
		LCD_clearScreen();
		LCD_displayString("Wrong Password");
		_delay_ms(300);
	}else if(is_true==1){
		LCD_clearScreen();
		LCD_displayString("Enter New Pass");
		LCD_goToRowColumn(1,0);
		for(int i=0;i<5; i++)
		{
			keyy = KeyPad_getPressedKey();
			k[i]=keyy;
			LCD_displayString("*");
		}
		for(int j=0;j<5;j++){
			PASSWORD[j]=k[j];
		}
		LCD_clearScreen();
		LCD_displayString("Pass Reset Succ");
		_delay_ms(300);
	}
}

void CloseLocker(void){
	LCD_clearScreen();
	LCD_displayString("enter any key");
	LCD_goToRowColumn(1,0);
	LCD_displayString("to close");
	KeyPad_getPressedKey();
	motor_PRT &= ~(1<<motor_control0);
	motor_PRT &= ~(1<<motor_control1);
	_delay_ms(Motot_Delay);
	motor_PRT|=1<<motor_control1;
}

void OpenLocker(void){
	unsigned char keyy;
	uint8 k[5];
	LCD_displayString("enter pass");
	LCD_goToRowColumn(1,0);
	for(int i=0;i<5; i++)
	{
		keyy = KeyPad_getPressedKey();
		k[i]=keyy;
		LCD_displayString("*");
	}
	LCD_clearScreen();
	unsigned char is_true = 1;
	for(int i=0;i<5; i++){
		if (PASSWORD[i]!=k[i]){
			is_true=0;
			break;
		}
	}
	if (is_true==0){
		LCD_clearScreen();
		LCD_displayString("Access Denied");
		_delay_ms(200);
	}else if(is_true==1){
		LCD_displayString("Access Granted");
		motor_PRT|=1<<motor_control0;
		motor_PRT|=1<<motor_control1;
		_delay_ms(Motot_Delay);
		motor_PRT&=~(1<<motor_control1);
		_delay_ms(200);
		CloseLocker();
	}
	_delay_ms(100);
}

void disp_count(void){
	distance = get_distance_Ultrasonic_sensor();
	if (distance%DrugWidth<=Error){
		distance -= distance%DrugWidth;
	}
	else if (distance-(distance%DrugWidth)<=Error){
		distance += distance%DrugWidth;
	}
	int DrugCount = 1;
	DrugCount =(Height-distance)/DrugWidth;
	ftoa(DrugCount,data_buffer,0);
	LCD_displayString("Count: ");
	LCD_displayString(data_buffer);

	LCD_goToRowColumn(1,0);
	LCD_displayString("enter any key");
	KeyPad_getPressedKey();
	LCD_clearScreen();
}
int main(void)
{
    uint8 key = 'z';
	LCD_init();
	init_Ultrasonic_sensor();
	motor_DIR|= 1<<motor_control0;
	motor_DIR|= 1<<motor_control1;
	motor_PRT&= ~(1<<motor_control0);
	motor_PRT|= 1<<motor_control1;


	while (1){
        LCD_clearScreen();
		LCD_displayString("Open locker: 1");
		LCD_goToRowColumn(1,0);
		LCD_displayString("Reset pass: 0");
		key = KeyPad_getPressedKey(); /* get the pressed button from keypad */

        switch (key){
            case 0: 
            LCD_clearScreen();
			ResetPassword();
            break;
            
			case 1:
            LCD_clearScreen();
			OpenLocker();
            break;

			case '%':
			LCD_clearScreen();
			disp_count();
			break;
			
			default:
            LCD_clearScreen(); 
            LCD_displayString("enter 0 or 1");
			_delay_ms(100);
			break;
        }	
	}
}