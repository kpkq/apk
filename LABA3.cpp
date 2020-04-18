#include <dos.h>

struct VIDEO
{
	unsigned char symb;															
	unsigned char attr;															
};

void get_reg();																	
// IRQ 0-7
void interrupt(*int8) (...);
void interrupt(*int9) (...);
void interrupt(*intA) (...);
void interrupt(*intB) (...);
void interrupt(*intC) (...);
void interrupt(*intD) (...);
void interrupt(*intE) (...);
void interrupt(*intF) (...);

// IRQ 8-15
void interrupt(*int0) (...);
void interrupt(*int1) (...);
void interrupt(*int2) (...);
void interrupt(*int3) (...);
void interrupt(*int4) (...);
void interrupt(*int5) (...);
void interrupt(*int6) (...);
void interrupt(*int7) (...);



void interrupt  new8(...) { get_reg(); int8(); }
void interrupt  new9(...) { get_reg(); int9(); }
void interrupt  newA(...) { get_reg(); intA(); }
void interrupt  newB(...) { get_reg(); intB(); }
void interrupt  newC(...) { get_reg(); intC(); }
void interrupt  newD(...) { get_reg(); intD(); }
void interrupt  newE(...) { get_reg(); intE(); }
void interrupt  newF(...) { get_reg(); intF(); }

void interrupt  new0(...) { get_reg(); int0(); }
void interrupt  new1(...) { get_reg(); int1(); }
void interrupt  new2(...) { get_reg(); int2(); }
void interrupt  new3(...) { get_reg(); int3(); }
void interrupt  new4(...) { get_reg(); int4(); }
void interrupt  new5(...) { get_reg(); int5(); }
void interrupt  new6(...) { get_reg(); int6(); }
void interrupt  new7(...) { get_reg(); int7(); }

void print(int val1, int val2, int val3, int val4, int val5, int val6)	// выводит состояние регистров
{
	static int colourChng = 0;		// перемнная для изменения цвета				
	char tmp;																	
	int i;

	colourChng = (colourChng + (val2/2)%2 ) % 4;									

	unsigned char currentColour;		// замена цвета
	if (colourChng > 1)
		currentColour = 0x6E;
	else 
		currentColour = 0x1E;
	VIDEO far* screen = (VIDEO far*)MK_FP(0xB800, 0);		// прямой доступ к видеопамяти
	
	for (i = 7; i >= 0; i--)		// поделить по битам
	{								// вывод в двоичном представлении
		tmp = val1 % 2;
		val1 /= 2;
		screen->symb = tmp + '0';
		screen->attr = currentColour;
		screen++;
	}
	screen++;						// пробел

	for (i = 7; i >= 0; i--)
	{
		tmp = val2 % 2;
		val2 /= 2; 
		screen->symb = tmp + '0';
		screen->attr = currentColour;
		screen++;
	}
	screen++;

	for (i = 7; i >= 0; i--)
	{
		tmp = val3 % 2;
		val3 /= 2; 
		screen->symb = tmp + '0';
		screen->attr = currentColour;
		screen++;
	}
	screen++;

	for (i = 7; i >= 0; i--)
	{
		tmp = val4 % 2;
		val4 /= 2;
		screen->symb = tmp + '0';
		screen->attr = currentColour;
		screen++;
	}
	screen++;

	for (i = 7; i >= 0; i--)
	{
		tmp = val5 % 2;
		val5 /= 2;
		screen->symb = tmp + '0';
		screen->attr = currentColour;
		screen++;
	}
	screen++;

	for (i = 7; i >= 0; i--)
	{
		tmp = val6 % 2;
		val6 /= 2; 
		screen->symb = tmp + '0';
		screen->attr = currentColour;
		screen++;
	}
}

void get_reg()		// получить состояние регистров
{
	int var1, var2, var3, var4, var5, var6;

								// для ведущего
	outp(0x20, 0x0A);			// чтобы считать регистр запросов записываем в 20h 0Ah
	var1 = inp(0x20);			// считать содержимое из порта 20h
	outp(0x20, 0x0B);			// чтобы считать регистр обслуживания записываем в 20h 0Bh
	var2 = inp(0x20);			// считать содержимое порта 21h
	var3 = inp(0x21);			// получаем значение регистра масок
								// для ведомого
	outp(0xA0, 0x0A);			// чтобы считать регистр запросов записываем в 20h 0Ah
	var4 = inp(0xA0);			// считать содержимое из порта 20h
	outp(0xA0, 0x0B);			// чтобы считать регистр обслуживания записываем в 20h 0Bh
	var5 = inp(0xA0);			// считать содержимое порта 21h
	var6 = inp(0xA1);			// получаем значение регистра масок

	print(var1, var2, var3, var4, var5, var6);
}

void init()
{

	int8 = getvect(0x70);		// старые обработчики
	int9 = getvect(0x71);
	intA = getvect(0x72);
	intB = getvect(0x73);
	intC = getvect(0x74);
	intD = getvect(0x75);
	intE = getvect(0x76);
	intF = getvect(0x77);

	int0 = getvect(0x08);
	int1 = getvect(0x09);
	int2 = getvect(0x0A);
	int3 = getvect(0x0B);
	int4 = getvect(0x0C);
	int5 = getvect(0x0D);
	int6 = getvect(0x0E);
	int7 = getvect(0x0F);

	setvect(0xA0, new0);		// новые обработчики
	setvect(0xA1, new1);
	setvect(0xA2, new2);
	setvect(0xA3, new3);
	setvect(0xA4, new4);
	setvect(0xA5, new5);
	setvect(0xA6, new6);
	setvect(0xA7, new7);

	setvect(0x08, new8);
	setvect(0x09, new9);
	setvect(0x0A, newA);
	setvect(0x0B, newB);
	setvect(0x0C, newC);
	setvect(0x0D, newD);
	setvect(0x0E, newE);
	setvect(0x0F, newF);

	_disable();		// запретить прерывания (cli)															

	outp(0x20, 0x11);	// ICW1
	outp(0x21, 0xA0);	// ICW2
	outp(0x21, 0x04);	// ICW3
	outp(0x21, 0x01);	// ICW4

	outp(0xA0, 0x11);	// ICW1
	outp(0xA1, 0x08);	// ICW2
	outp(0xA1, 0x02);	// ICW3
	outp(0xA1, 0x01);	// ICW4

	_enable();		// разрешить прерывания (sti)		
												
}

int main()
{
	unsigned far* fp;
	init();

	FP_SEG(fp) = _psp;			// получаем сегмент
	FP_OFF(fp) = 0x2c;			// смещение сегмента данных с переменными среды
	_dos_freemem(*fp);			// освобождение памяти

	_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);		// оставляем программу резидентной
	return 0;
}