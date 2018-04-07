/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static u8 au8ChannelTypes[] = "Channel types:";
static u16 u16MaxVoltage;
static u16 u16AverageVoltage;
static u8 au8ElectricityMessage[] = "Power:";
static u8 au8VoltageMessage[] = "V:";
static u8 u8DelayTime;
static u16 u16Result;

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/
static void Delay(u8 u8DelayTime)
{
  	for(u8 u8a = 0 ; u8a <u8DelayTime ; u8a++ )
	{	  	
	}
}

static void Voltage_Conversion(u16 u16Result)
{
  	u16AverageVoltage=3.3/4096*u16Result*100;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
	LedOff(WHITE);
	LedOff(PURPLE);
	LedOff(BLUE);
	LedOff(CYAN);
	LedOff(GREEN);
	LedOff(YELLOW);
	LedOff(ORANGE);
	LedOff(RED);
	LCDCommand(LCD_CLEAR_CMD);
	LCDMessage(LINE1_START_ADDR, au8ChannelTypes);
	LCDMessage(LINE2_START_ADDR, au8ElectricityMessage);
 
	/* Enable PIO */
	//PIOB
	AT91C_BASE_PIOB->PIO_PER |= PB_04_BLADE_AN1;           // Enable pio control of AN1 and AN0
	AT91C_BASE_PIOB->PIO_OER |= PB_04_BLADE_AN1;           // Enable pins output control
	
	//PIOA
	AT91C_BASE_PIOA->PIO_PER |= 0x0001F800;           // Enable pio control of SCK, MISO, MOSI, TX, RX
	AT91C_BASE_PIOA->PIO_OER |= 0x0001F800;           // Enable pins output control
	
	//ADC12B
	//AT91C_BASE_ADC12B->ADC12B_CHER |= 0x00000004;
	
	/* Initialize pins */
	AT91C_BASE_PIOA->PIO_CODR = PA_16_BLADE_CS;       // AD to low
	AT91C_BASE_PIOB->PIO_CODR = PB_03_BLADE_AN0;     // RE to low
	AT91C_BASE_PIOA->PIO_SODR = PA_13_BLADE_MISO;     // CS to high
	AT91C_BASE_PIOA->PIO_CODR = PA_14_BLADE_MOSI;     // UD to low
	AT91C_BASE_PIOA->PIO_CODR = PA_12_BLADE_UPOMI;    // INC to low
	AT91C_BASE_PIOA->PIO_CODR = PA_11_BLADE_UPIMO;    // 4053C to CX (A_OUT)
	AT91C_BASE_PIOB->PIO_SODR = PB_04_BLADE_AN1;      // 4053B to BY (GND)
	AT91C_BASE_PIOA->PIO_CODR = PA_15_BLADE_SCK;    // 4053A to AX (AUD2)
	
	/* Turn wiper destination to the lowest (Volume level 0)*/
	for(u8 i = 99; i; i--)
	{
		AT91C_BASE_PIOA->PIO_CODR = PA_13_BLADE_MISO;  // CS to low (Enable wiper change)
		AT91C_BASE_PIOA->PIO_CODR = PA_14_BLADE_MOSI;  // UD to low (Wiper down)
		Delay(2);
		
		/* INC change from high to low (change wiper once) */
		AT91C_BASE_PIOA->PIO_SODR = PA_12_BLADE_UPOMI; // INC to high
		Delay(2);
		AT91C_BASE_PIOA->PIO_CODR = PA_12_BLADE_UPOMI; // INC to low
		Delay(2);
		
		/* CS change from low to high (store wiper position)*/
		AT91C_BASE_PIOA->PIO_SODR = PA_12_BLADE_UPOMI; // INC to high
		Delay(2);
		AT91C_BASE_PIOA->PIO_SODR = PA_13_BLADE_MISO;  // CS to high
	}
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */

	    /*    |     +5V       |       5V         |                                    |
	          |     GND       |       GND        |                                    |
	          |     AN0       |       AD     	 | PB_03_BLADE_AN0    (u32)0x00000008 |
	          |     SCK       |       RE         | PA_15_BLADE_SCK    (u32)0x00008000 |
	          |     MISO      |       CS         | PA_13_BLADE_MISO   (u32)0x00002000 |
	          |     MOSI      |       UD         | PA_14_BLADE_MOSI   (u32)0x00004000 |
	          |     TX        |       INC        | PA_12_BLADE_UPOMI  (u32)0x00001000 |
	          |     RX        |       4053C      | PA_11_BLADE_UPIMO  (u32)0x00000800 |
	          |     AN1       |       4053B      | PB_04_BLADE_AN1    (u32)0x00000010 |
			  |     ICS       |       4053A      | PA_16_BLADE_CS     (u32)0x00010000 |
        */
				
static void UserApp1SM_Idle(void)
{
  	static bool bPhoneSignal = FALSE;
	static bool bMICSignal = FALSE;
	static bool bSoundOff = FALSE;
	static bool bLedRed = FALSE;
	static bool bVoltageTest = FALSE;
	static u16 u16LedTime = 0;
	static u8 u8Clock;
	static u16 u16Voltage;
	static u8 au8ChannelTypes2[] = "MIC";
	static u8 au8ChannelTypes3[] = "PHONE";
	static u8 au8ChannelTypes4[] = "OFF";
	static u8 au8ElectricityMessage2[2];
	static u8 u8Bits;
	static u8 u8Ten;
	static u8 u8Electricity;
	static u8 u8Button3PressTimes = 0;
	static float u16flaotVoltage;
	
 	if(WasButtonPressed(BUTTON3))
    {
        ButtonAcknowledge(BUTTON3);
		bLedRed = TRUE;
		u8Button3PressTimes ++;
		 		
		switch(u8Button3PressTimes)
		{
			case 1:
			{
				bPhoneSignal = TRUE;
				bMICSignal = FALSE;
				bSoundOff = FALSE;
				LCDClearChars(LINE1_START_ADDR + 15, 6);
				LCDMessage(LINE1_START_ADDR + 15, au8ChannelTypes3);
				break;
			}
			
			case 2:
			{
				bMICSignal = TRUE;
				bPhoneSignal = FALSE;
				bSoundOff = FALSE;
				LCDClearChars(LINE1_START_ADDR + 15, 6);
				LCDMessage(LINE1_START_ADDR + 15, au8ChannelTypes2);
				break;
			}
			
			case 3:
			{
				bSoundOff = TRUE;
				bPhoneSignal = FALSE;
				bMICSignal = FALSE;
				LCDClearChars(LINE1_START_ADDR + 15, 6);
				LCDMessage(LINE1_START_ADDR + 15, au8ChannelTypes4);
				u8Button3PressTimes = 0;
				break;
			}
		}
    }
	
	
				
	if(bPhoneSignal)
	{
	  	AT91C_BASE_PIOB->PIO_SODR = PA_16_BLADE_CS;
		AT91C_BASE_PIOB->PIO_CODR = PB_04_BLADE_AN1;
		AT91C_BASE_PIOA->PIO_CODR = PA_11_BLADE_UPIMO;
		AT91C_BASE_PIOA->PIO_CODR = PA_15_BLADE_SCK;
		LedOn(GREEN);
		LedOff(BLUE);
		LedOff(PURPLE);
	}
	
	if(bMICSignal)
	{
	  	AT91C_BASE_PIOB->PIO_CODR = PA_16_BLADE_CS;
		AT91C_BASE_PIOB->PIO_CODR = PB_04_BLADE_AN1;
		AT91C_BASE_PIOA->PIO_CODR = PA_11_BLADE_UPIMO;
		AT91C_BASE_PIOA->PIO_CODR = PA_15_BLADE_SCK;
		LedOff(GREEN);
		LedOn(BLUE);
		LedOff(PURPLE);
	}
	
	if(bSoundOff)
	{
	  	AT91C_BASE_PIOB->PIO_CODR = PA_16_BLADE_CS;
		AT91C_BASE_PIOB->PIO_SODR = PB_04_BLADE_AN1;
		AT91C_BASE_PIOA->PIO_CODR = PA_11_BLADE_UPIMO;
		AT91C_BASE_PIOA->PIO_CODR = PA_15_BLADE_SCK;
		LedOff(GREEN);
		LedOff(BLUE);
		LedOn(PURPLE);
	}
	
	if(WasButtonPressed(BUTTON0))
	{
	  	ButtonAcknowledge(BUTTON0);
		bLedRed = TRUE;
		AT91C_BASE_PIOA->PIO_CODR = PA_13_BLADE_MISO;  // CS to low 
		AT91C_BASE_PIOA->PIO_SODR = PA_14_BLADE_MOSI;  // UD to high 
		Delay(2);
		
		/* INC change from high to low  */
		AT91C_BASE_PIOA->PIO_SODR = PA_12_BLADE_UPOMI; // INC to high
		Delay(2);
		AT91C_BASE_PIOA->PIO_CODR = PA_12_BLADE_UPOMI; // INC to low
		Delay(2);
		
		/* CS change from low to high (store wiper position)*/
		AT91C_BASE_PIOA->PIO_SODR = PA_12_BLADE_UPOMI; // INC to high
		Delay(2);
		AT91C_BASE_PIOA->PIO_SODR = PA_13_BLADE_MISO;  // CS to high
	}
	
	if(WasButtonPressed(BUTTON1))
	{
	  	ButtonAcknowledge(BUTTON1);
		bLedRed = TRUE;
		AT91C_BASE_PIOA->PIO_CODR = PA_13_BLADE_MISO;  // CS to low 
		AT91C_BASE_PIOA->PIO_CODR = PA_14_BLADE_MOSI;  // UD to low 
		Delay(2);
		
		/* INC change from high to low  */
		AT91C_BASE_PIOA->PIO_SODR = PA_12_BLADE_UPOMI; // INC to high
		Delay(2);
		AT91C_BASE_PIOA->PIO_CODR = PA_12_BLADE_UPOMI; // INC to low
		Delay(2);
		
		/* CS change from low to high */
		AT91C_BASE_PIOA->PIO_SODR = PA_12_BLADE_UPOMI; // INC to high
		Delay(2);
		AT91C_BASE_PIOA->PIO_SODR = PA_13_BLADE_MISO;  // CS to high
	}
	
	if(WasButtonPressed(BUTTON2))
	{
	  	ButtonAcknowledge(BUTTON2);
		AT91C_BASE_PIOA->PIO_SODR = PA_15_BLADE_SCK;  
		Adc12AssignCallback(ADC12_CH2,Voltage_Conversion);
		
		Adc12StartConversion(ADC12_CH2);
		Delay(2);

		/*bLedRed = TRUE;
		bVoltageTest = TRUE;
		AT91C_BASE_PIOA->PIO_SODR = PA_15_BLADE_SCK; */ 
	}
	
	/*if(bVoltageTest)
	{
	    AT91C_BASE_ADC12B->ADC12B_CR = 0x00000002;  	
		u16flaotVoltage = AT91C_BASE_ADC12B->ADC12B_CDR[2];		
		u16Voltage = 3.3/4096*u16flaotVoltage*10;

		u8Bits = u16Voltage%10;
		u8Ten = u16Voltage/10;
		au8ElectricityMessage2[0] = u8Ten+48;
		au8ElectricityMessage2[1] = '.';
		au8ElectricityMessage2[2] = u8Bits+48;
		LCDMessage(LINE2_START_ADDR + 7, au8ElectricityMessage2);		
		bVoltageTest = FALSE;
	}*/
	
	if(bLedRed)
	{
	  	LedOn(RED);
		u16LedTime++;
		
		if(u16LedTime == 500)
		{
		  	LedOff(RED);
			u16LedTime = 0;
			bLedRed = FALSE;
		}
	}

} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
