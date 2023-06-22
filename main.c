/*
===============================================================================
 Name        : final.c
 Author      : Troy Miller, Anthony Ngyuen
 Description : Digital Design Lab - Final Project
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#define PCONP (*(volatile unsigned int *)0x400fc0c4)

#define AD0CR (*(volatile unsigned int *)0x40034000)
#define AD0GDR (*(volatile unsigned int *)0x40034004)
#define AD0INTEN (*(volatile unsigned int *)0x4003400c)
#define AD0DR0 (*(volatile unsigned int *)0x40034010)

#define I2STXRATE (*(volatile unsigned int *)0x400a8020)
#define I2SDAO (*(volatile unsigned int *)0x400a8000)
#define I2STXMODE (*(volatile unsigned int *)0x400a8030)
#define I2STXFIFO (*(volatile unsigned int *)0x400a8008)
#define I2SSTATE (*(volatile unsigned int *)0x400a8010)
#define I2SIRQ (*(volatile unsigned int *)0x400a801C)

#define PLL0CON (*(volatile unsigned int *)0x400fc080)
#define PLL0CFG (*(volatile unsigned int *)0x400fc084)
#define PLL0STAT (*(volatile unsigned int *)0x400fc088)
#define PLL0FEED (*(volatile unsigned int *)0x400fc08c)
#define CCLKCFG (*(volatile unsigned int *)0x400fc104)
#define CLKSRCSEL (*(volatile unsigned int *)0x400fc10c)
#define CLKOUTCFG (*(volatile unsigned int *)0x400fc1c8)
#define PCLKSEL0 (*(volatile unsigned int *)0x400fc1a8)
#define PCLKSEL1 (*(volatile unsigned int *)0x400fc1ac)


#define FLASHCFG (*(volatile unsigned int *)0x400FC000)

#define EXTINT (*(volatile unsigned int *)0x400FC140)
#define EXTMODE (*(volatile unsigned int *)0x400FC148)
#define IO0IntStatR (*(volatile unsigned int *)0x40028084)
#define IO0IntStatF (*(volatile unsigned int *)0x40028088)
#define IO0IntEnR (*(volatile unsigned int *)0x40028090)
#define IO0IntEnF (*(volatile unsigned int *)0x40028094)
#define IO0IntClr (*(volatile unsigned int *)0x4002808C)
#define IOIntStatus (*(volatile unsigned int *)0x40028080)
#define FIO0DIR (*(volatile unsigned int *)0x2009c000)
#define FIO0PIN (*(volatile unsigned int *)0x2009c014)
#define FIO0SET	(*(volatile unsigned int *)0x2009c018)
#define FIO0CLR (*(volatile unsigned int *)0x2009c01c)
#define FIO1DIR (*(volatile unsigned int *)0x2009c020)
#define FIO2DIR (*(volatile unsigned int *)0x2009c040)
#define FIO1PIN (*(volatile unsigned int *)0x2009c034)
#define FIO2PIN (*(volatile unsigned int *)0x2009c054)

#define PINSEL0 (*(volatile unsigned int *)0x4002c000)
#define PINSEL1 (*(volatile unsigned int *)0x4002c004)
#define PINSEL3 (*(volatile unsigned int *)0x4002c00c)
#define PINSEL4 (*(volatile unsigned int *)0x4002c010)


#define ISER0 (*(volatile unsigned int *)0xE000E100)
#define ICPR0 (*(volatile unsigned int *)0xE000E280)

#define T0IR (*(volatile unsigned int *)0x40004000)
#define T1IR (*(volatile unsigned int *)0x40008000)
#define T2IR (*(volatile unsigned int *)0x40090000)
#define T0MCR (*(volatile unsigned int *)0x40004014)
#define T1MCR (*(volatile unsigned int *)0x40008014)
#define T2MCR (*(volatile unsigned int *)0x40090014)
#define T0MR0 (*(volatile unsigned int *)0x40004018)
#define T0MR1 (*(volatile unsigned int *)0x4000401C)
#define T0MR2 (*(volatile unsigned int *)0x40004020)
#define T0MR3 (*(volatile unsigned int *)0x40004024)
#define T1MR0 (*(volatile unsigned int *)0x40008018)
#define T1MR1 (*(volatile unsigned int *)0x4000801C)
#define T1MR2 (*(volatile unsigned int *)0x40008020)
#define T1MR3 (*(volatile unsigned int *)0x40008024)
#define T2MR0 (*(volatile unsigned int *)0x40090018)
#define T2MR1 (*(volatile unsigned int *)0x4009001C)
#define T2MR2 (*(volatile unsigned int *)0x40090020)
#define T2MR3 (*(volatile unsigned int *)0x40090024)
#define T0TC (*(volatile unsigned int *)0x40004008)
#define T1TC (*(volatile unsigned int *)0x40008008)
#define T2TC (*(volatile unsigned int *)0x40090008)
#define T0TCR (*(volatile unsigned int *)0x40004004)
#define T1TCR (*(volatile unsigned int *)0x40008004)
#define T2TCR (*(volatile unsigned int *)0x40090004)
#define T2CTCR (*(volatile unsigned int *)0x40090070)
#define T2CCR (*(volatile unsigned int *)0x40090028)
#define T2CR0 (*(volatile unsigned int *)0x4009002c)

#define PWM1IR (*(volatile unsigned int *)0x40018000)
#define PWM1MCR (*(volatile unsigned int *)0x40018014)
#define PWM1MR0 (*(volatile unsigned int *)0x40018018)
#define PWM1MR1 (*(volatile unsigned int *)0x4001801C)
#define PWM1MR2 (*(volatile unsigned int *)0x40018020)
#define PWM1MR3 (*(volatile unsigned int *)0x40018024)
#define PWM1MR4 (*(volatile unsigned int *)0x40018040)
#define PWM1MR5 (*(volatile unsigned int *)0x40018044)
#define PWM1MR6 (*(volatile unsigned int *)0x40018048)
#define PWM1PCR (*(volatile unsigned int *)0x4001804C)
#define PWM1TCR (*(volatile unsigned int *)0x40018004)
#define PWM1CCR (*(volatile unsigned int *)0x40018028)
#define PWM1LER (*(volatile unsigned int *)0x40018050)

#define IPR0 (*(volatile unsigned int *)0xE000E400)
#define IPR2 (*(volatile unsigned int *)0xE000E408)
#define IPR5 (*(volatile unsigned int *)0xE000E414)
#define IPR6 (*(volatile unsigned int *)0xE000E418)

//Global variables
const int width = 8;
int map[240][8];//8 is width, compiler hates me putting a const in this declaration for some reason
int word = 0;//Current word
int line = 0;//current line
int counter = 0;
int block = 0;
int sample = 500;

double out;
int data_size = 175;
unsigned int data[175];
//volatile unsigned int linkedList[4];
int updog = 0;

double height_pos = 0;
int height_pos_int = 0;

float dc_voltage = 0;
float ac_voltage = 0;

int GPIO_flag = 0;
int GPIO_count = 0;

int test = 0;
int hz = 1000;
//int counter = 0;

void setupADC(){


    // turns on subsystem
	PCONP |= (1<<12);

	AD0CR &= (1<<24);

	AD0CR |= (1<<21);

	// set up P0.23 (pin 15) for AD0

	PINSEL1 &= ~(1<<15);
	PINSEL1 |= (1<<14);

    // AD0.0 activated
    AD0CR |= (1<<0);
    //AD0CR |= (1<<1);//Ad0.1

    // ENABLE burst mode
    AD0CR |= (1<<16);

    //Pclcock divider, keep measure frequency just belwo 13 MHz
//    AD0CR |= (1<<14);
//    AD0CR |= (1<<13);
//    AD0CR &= ~(1<<12);
//    AD0CR &= ~(1<<11);
//    AD0CR &= ~(1<<10);
//    AD0CR &= ~(1<<9);
//    AD0CR &= ~(1<<8);

    AD0CR |= (1<<8);

    //ISER0 |= (1<<22);//Enable interrupts
    AD0INTEN |= (1<<0);//Enable interrupt channel 0
    //IPR5 |= (0b111 << 19);
}

void startADC(){
    // start conversion
    AD0CR &= ~(1<<25);
    AD0CR &= ~(1<<26);

	AD0CR |= (1<<24);

}

int readADC(){

    int output;

	// wait for AD0 DONE bit to return to 1
	while (!((AD0GDR >> 31) & 1)){
		// just waiting
	}

	// extract [15:4]
	//output = AD0GDR << 16;
	//output = output >> 20;
	output = AD0GDR >> 4;
	output = output & 0x00000FFF;


	// stop output
	AD0CR &= ~(1<<24);

	return output;
}

void stopADC(){
    // turns off subsystem
	AD0CR &= ~(1<<21);
	PCONP &= ~(1<<12);

}

//Functions for modular design
void wait_ticks(int ticks){
	volatile int count;
	for (count=0; count<ticks; count++){
		//nothing
	}
}




//Function to set Clock
void PLL0OneFeedSequence(void)
{
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;
}
void updateClock(void)
{
	//FLASHTIM
	FLASHCFG &= ~(1 << 12);
	FLASHCFG &= ~(1 << 13);
	FLASHCFG |= (1 << 14);
	FLASHCFG &= ~(1 << 15);

	PLL0CON &= ~(0x00000002);//Disconnect PLL0
	PLL0OneFeedSequence();

	PLL0CON &= ~(0x00000001);//PLL0 Disable
	PLL0OneFeedSequence();

	CCLKCFG = 0; //Change CPU Clock Divider setting [40]

	CLKSRCSEL = 0x00; //Write to clock source selection

	//PLL0CFG = 0x0001006A;//Multiplication and Pre-Divide
	PLL0CFG = 0x00000031;
	//m = 49+1 n = 0+1 //M = 100+1 n = 1+1
	PLL0OneFeedSequence();

	PLL0CON |= 0x00000001;//PLL0 Enable
	PLL0OneFeedSequence();

	CCLKCFG = 0x00000003;
	//Post Divide = 3 + 1

	int i = 0;
    //while((PLL0STAT & (PLL0STAT << 26)) != (PLL0STAT << 26)){
	while(!(1 & (PLL0STAT >> 26))){
        i++;
	}//Wait for PLOCK0 to achieve lock

	PLL0CON |= 0x00000002;//Connect PLL0 with one feed sequence
	PLL0OneFeedSequence();

}

//Function to configure the I2S
void initializeI2S(){
	PCONP |= (1<<27);
	PCLKSEL1 |= (1 << 23); //Select pclk 25 MHZ

	I2SDAO |= (1<<4);
	I2SDAO &= ~(1<<4);

	//Set clock divide Clk = Pclck (x/y)
	I2STXRATE = 0x0102;// 01 = x ; 02 = y // 12.5MHZ out
	//Default bitrate is fine
	//We want typical transmitter master mode
	I2SDAO |= (1 << 3); //stop
	I2SDAO |= (1 << 0); //32 bit mode
	I2SDAO |= (1 << 1);
	I2SDAO &= ~(1 << 5); //ws_sel to zero, enables master mode
	I2STXMODE |= (0b0000); //tx rate divider clock (x/y Pclck) : regular pin mode : DO not enable TX_MCLK

	PINSEL0 |= (1<<18);
	PINSEL0 &= ~(1<<19);

	I2SDAO &= ~(1<<3);//Resume TX
	I2SDAO &= ~(1<<15);

}

int character(char x){
	switch(x){
		case 'A':
			return 0b0010001010100011111110001;
		case 'B':
			return 0b1110010010111001001011100;
		case 'C':
			return 0b0111010000100001000001110;
		case 'D':
			return 0b1110010010100101001011100;
		case 'E':
			return 0b1110010000110001000011100;
		case 'F':
			return 0b1110010000111001000010000;
		case 'G':
			return 0b0111010000101101001001110;
		case 'H':
			return 0b1001010010111101001010010;
		case 'I':
			return 0b0111000100001000010001110;
		case 'J':
			return 0b0001000010000100001001110;
		case 'K':
			return 0b1001010100110001010010010;
		case 'L':
			return 0b0100001000010000100001110;
		case 'M':
			return 0b1000111011101011000110001;
		case 'N':
			return 0b1000111001101011001110001;
		case 'O':
			return 0b0111010001100011000101110;
		case 'P':
			return 0b1110010010111001000010000;
		case 'Q':
			return 0b0110010010100101011001111;
		case 'R':
			return 0b1110010010111101010010010;
		case 'S':
			return 0b0111010000011000001011100;
		case 'T':
			return 0b1111100100001000010000100;
		case 'U':
			return 0b1000110001100011000101110;
		case 'V':
			return 0b1000110001010100101000100;
		case 'W':
			return 0b1010110101101011010101010;
		case 'X':
			return 0b1000101010001000101010001;
		case 'Y':
			return 0b1000101010001000010000100;
		case 'Z':
			return 0b1111100010001000100011111;
		case '0':
		case '\0':
			return 0b0110010010100101001001100;
		case '1':
		case '\001':
			return 0b0010001100001000010001110;
		case '2':
		case '\002':
			return 0b1110000010001000100011110;
		case '3':
		case '\003':
			return 0b1110000010111000001011100;
		case '4':
		case '\004':
			return 0b1010010100111100010000100;
		case '5':
		case '\005':
			return 0b1111010000111000001011100;
		case '6':
		case '\006':
			return 0b0110010000111001001001100;
		case '7':
		case '\007':
			return 0b1111000010001000100001000;
		case '8':
		case '\b':
			return 0b0110010010011001001001100;
		case '9':
		case '\t':
			return 0b1111010010111100001011110;
		case '.':
			return 0b0000000000000000000000100;
		case ':':
			return 0b0000000100000000010000000;
	}
	return 0;
}


void initializeGPIO(){
	//Needed to capture an edge
	//FIO2DIR &= ~(1<<13);//input
	//PINSEL4 &= ~(1<<27);
	//PINSEL4 |= (1<<26);//01 FOR EINT3

	FIO0DIR &= ~(1<<4);//INPUT

	IO0IntClr = (1<<4);//Clear interrupt pending
	IO0IntEnR |= (1<<4);//Enable interrupt on rising edge
	//IO0IntEnF |= (1<<4);//Interrupt on falling edge
	//EXTMODE = (0b1000);
	//EXTINT = (0b1000);//Clear interrupt register
	ISER0 = (1<<21);//interrupt set enable
	IPR5 |= (0b11111 << 11);
	//Once one interrupt hits, they never ever clear fully
}

void initializeTMR(){//Old busted
	//Timer is for ADC measurement interrupts
	//PCONP reset value for Timer 0 AND 1 is default 1
	//PCONP |= (1<<22);//Timer 2 on
	PCLKSEL0 |= (1 << 2); //Clock speed = Processor speed for granular control
	PCLKSEL0 |= (1<<4); //TIMER 1
	//PCLKSEL0 |= (1<<12); //TIMER 2

	T1TCR = 0b11;//Enable reset
	T1TCR = 0b01;//Disable reset


	//Timer 2, a counter for finding the frequency
	//T2CTCR = 0b0010; //Enable counter mode
	//PINSEL0 |= (1<<8);
	//PINSEL0 |= (1<<9);//CAP 2.0
	//Is also capture control register, which is interesting

	//T2CCR |= (1<<0);


	//PINSEL
	//PINMODE
	T0IR &= ~(0b111111);//Yeet old interrupt flags
	T0TCR = 0b11;//Enable counter and reset
	T0TCR = 0b01;////Disable reset, keep counter
	T0MR0 = sample;//interrupt every x clock cycle
	T0MCR = 0b000000000011;//Match control register, enable interrupt and reset on match
	ISER0 |= (1<<1);//Enable interrupts
	//T0IR is the interrupt flag register
	IPR0 |= (1 << 13); //Drop priority for VGA

}

void initializePWM(){//The new hotness
	//yeet old interrupt flags
	PWM1IR = 0b11100111111;

	//enable interrupts
	ISER0 = (1<<9);

	//Enable match interrupts
	PWM1MCR |= (1<<0);
	PWM1MCR |= (1<<3);
	PWM1MCR |= (1<<6);
	PWM1MCR |= (1<<9);
	PWM1MCR |= (1<<15);
	PWM1MCR |= (1<<18);

	//Set match registers

	//MR0 always sets PWM1 and PWM4 high, //MR1 sets PWM1 low
	//MR0 must go last, requiring re-arrangement
	PWM1MR0 = 800;//back porch
	PWM1MR1 = 704;//front porch
	PWM1MR2 = 688;//blanking
	PWM1MR3 = 48;//video on
	PWM1MR4 = 704; //For the Vsync PWM
	PWM1MR5 = 615; //For jank I2S register loading
	PWM1MR6 = 600; //For jank I2S reset.
	//Prescale register defaults to zero, just leave alone
	//TC increments every PR+1 cycles of PCLK/4

	//Make sure the reset is enabled
	PWM1MCR |= (1<<1); //PWMMR0R

	//Make sure timers are enabled
	PWM1TCR |= (1<<0);
	PWM1TCR |= (1<<3);

	//Enable PWM output
	PWM1PCR |= (1<<9);
	PWM1PCR |= (1<<12);

	//Pinselect
	PINSEL4 |= (1<<0);
	PINSEL4 &= ~(1<<1);
	PINSEL4 |= (1<<6);
	PINSEL4 &= ~(1<<7);

	PWM1LER |= 0b1111111; //Latch Enable - Required to update all match registers

	IPR2 &= ~(0b11111 << 11);//BIG priority interrupts
}

//Function to Write to the buffer
void writeVGA(int x){
	I2STXFIFO = x;
}


void eraseColumn(int x){
	int x_pos = 31 - (x % 32);
	int block = x / 32;

	for(int i = 10; i <= 175; i++){//Write line by line, overwriting what was there
		map[i][block] &= ~(1 << x_pos); //clear that entire column
	}

}

void writePosition(int x, int y){

	int x_pos = 31 - (x % 32);
	int block = x / 32;

	eraseColumn(x);

	map[y][block] |= (1 << x_pos);
	map[y+1][block] |= (1 << x_pos);
}


void writeChar(int x, int block, int y, char z){//DO NOT USE X > 27, OR YOUR OUTPUT WILL BE SAD
	//Assume that this will write to a grid shape! The y can be arbitrary, but the X needs to align to preset areas.
	//Letters are 5 wide, we can fit 6 in one block. DO NOT OVERFLOW OUT OF THE REG. Watch division lines.
	int temp = character(z);
	x = x % 32;
	//Break the character into chunks, bottom line is on the right
	//The time I would waste learning how to make a struct could be better spent finish the project
	int line0 = (0b11111 & (temp >> 0));
	int line1 = (0b11111 & (temp >> 5));
	int line2 = (0b11111 & (temp >> 10));
	int line3 = (0b11111 & (temp >> 15));
	int line4 = (0b11111 & (temp >> 20));
	for(int i = 0; i < 5; i++){//Write line by line, overwriting what was there
		map[y + i][block] &= ~(0b11111 << x);//Clear those bits
	}
	map[y + 0][block] |= (line4 << x);
	map[y + 1][block] |= (line3 << x);
	map[y + 2][block] |= (line2 << x);
	map[y + 3][block] |= (line1 << x);
	map[y + 4][block] |= (line0 << x);
	//There is a more efficeint and less legible way to write this. Oh well.
}

void writeNames(){ //My preferred programming language is duct tape
	writeChar(24, 0, 234, 'T');
	writeChar(18, 0, 234, 'R');
	writeChar(12, 0, 234, 'O');
	writeChar(6, 0, 234, 'Y');

	writeChar(26, 1, 234, 'M');
	writeChar(21, 1, 234, 'I');
	writeChar(16, 1, 234, 'L');
	writeChar(11, 1, 234, 'L');
	writeChar(6, 1, 234, 'E');
	writeChar(1, 1, 234, 'R');

	writeChar(24, 3, 234, 'A');
	writeChar(18, 3, 234, 'N');
	writeChar(12, 3, 234, 'T');
	writeChar(6, 3, 234, 'H');
	writeChar(0, 3, 234, 'O');

	writeChar(26, 4, 234, 'N');
	writeChar(20, 4, 234, 'Y');
	writeChar(12, 4, 234, 'N');
	writeChar(6, 4, 234, 'G');
	writeChar(0, 4, 234, 'U');

	writeChar(26, 5, 234, 'Y');
	writeChar(20, 5, 234, 'E');
	writeChar(16, 5, 234, 'N');
}

void makeBorders(){
	//Left and right walls
	for (int i = 0; i < 240; i++){
		map[i][0] |= 0xC0000000;

		if (i < 176){
			map[i][5] |= 0x00000001;
		}

		map[i][width - 1] |= 0x00000C00;
	}


	//Top and bottom walls
	for (int i = 0; i < width; i++){
		map[0][i] = 0xFFFFFFFF;

		if (i < 6){
			map[176][i] = 0xFFFFFFFF;
		}

		map[239][i] = 0xFFFFFFFF;
	}
	//Viewport bottom line

}

void initializeScreen(){
	writeNames();
	makeBorders();


	writeChar(24, 3, 180, 'D');
	writeChar(18, 3, 180, 'C');
	writeChar(12, 3, 180, ':');
	writeChar(0, 3, 180, '.');
	writeChar(12, 4, 180, 'V');

	writeChar(24, 3, 200, 'A');
	writeChar(18, 3, 200, 'C');
	writeChar(12, 3, 200, ':');
	writeChar(0, 3, 200, '.');
	writeChar(12, 4, 200, 'V');


	writeChar(24, 6, 15, '3');
	writeChar(18, 6, 15, '.');
	writeChar(12, 6, 15, '3');
	writeChar(6, 6, 15, 'V');

	writeChar(18, 6, 160, '0');
	writeChar(12, 6, 160, 'V');

	writeChar(18, 0, 180, 'F');
	writeChar(12, 0, 180, ':');
	writeChar(8, 1, 180, 'H');
	writeChar(2, 1, 180, 'Z');

}


//Function to create checkerboard to test the funny bitmap
void VgaCheckerboard(){
	//turn the map into a checkerboard for testing purposes.
	//int builder = 0;
	//int test = 0;
//	while (builder < 240){
//		for (int i2 = 0; i2 < 32; i2++){
//			for (int i = 0; i < width; i++){
//				map[builder][i] = test;
//				test = ~test;
//			}
//			builder++;
//			if (builder == 240){break;}
//		}
//		test = ~test;
//	}

	//Trying something. I think the checkerboard messes things up, going to put a solid border on everything

	for (int i = 0; i < 240; i++){
		map[i][0] |= 0xC0000000;
		map[i][width - 1] |= 0x00000003;
	}
	for (int i = 0; i < width; i++){
		map[0][i] = 0xFFFFFFFF;
		map[239][i] = 0xFFFFFFFF;
	}
}

// at every X time interval, grab a new time measurement
// then, increment the index variable (named "updog")
void TIMER0_IRQHandler(void){
	//volatile int countIntVal = T0TC;
	//startADC();
	T0IR = (1<<0);//Clear interrupt
	//data[updog] = readADC();
	data[updog] = (0xFFF & (AD0DR0 >> 4));
	updog++;
	//updog = updog % 128;
	if(updog >= data_size){
		T0MCR = 0;
	}

	//volatile int countIntVal2 = T0TC;


}


void PWM1_IRQHandler(void){
	//When a match register hits the timer interrupt, figure out which matches and do the thing
	//Check match register
	if (PWM1IR & (1<<9)){//Jank I2S load //PWMMR5
		//WHY DOES THIS WORK?
		if((line >= 16) && (line < 496)){//manual 16 line offset
			I2SDAO &= ~(1<<4);//REset off
			for(int i = 0; i < width; i++){
				//writeVGA(0xAAAAAAAA);
				writeVGA(map[(line-16)>>1][i]);
			}
		}
		PWM1IR = (1<<9);
	}
	if (PWM1IR & (1<<0)){//Back porch begins
		PWM1IR = (1<<0);
		line++;
		line  = line % 525;

	}
	if (PWM1IR & (1<<1)){//front porch
		//Empty interrupt
		PWM1IR = (1<<1);
	}
	if (PWM1IR & (1<<10)){//Jank I2S reset //PWMMR6
		//The location of this interrupt in this interrupt if pile changes where the reset happens
		//Just like, move it until all 8 registers are visible. IT's jank, just roll with it.
		I2SDAO |= (1<<4);
		PWM1IR = (1<<10);
	}
	if (PWM1IR & (1<<2)){//Blanking begins
		//Empty interrupt
		PWM1IR = (1<<2);
	}

	//Video on has the most time between interrupts, try to schedule the most overhead here
	if (PWM1IR & (1<<3)){//video on
		PWM1IR = (1<<3);
		//For controlling the VSYNC PWM we must set the register one line early
		//Luckily, each step is at least one H-line long
		//It may have a lot of overhead, so it's logical to schedule it here
		//1: Schedule Front Porch
		if (line == 9){
			PWM1MR4 = 704; //Sync up with H-sync low
			PWM1LER = (1<<4);//704 will set on next TC reset
		}
		//2: Hold low for pulse
		if (line == 10){
			PWM1MR4 = 0;//Should never go high to begin with
			PWM1LER = (1<<4);
		}
		//3: Schedule back porch
		if (line == 12){
			PWM1MR4 = 800;//should stay high now
			PWM1LER = (1<<4);
		}

	}
}

void EINT3_IRQHandler(void){
	//GPIO interrupt handler
	GPIO_flag = 1;
	GPIO_count++;
	IO0IntClr = (1<<4);//Clear interrupt pending
	//EXTINT = (0b1000);//Clear other interrupt register

}

// Quake inverse square root
float Q_rsqrt( float number ) {
  long i;
  float x2, y;
  const float threehalfs = 1.5F;

  x2 = number * 0.5F;
  y  = number;
  i  = * ( long * ) &y;                       // evil floating point bit level hacking
  i  = 0x5f3759df - ( i >> 1 );               // what is this?
  y  = * ( float * ) &i;
  y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
  //      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
    return y;
}


int getVoltageButton(){
 return (FIO0PIN >> 24) & 1;
}

int voltageButtonState = 0;
int newVoltageButtonState;


int main(void) {

	// for voltage button
	double vScale[7] = {0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5};
	int voltageCounter = 6;

	FIO0DIR &= ~(1 << 24);


	//Set clock 100MHz
	updateClock();


	initializeI2S();
	initializePWM();
	initializeGPIO();

	setupADC();
	initializeTMR();
	initializeScreen();


	double data_temp;

	char dc_digit;
	char dc_tenths;
	char dc_hundredths;

	char ac_digit;
	char ac_tenths;
	char ac_hundredths;

	char onesVoltage;
	char tenthsVoltage;

    volatile static int i = 0 ;
    while(1) {
		
		// if voltage button pressed, changed scale
    	newVoltageButtonState = getVoltageButton();
    	if ((voltageButtonState != newVoltageButtonState) && (newVoltageButtonState == 1)){
    		voltageCounter++;
    	}


		// digit calculations for voltage scale
    	onesVoltage = (char)  ((int) vScale[voltageCounter % 7] % 10);
    	tenthsVoltage = (char)  ((int) (vScale[voltageCounter % 7] * 10) % 10);

		
		// write the voltage scale on screen
    	writeChar(24, 6, 15, onesVoltage);
    	writeChar(12, 6, 15, tenthsVoltage);


        i++ ;

        dc_voltage = 0;
        ac_voltage = 0;

        //Find frequency


        while(updog < data_size - 1){
        	//aaaaa
        }
		int x = GPIO_count;//Number of falling edges
		int y = T1TC;//Clock cycles elapsed at 100MHZ
    	T1TCR = 0b11;//Enable reset
        GPIO_count = 0;
    	T1TCR = 0b01;//Disable reset
    	//Start T1 counting now


    	// between 10 and 160 y

        //Write Graph
        for (int x_position = 10; x_position < data_size + 10; x_position++){

            data_temp = (( (float) data[x_position - 10]) / 0xfff ) * 3.3;

            // voltage scaling
            height_pos = ((150 * data_temp) / (vScale[voltageCounter % 7]));
            height_pos_int = 150 - (int) height_pos;

			// if it'd be outside the screen, just don't write it 
            if (!((height_pos_int + 10) < 10 || ((height_pos_int + 10) > 160))) {
            	writePosition(x_position, height_pos_int + 10);
            }
            else{
            	eraseColumn(x_position);
            }


            dc_voltage += data_temp;
            ac_voltage += (data_temp * data_temp);
        }


        dc_voltage /= data_size;

        // what the fast functional square root
        ac_voltage = 1.0 / (Q_rsqrt(ac_voltage / data_size));

		// calculating digits
        dc_digit = (char) ((int) dc_voltage % 10);
        dc_tenths = (char) ((int) (dc_voltage * 10) % 10);
        dc_hundredths = (char) ((int) (dc_voltage * 100) % 10);

        ac_digit = (char) ((int) ac_voltage % 10);
        ac_tenths = (char) ((int) (ac_voltage * 10) % 10);
        ac_hundredths = (char) ((int) (ac_voltage * 100) % 10);

		// write dc/ac values to screen
		writeChar(6, 3, 180, dc_digit);
		writeChar(24, 4, 180, dc_tenths);
		writeChar(18, 4, 180, dc_hundredths);

		writeChar(6, 3, 200, ac_digit);
		writeChar(24, 4, 200, ac_tenths);
		writeChar(18, 4, 200, ac_hundredths);


		// so dc/ac values don't look weird
        wait_ticks(10);
        counter++;
        if(counter >= 10){
			hz = ( (float)x / ( (float)y / 100000000.0));//Frequency

			char digit1 = (char) (hz % 10);
			char digit2 = (char) ((hz/10) % 10);
			char digit3 = (char) ((hz/100) % 10);
			char digit4 = (char) ((hz/1000) % 10);
			char digit5 = (char) ((hz/10000) % 10);

			writeChar(6, 0, 180, digit5);
			writeChar(0, 0, 180, digit4);
			writeChar(26, 1, 180, digit3);
			writeChar(20, 1, 180, digit2);
			writeChar(14, 1, 180, digit1);

			//Change freq here, automagically
			sample = 100000;
			if (hz > 10){
				sample = 3500;
			}
			if (hz > 100){
				sample = 2500;
			}
			if (hz > 500){
				sample = 500;//This is the max for A2D
			}
			//if (hz > 5000){
			//	sample = 250;
			//}

			counter = 0;
        }

    	int blah = 0;
        GPIO_flag = 0;
		T0TCR = 0b11;//Enable reset
		T0MR0 = sample;//Change freq when needed
        if(hz > 100){
			while(!GPIO_flag){//Wait for high
				blah++;
				if (blah > 10000){
					break;//Flat lines have no sync lol
				}
			}
        }
		T0MCR = 0b000000000011;//Match control register, enable interrupt and reset on match
		updog = 0;
		T0TCR = 0b01;////Disable reset


        voltageButtonState = newVoltageButtonState;


    }
    return 0 ;
}



