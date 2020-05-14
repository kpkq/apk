#include <dos.h>
#include <stdio.h>

void interrupt(*oldKeyboard)(...);
int isNotEnded = 1;
void print(int);			
void keyboardBlink();		
int checkBuffer();			
int checkRetCode();			
int setMask(int);			


void interrupt newKeyboard(...) 
{			                        
    int keyCode = inp(0x60);
    printf("return code == %2x\n", keyCode); //Вывести код возврата
	oldKeyboard();					//Старое прерывание
    if (keyCode == 0x81) isNotEnded = 0;    // выход
}

void keyboardBlink()    // моргаем лампочками клавиатуры
{
	if (!checkBuffer()) return;
	for (int i = 0; i <= 10; i++) 
    {
        printf("%d/10 keyboardBlink\n", i);
		if (!setMask(0xED)) return;
		if (!setMask(0x02)) return;
		delay(500);
		if (!setMask(0xED)) return;
		if (!setMask(0x00)) return;
		delay(500);
	}
}

int checkBuffer() // проверка буффера клавиатуры
{
	for (int i = 0; i < 10; i++) 
    {
		if (!(inp(0x64) & 0x02)) return 1;
	}
	printf("Buffer is busy!!!\n");
	return 0;
}

int checkRetCode()
{
	int retCode = inp(0x60);    // получаем код возврата
	if(retCode == 0xFE)     // код возврата соответсвует ошибке
    {
		printf("ERROR.\n"); // сообщаем об ошибке
		return 0;
	}
    else 
    {
        printf("OK!!!\n");  // все хорошо
        return 1;
    }
}

int setMask(int val) 
{
	for (int i = 0; i < 3; i++) 
    {
		outp(0x60, val);    //установка маски
		if (!checkBuffer()) return 0;   // проверка
		if (!checkRetCode()) continue;
		return 1;
	}
}

int main() 
{	
    keyboardBlink();
    _disable();
    oldKeyboard = getvect(0x09);
    setvect(0x09, newKeyboard);
    _enable();
    while(isNotEnded);
	return 0;
}