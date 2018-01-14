#include "dio.h"
#include "lcd.h"
#include <util/delay.h>

void Lcd_vidInit();
void vidTakeNumber(s8);
void vidAskPassword(void);

u8 u8Shift = 0b00010100;
s8 * s8Message;
s8 number = 0;
u8 flag = 0xff;
s8 u8KeyPad[4][4] = {{'0','c','-',' '},{'1','2','3','+'},{'4','5','6','*'},{'7','8','9','/'}};
s8 s8Password[4] = {'1','3','2','4'};
s8 user[16];
s8 iUser;

void main(void)  {
	Dio_vidSetPortDirection(DIO_PORTD,0b11111111);
	Dio_vidSetPortDirection(DIO_PORTA,0b00000111);
	Dio_vidSetPortDirection(DIO_PORTB,0b00001111);
	Dio_vidSetPortValue(DIO_PORTB,0b11110000);
	Dio_vidSetPortValue(DIO_PORTD,0b00000000);
	Lcd_vidInit();
	while(1) {
		vidAskPassword();
	}
}

void vidChoose(void) {
	switch(u8choose) {
		case 1:
			vidCalc();
			break;
		case 2:
			vidMotor();
			break;
		case 3:
			vidFarah();
			break;
		default
			s8Message = "Invalid Input.";
			Lcd_vidInsertMessage(s8Message);
	}
}

void vidAskPassword(void) {
	while (1) {
		for (u8 r = 0; r < 4; r++) {
			Dio_vidSetPinValue(DIO_PORTB,r,0);
			for (u8 c = 4; c <= 7; c++) {
				if (Dio_u8GetPinValue(DIO_PORTB,c) == 0) {
					if(u8KeyPad[c-4][r] == 'c') {
						Lcd_vidSendCommand(0b00000001); /*Clear screen*/
						number = 0;
						iUser = 0;
						break;
					}
					Lcd_vidWriteCharacter(u8KeyPad[c-4][r]);	
					vidTakeNumber(u8KeyPad[c-4][r]);
				}
			}
			Dio_vidSetPinValue(DIO_PORTB,r,1);
		}
	}
}
void vidTakeNumber(s8 key) {
	if (key == ' ') {
		if (iUser >= 5) {
			s8Message = "Wrong Password";
			Lcd_vidInsertMessage(s8Message);
			iUser = 0;
		}
		else {
			for(s8 x = 0; x < 4; x++) {
				if (s8Password[x] == user[x]) {;
					continue;
				}
				else  {
					s8Message = "Error";
					Lcd_vidInsertMessage(s8Message);
					break;
				}
			}

		}
	}
	else {
		user[iUser] = key;
		iUser++;
	}
}

