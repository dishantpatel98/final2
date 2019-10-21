#define CR 0x0D
#include<lpc214x.h>
#include"LCD7TDMI.h"
#include <math.h>
#include<string.h>
void init_serial(void);
void transmit (int ch);
char  receive (void);
unsigned char test;
char *ptr1="Welcome\n\r";
//char *ptr2="Patel\n";	
void spi_data(unsigned char data);
char *id1="CDAC";
char *ps1="Acts123";
char id[100];
char ps[100];
void adc_init()
{
	PINSEL1|=(1<<28); //AD0.3 select
	AD0CR|=(1<<3)|(1<<16)|(1<<21); // Channel AD0.3 select, Burst enable, ADC operational/on
	AD0CR|=(0x0000FF00);	// CLKDIV = 0xFF
	//AD0INTEN|=(1<<3);
}

void send_string(unsigned char *ch)
{
		while(*ch!='\0')
		{
			send_data(*ch++);
		}
}

void delay_ms()
{
	for(unsigned int i=0; i<100; i++)
	for(unsigned int j=0; j<100; j++)
	{
	}
}






void delay_spi(unsigned long time)
{
 	int i, j;
	for(i=0;i<1000;i++)
		for(j=0;j<time;j++);
}

void SPI_init()
{
	
	PINSEL0|=(1<<8)/*SCK*/|(1<<10)/*MISO but not used*/|(1<<12)/*MOSI*/;
	IO0DIR|=(1<<7);//Since Shift Register A;redy working in slave mode therefore no need to SSEL , insted using that pin for controlling ST_CP
	IO0SET|=(1<<7);//ST_CP is storage register clock pulse
	S0SPCR=0x00;
	S0SPCR|=/*Master Mode*/(1<<5)/*LSB first*/|(1<<6);///*Interrupt enabled*/|(1<<7);
	S0SPCCR=0x0F;//As this input must be grater than 8 and a even number
	S0SPINT=(1<<0);
}

void SPI_data(unsigned char data)
{
	IO0SET|=(1<<7);//to shift the data from storage reg of IC
	S0SPDR=data;//data
	while(!(S0SPSR&(1<<7)));//chesecking status if data is ready and transfer is complete
	IO0CLR|=(1<<7);//to shift the data from storage reg of IC
}



void ext_int()
{
	//IO1DIR= (1<<25);
	//IO0DIR=(1<<31);  SET THE DIRECTION OF THE BUZZER is 25 and LED is 31
	VPBDIV = 0x02;  		//VPBDIV GIVES US FREQUENCY WE CAN DIVIDE IT BY CHANGES ITS VALUE
	PINSEL0 = (1<<31);		//SELECT EXTERNAL INTERRUPT PIN
	EXTMODE = (1<<2);				//FOR LEVEL  TRIGGER OR EDGE  TRIGGER OF PARTICULER EINT 2 
	EXTPOLAR = (1<<2);			//CHANGE  ITS POLAIRTY WEATHER RISING EDGE OR FALLING EDGE (INTIALLY IT HAS FALLING EDGE )
	EXTINT = (1<<2);			
}

void External_int(void)__irq          //INTERRUPT SERVICE ROUTINE 
	{
		
unsigned int decval,i,data;
		float val; 
		unsigned char volt[16], dig[16];
		init_lcd();
		adc_init();
		while(1){
		send_cmd(0x01);
		send_cmd(0x80);
		while(!(AD0GDR & (1<<31)));
		data=AD0GDR;
		decval=(data>>6)&(0x3FF);
		sprintf(dig, "ADC Value=%d", decval);
		send_string(dig);
		val=decval*(3.3/1024);
		sprintf(volt, "Voltage=%.3fV", val);
		send_cmd(0xC0);
		send_string(volt);
		delay_ms();
	}	



		
		
		
		
		/*SPI_init();
	while(1)
	{
	SPI_data(0x55);
	delay_spi(10000);
	SPI_data(0xAA);
	delay_spi(10000);
	}
	*/
		
		
		/*	while(*ptr1){
transmit(*ptr1);
	ptr1++;
}

while(*ptr2){
transmit(*ptr2);
	ptr2++;
}
*/

/*
	static int i=0;
	
	if(i==0)
	{
		i=1;
		IO1CLR=(1<<25);
		IO0CLR=(1<<31);
		
	}
	else
	{
		i=0;
		IO1SET=(1<<25);
		IO0SET=(1<<31);
	}*/
	
	/*int i=0;
	char A[]="JAI JAI  GUJARAT";
	init_lcd();
	
	send_cmd(0x80);
	//send_cmd(0xc0);
	for (i=0;i<15;i++)
	{
		send_data(A[i]);
	}
	//send_cmd(0xc0);
	for (i=15;i<31;i++)
	{	
		if (A[i]=='\0')
		{
			break;
		}
		else 
			{
				send_data(A[i]);
			}
	}
	*/
	EXTINT |= (1<<2);
	VICVectAddr = 0x00;

		
		}

void int_init()
{
	VICIntEnable = (1<<16);      //ENABLE PARTICULAR SLOT OF INTERRUPT  	
	VICVectCntl0 = 0x20 | 0x10;			//ENABLE INTERRPUT WHERE FIRST FIVE BITS ARE USE FOR 0-32 INTERRUPT WHERE AS 5 BIT IS SET MEANS PARTICULAR TO INTERRUPT IS ACTIVATED   
	VICVectAddr0 = (unsigned)External_int;     // ASSIGNE THE ADDRESS OF THE FUNCTION TO THE PARTIULAR VICvectaddress
}

int main(void)
	{	 
				VPBDIV=0x02;
				init_serial();
		    //ext_int();
	      //int_init();
		init_lcd();
		send_cmd(0x01);
  send_string("wELCOME");
	delay_ms();
delay_ms();
delay_ms();
send_cmd(0x01);	
send_string("Enter Id:");
delay_ms();

int j=0;
send_cmd(0x01);	

		while(1){
char i= receive();
	transmit(i);
send_data(i);
		delay_ms();
delay_ms();
delay_ms();
	if(i!=CR){
	id[j++]=i;
	}
	else{
		send_cmd(0x01);
	break;
	}
	
}












int k=0;



while(1){
char i= receive();
send_data(i);

	if(i!=CR){
	ps[k++]=i;
	}
	else{
	
		send_cmd(0x01);
	  send_string("Entered");
		break;
	}
	
}



if(!strcmp(id,id1)){
if(!strcmp(ps,ps1)){
send_string("Sucessfull");
}
else{
send_string("WrongPassword");

}

}

else{
send_string("Wrong id");
}







		
		
		//   while(1);
		
		/*while(*ptr1){
transmit(*ptr1);
	ptr1++;
}*/

/*while(*ptr2){
transmit(*ptr2);
	ptr2++;
}*/





		/*	while(1)
						{ 
						
								
							//		transmit(getchar());
										x=receive();
									transmit(x);
								 
						}  */
	}

void init_serial(void)	
			{
					PINSEL0|=(1<<16)|(1<<18);	
					U1LCR=0x00000083;                            //(1<<7)|(1<<0)|(1<<1);
					U1DLL = 183;                                
					U1LCR=0x00000003;                                    //(~(1<<7))|(1<<0)|(1<<1);	
					U1FDR=0xF1;                                   //(1<<0);															
				  U1TER|=0x80;                                   //(1<<7);
		
			}


void transmit( int ch )
{
		//if (ch=='\n')
			//{	
				//while(!(U1LSR&(1<<5)));
			//	U1THR= CR;				//enter ASCII value
		//	}
				
					while(!(U1LSR&(1<<5)));
					 U1THR=ch; //data
				
			
}

char receive ()
{ unsigned char ch;
	while( !(U1LSR&(1<<0)));
	ch = U1RBR;
	return ch;
}