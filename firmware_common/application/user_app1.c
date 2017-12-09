/**********************************************************************************************************************
File: user_app.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app as a template:
 1. Copy both user_app.c and user_app.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app.c file template 

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
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern u32 G_u32AntApiCurrentDataTimeStamp;                       /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;        /* From ant_api.c */

extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */



/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static u32 UserApp1_u32DataMsgCount = 0;             /* Counts the number of ANT_DATA packets received */
static u32 UserApp1_u32TickMsgCount = 0;             /* Counts the number of ANT_TICK packets received */

static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */

u8 au8WelcomeMessage1[] = "   Heart rate detector";
u8 au8WelcomeMessage2[] = "   Press B0 to Start";

static bool bUsuallyDisplay = TRUE;
static bool bRGBLED = FALSE;
static bool bAllLCDDisplay = TRUE;
static bool bLedAndBuzzer1 = FALSE;
static bool bLedAndBuzzer2 = FALSE;
static bool bLedOn = FALSE;
static bool bMaxandMin = FALSE;
static bool bRGBRed = FALSE;
static bool bRGBYellow = FALSE;
static bool bRGBGreen = FALSE;
/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


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
  	u8 au8ANT_NETWORK_KEY[] = {0xB9,0xA5,0x21,0xFB,0xBD,0x72,0xC3,0x45};	

	AntAssignChannelInfoType sAntSetupData;

	/* Clear screen and place start messages */

	LCDCommand(LCD_CLEAR_CMD);
	LCDMessage(LINE1_START_ADDR, au8WelcomeMessage1); 
	LCDMessage(LINE2_START_ADDR, au8WelcomeMessage2); 



	/* Configure ANT for this application */
	sAntSetupData.AntChannel          = ANT_CHANNEL_USERAPP;
	sAntSetupData.AntChannelType      = ANT_CHANNEL_TYPE_USERAPP;
	sAntSetupData.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
	sAntSetupData.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;

	sAntSetupData.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
	sAntSetupData.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
	sAntSetupData.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
	sAntSetupData.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
	sAntSetupData.AntFrequency        = ANT_FREQUENCY_USERAPP;
	sAntSetupData.AntTxPower          = ANT_TX_POWER_USERAPP;

	sAntSetupData.AntNetwork = ANT_NETWORK_DEFAULT;
	for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
	{
		sAntSetupData.AntNetworkKey[i] = au8ANT_NETWORK_KEY[i];
	}

	/* If good initialization, set state to Idle */
	if( AntAssignChannel(&sAntSetupData) )
	{
	/* Channel assignment is queued so start timer */

		UserApp1_u32Timeout = G_u32SystemTime1ms;
		
		UserApp1_StateMachine = UserApp1SM_WaitChannelAssign;
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

/*-------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for the ANT channel assignment to finish */
static void UserApp1SM_WaitChannelAssign(void)
{
	/* Check if the channel assignment is complete */
	if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
	{
		UserApp1_StateMachine = UserApp1SM_Idle;
	}

	/* Monitor for timeout */
	if( IsTimeUp(&UserApp1_u32Timeout, 5000) )
	{
		DebugPrintf("\n\r***Channel assignment timeout***\n\n\r");
		UserApp1_StateMachine = UserApp1SM_Error;
	}
      
} /* end UserApp1SM_WaitChannelAssign() */


  /*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void UserApp1SM_Idle(void)
{		
	/*Button(0) to start to work */
	if(WasButtonPressed(BUTTON0))
	{
	  	ButtonAcknowledge(BUTTON0);
	  	LCDCommand(LCD_CLEAR_CMD);
		AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
		bUsuallyDisplay = TRUE;
		bRGBLED = FALSE;
		bAllLCDDisplay = TRUE;	
		bMaxandMin = FALSE;
		bRGBRed = FALSE;
		bRGBYellow = FALSE;
		bRGBGreen = FALSE;
		UserApp1_StateMachine = UserApp1SM_UsuallyHeartRateDetector;

	}
			   
} /* end UserApp1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
static void UserApp1SM_UsuallyHeartRateDetector(void)
{
  	u8 au8WelcomeMessage3[] = "   Heart rate detector";
	u8 au8WelcomeMessage4[] = "   Press B0 to Start";
  	static u16 u16AveragingHeartRate = 0;
	static u16 u16AveragingHeartRate2;
	static u16 u16AveragingHeartRate3;
	static u16 u16NowHeartRate;
	static u8 u8LCDNowHeartRatedisplay1[] = "Heart Rate(N) :";
	static u8 u8LCDNowHeartRatedisplay2[2] ;
	static u8 u8LCDAveragingHeartRatedisplay1[] = "Heart Rate(A) :";
	static u8 u8LCDAveragingHeartRatedisplay2[2] ;
	static u8 u8LCDMaxHeartRate[] = "MaxHR:";
	static u8 u8LCDMaxHeartRate2[2];
	static u8 u8LCDMinHeartRate[] = "MinHR:";
	static u8 u8LCDMinHeartRate2[2];
	static u8 u8CharacterSpaces[200];
	static u8 u8SpacesLength;
	static u16 u16CharacterNumber;
	static u8 u8Bits;
	static u8 u8Tenbit;
	static u8 u8Hundredbit;
	static u8 u8Bits2;
	static u8 u8Tenbit2;
	static u8 u8Hundredbit2;
	static u16 u16TimeCount;
	static u16 u16RecordNumber;     
	static u16 u16Buzzer1Count;
	static u16 u16Buzzer1Count2;
	static u16 u16MaxHeartRate = 0;
	static u16 u16MaxHeartRate2 ;
	static u16 u16MinHeartRate = 200;
	static u16 u16MinHeartRate2 ;
	static u16 u16CasualHeartRate;

	static u16 u16RGBLEDTime ;

	
	if( AntReadAppMessageBuffer() )	   
	{		
		if(G_eAntApiCurrentMessageClass == ANT_DATA)
		{		  
/*--------------------------Average heart rate----------------------------------------------------*/
			u16RecordNumber++;  //use to Calculate the time shown //
			u16AveragingHeartRate += G_au8AntApiCurrentMessageBytes[7];
			
			if(u16RecordNumber == 100)
			{
				bLedOn = TRUE;    // Enter into LED and Buzzer program //
				u16AveragingHeartRate = u16AveragingHeartRate/100+0.5;		// Calculated mean heart rate//
				u16AveragingHeartRate2 = u16AveragingHeartRate;
				u16AveragingHeartRate3 = u16AveragingHeartRate;
				
				/*use to The Numbers are converted to characters*/
				u8Bits2 = u16AveragingHeartRate2 % 10;
				u16AveragingHeartRate2 = u16AveragingHeartRate2 / 10 ;
				u8Tenbit2 = u16AveragingHeartRate2 % 10 ;
				u16AveragingHeartRate2 = u16AveragingHeartRate2 / 10 ;
				u8Hundredbit2 = u16AveragingHeartRate2 % 10 ;
				
				u8LCDAveragingHeartRatedisplay2[0] = u8Hundredbit2 + '0';
				u8LCDAveragingHeartRatedisplay2[1] = u8Tenbit2 + '0';				  
				u8LCDAveragingHeartRatedisplay2[2] = u8Bits2 + '0';	
				
				if(bAllLCDDisplay)    //The default is TRUE //
				{
					LCDMessage(LINE2_START_ADDR, u8LCDAveragingHeartRatedisplay1); 
					LCDMessage(LINE2_START_ADDR+16, u8LCDAveragingHeartRatedisplay2);
				}
								
				u8LCDAveragingHeartRatedisplay2[0] = 0;
				u8LCDAveragingHeartRatedisplay2[1] = 0;
				u8LCDAveragingHeartRatedisplay2[2] = 0;
				
				u16AveragingHeartRate = 0;
				u16RecordNumber = 0;
			}
			
/*-------------------------------------------------------------------------------------------------*/			
			u16TimeCount++;
			
			if(u16TimeCount == 5)  //Update the LCD once every 5 messages//
			{
				u16NowHeartRate = G_au8AntApiCurrentMessageBytes[7];	//Put in real time heart rate//
				u16CharacterNumber = G_au8AntApiCurrentMessageBytes[7];
				u16CasualHeartRate = G_au8AntApiCurrentMessageBytes[7];

				/*Put into maximum heart rate*/
				if(u16CasualHeartRate >= u16MaxHeartRate)
				{
				  	u16MaxHeartRate = u16CasualHeartRate;
				}
				
				/*Put into Minimum heart rate*/
				if(u16CasualHeartRate <= u16MinHeartRate)
				{
				  	u16MinHeartRate = u16CasualHeartRate;
				}
				
				u8Bits = u16CharacterNumber % 10;
				u16CharacterNumber = u16CharacterNumber / 10 ;
				u8Tenbit = u16CharacterNumber % 10 ;
				u16CharacterNumber = u16CharacterNumber / 10 ;
				u8Hundredbit = u16CharacterNumber % 10 ;
					
/*--------------------------Tera Term Display and Real-time rate-------------------------------------------*/
				/*The heart rate of Tera term is shown*/
				for(u8SpacesLength = 4; u8SpacesLength < u16NowHeartRate/4+0.5 ; u8SpacesLength++)
				{
					u8CharacterSpaces[u8SpacesLength] = '-';
				}
				
				u8CharacterSpaces[3] = '|';
				u8CharacterSpaces[2] = u8Bits + '0';
				u8CharacterSpaces[1] = u8Tenbit + '0';
				u8CharacterSpaces[0] = u8Hundredbit + '0';	
				
				u8LCDNowHeartRatedisplay2[0] = u8Hundredbit + '0';
				u8LCDNowHeartRatedisplay2[1] = u8Tenbit + '0';				  
				u8LCDNowHeartRatedisplay2[2] = u8Bits + '0';
				
				if(bUsuallyDisplay) 	//The default is TRUE //
				{
				  	if(bAllLCDDisplay) 	//The default is TRUE //
					{
						LCDMessage(LINE1_START_ADDR, u8LCDNowHeartRatedisplay1); 
						LCDMessage(LINE1_START_ADDR+16, u8LCDNowHeartRatedisplay2);
					}
				}
				
				DebugPrintf(u8CharacterSpaces);
				DebugLineFeed(); 
				
/*--------------------------Buzzer and Led----------------------------------------------------------*/	
				if(bAllLCDDisplay)
				{
					if(bLedOn)			//When the average heart rate came out, the comparison began//
					{
						bRGBLED = TRUE;   	//Enter into RGBLED//
						
						/* average heart rate - 10 < Real-time heart rate < average heart rate + 10*/
						if(u16NowHeartRate > u16AveragingHeartRate3 - 10 & u16NowHeartRate < u16AveragingHeartRate3 + 10) 
						{
							LedOn(GREEN);
							LedOff(YELLOW);
							LedOff(RED);
							bLedAndBuzzer1 = FALSE;
							bLedAndBuzzer2 = FALSE;
							bRGBGreen = TRUE;			//Open the green RGBLED//
							bRGBRed = FALSE;
							bRGBYellow = FALSE;
						}
							
						/* average heart rate + 10 <= Real-time heart rate <= average heart rate + 30*/
						if(u16NowHeartRate <= u16AveragingHeartRate3 + 30 & u16NowHeartRate >= u16AveragingHeartRate3 + 10)
						{
							bLedAndBuzzer1 = TRUE;		//Buzzer Pattern 1 open//
							bLedAndBuzzer2 = FALSE;
							bRGBGreen = FALSE;
							bRGBRed = FALSE;
							bRGBYellow = TRUE;			//Open the yellow RGBLED//							
						}
						
						/* average heart rate - 30 <= Real-time heart rate <= average heart rate - 10*/
						if(u16NowHeartRate <= u16AveragingHeartRate3 - 10 & u16NowHeartRate >= u16AveragingHeartRate3 - 30)
						{
							bLedAndBuzzer1 = TRUE;		//Buzzer Pattern 1 open//
							bLedAndBuzzer2 = FALSE;
							bRGBGreen = FALSE;
							bRGBRed = FALSE;
							bRGBYellow = TRUE;			//Open the yellow RGBLED//
						}
						
						/* average heart rate + 30 < Real-time heart rate*/
						if(u16NowHeartRate > u16AveragingHeartRate3 + 30)
						{
							bLedAndBuzzer2 = TRUE;		//Buzzer Pattern 2 open//
							bLedAndBuzzer1 = FALSE;
							bRGBGreen = FALSE;
							bRGBRed = TRUE;				//Open the red RGBLED//
							bRGBYellow = FALSE;													
						}
						
						/* average heart rate - 30 > Real-time heart rate*/
						if(u16NowHeartRate < u16AveragingHeartRate3 - 30)
						{
							bLedAndBuzzer2 = TRUE;		//Buzzer Pattern 2 open//
							bLedAndBuzzer1 = FALSE;
							bRGBGreen = FALSE;
							bRGBRed = TRUE;					//Open the red RGBLED//
							bRGBYellow = FALSE;												
						}	
					}
				}
/*-------------------------------------------------------------------------------------------------*/	
				/*Real-time heart rate data zero*/
				for(u8SpacesLength = 0; u8SpacesLength < u16NowHeartRate/4+0.5 ; u8SpacesLength++)
				{
					u8CharacterSpaces[u8SpacesLength] = 0;
				}
				
				u8LCDNowHeartRatedisplay2[0] = 0;
				u8LCDNowHeartRatedisplay2[1] = 0;
				u8LCDNowHeartRatedisplay2[2] = 0;				
				u16TimeCount=0;				
			}						
		}	
	}
/*---------------------------------RGB LED ----------------------------------------------------*/	
	if(bRGBLED)				// change in Buzzer and Led //
	{
	  	u16RGBLEDTime++;
		  
	  	if(u16RGBLEDTime == 2)
		{
		  	/*Green RGB*/
		  	if(bRGBGreen)		
			{
			  	LedPWM(LCD_RED, LED_PWM_0);
  				LedPWM(LCD_GREEN, LED_PWM_100);
  				LedPWM(LCD_BLUE, LED_PWM_0);
			}	
			
			/*Yellow RGB*/
			if(bRGBYellow)
			{
			  	LedPWM(LCD_RED, LED_PWM_100);
  				LedPWM(LCD_GREEN, LED_PWM_100);
  				LedPWM(LCD_BLUE, LED_PWM_0);
			}
			
			/*Red RGB*/
			if(bRGBRed)
			{
			  	LedPWM(LCD_RED, LED_PWM_100);
  				LedPWM(LCD_GREEN, LED_PWM_0);
  				LedPWM(LCD_BLUE, LED_PWM_0);
			}
		}
		
		/*Return to white*/
		if(u16RGBLEDTime == 200)
		{
		  	LedPWM(LCD_RED, LED_PWM_100);
  			LedPWM(LCD_GREEN, LED_PWM_100);
  			LedPWM(LCD_BLUE, LED_PWM_100);
		}
		
		if(u16RGBLEDTime == 400)
		{
		  u16RGBLEDTime = 0;
		}
	}
	else					//when bRGBLED Shut down //
	{
		LedPWM(LCD_RED, LED_PWM_100);
		LedPWM(LCD_GREEN, LED_PWM_100);
		LedPWM(LCD_BLUE, LED_PWM_100);	  	
	}
	
/*----------------------------Button(1) to display max and min rate---------------------------------------------*/	
	if(WasButtonPressed(BUTTON1))
	{
	  	ButtonAcknowledge(BUTTON1);
		LCDClearChars(LINE1_START_ADDR, 21);
		
		if(bUsuallyDisplay)
		{
		  	bUsuallyDisplay = FALSE;		//Close the line1 of display//
		}
		else
		{
		  	bUsuallyDisplay = TRUE;			//Open the line1 of display//
		}
		
		if(bMaxandMin)
		{
		  	/*Close the Calculation and display of maximum heart rate and minimum heart rate*/
			bMaxandMin = FALSE;			
		}
		else
		{
		  	/*Open the Calculation and display of maximum heart rate and minimum heart rate*/
		  	bMaxandMin = TRUE;
		}
		
		if(bMaxandMin)
		{
		  	/*Maximum heart rate calculation and display*/
			u16MaxHeartRate2 = u16MaxHeartRate;
			u8Bits = u16MaxHeartRate2 % 10;
			u16MaxHeartRate2 = u16MaxHeartRate2 / 10 ;
			u8Tenbit = u16MaxHeartRate2 % 10 ;
			u16MaxHeartRate2 = u16MaxHeartRate2 / 10 ;
			u8Hundredbit = u16MaxHeartRate2 % 10 ;
			
			u8LCDMaxHeartRate2[0] = u8Hundredbit + '0';
			u8LCDMaxHeartRate2[1] = u8Tenbit + '0';				  
			u8LCDMaxHeartRate2[2] = u8Bits + '0';
			
			LCDMessage(LINE1_START_ADDR, u8LCDMaxHeartRate); 
			LCDMessage(LINE1_START_ADDR+6, u8LCDMaxHeartRate2);	
						
			/*Minimum heart rate calculation and display*/
			u16MinHeartRate2 = u16MinHeartRate;
			u8Bits = u16MinHeartRate2 % 10;
			u16MinHeartRate2 = u16MinHeartRate2 / 10 ;
			u8Tenbit = u16MinHeartRate2 % 10 ;
			u16MinHeartRate2 = u16MinHeartRate2 / 10 ;
			u8Hundredbit = u16MinHeartRate2 % 10 ;
			
			u8LCDMinHeartRate2[0] = u8Hundredbit + '0';
			u8LCDMinHeartRate2[1] = u8Tenbit + '0';				  
			u8LCDMinHeartRate2[2] = u8Bits + '0';
			
			LCDMessage(LINE1_START_ADDR+10, u8LCDMinHeartRate); 
			LCDMessage(LINE1_START_ADDR+17, u8LCDMinHeartRate2);
		}		
	}
	
	/*When in display max and min rate,use Button(2) to emptying maximum heart rate and minimum heart rate */
	if(WasButtonPressed(BUTTON2))
	{
		ButtonAcknowledge(BUTTON2);	
		
		u8LCDMaxHeartRate2[0] = '0';
		u8LCDMaxHeartRate2[1] = '0';				  
		u8LCDMaxHeartRate2[2] = '0';
		
		u8LCDMinHeartRate2[0] = '0';
		u8LCDMinHeartRate2[1] = '0';				  
		u8LCDMinHeartRate2[2] = '0';
		
		u16MaxHeartRate = 0;
		u16MinHeartRate = 0;
		
		LCDMessage(LINE1_START_ADDR, u8LCDMaxHeartRate); 
		LCDMessage(LINE1_START_ADDR+6, u8LCDMaxHeartRate2);	
		LCDMessage(LINE1_START_ADDR+10, u8LCDMinHeartRate); 
		LCDMessage(LINE1_START_ADDR+17, u8LCDMinHeartRate2);		
	}
	
/*------------------------------Buzzer 1 ----------------------------------------------------*/	
	/*Enter into Buzzer Pattern 1 */
	if(bLedAndBuzzer1)	
	{
	  	PWMAudioSetFrequency(BUZZER1, 330);
	  	LedOn(YELLOW);
		LedOff(GREEN);
		LedOff(RED);
		
		u16Buzzer1Count++;
		
		if(u16Buzzer1Count >=1000 & u16Buzzer1Count < 2000)
		{
			PWMAudioOn(BUZZER1);
		}
		
		if(u16Buzzer1Count == 2000)
		{
			PWMAudioOff(BUZZER1);
			u16Buzzer1Count = 0;
		}
	}
	else
	{
	  	u16Buzzer1Count = 0;
		PWMAudioOff(BUZZER1);	
	}
	
	/*Enter into Buzzer Pattern 2 */
	if(bLedAndBuzzer2)
	{
	  	PWMAudioSetFrequency(BUZZER1, 330);
		LedOn(RED);
		LedOff(YELLOW);
		LedOff(GREEN);
		
		u16Buzzer1Count2++;
		
		if(u16Buzzer1Count2 >= 500 & u16Buzzer1Count2 < 1000)
		{
			PWMAudioOn(BUZZER1);
		}
		
		if(u16Buzzer1Count2 == 1000)
		{
			PWMAudioOff(BUZZER1);
			u16Buzzer1Count2 = 0;
		}	  	
	}
	else
	{
	  	PWMAudioOff(BUZZER1);
		u16Buzzer1Count2 = 0;
	}
	
/*--------------------------Button(3) to stop ------------------------------------------------------------------------*/	
	/*Pause the LED RGBLED BUZZER ,but still reception Heart rate data*/
	if(WasButtonPressed(BUTTON3))
	{
	  	ButtonAcknowledge(BUTTON3);
		
		if(bAllLCDDisplay)
  		{
			bAllLCDDisplay = FALSE;
			bLedAndBuzzer1 = FALSE;
			bLedAndBuzzer2 = FALSE;
			bRGBLED = FALSE;
		}
		else
		{
			bAllLCDDisplay = TRUE;
		}
	}
/*--------------------------Button(0) to close ------------------------------------------------------------------------*/
	/*Button(0) to Turn off the heart rate detector and return the initial page*/
	if(WasButtonPressed(BUTTON0))
	{
	  	ButtonAcknowledge(BUTTON0);		
		LCDCommand(LCD_CLEAR_CMD);
		LCDMessage(LINE1_START_ADDR, au8WelcomeMessage1); 
		LCDMessage(LINE2_START_ADDR, au8WelcomeMessage2); 
		
		/*All switches off*/
		bRGBLED = FALSE;
		bUsuallyDisplay = FALSE;
		bAllLCDDisplay = FALSE;
		bLedOn = FALSE;
		bLedAndBuzzer1 = FALSE;
		bLedAndBuzzer2 = FALSE;
		bMaxandMin = FALSE;
		bRGBRed = FALSE;
		bRGBYellow = FALSE;
		bRGBGreen = FALSE;
		
		/*The zeros of all kinds of data*/
		u16RecordNumber = 0;
		u16AveragingHeartRate = 0;
		u16TimeCount = 0;
		u16RGBLEDTime = 0;
		u16MaxHeartRate = 0;
		u16MinHeartRate = 0;
		u16Buzzer1Count = 0;
		u16Buzzer1Count2 = 0;
		
		/*Close LED BUZZER1 , and RGBLED to white*/
		PWMAudioOff(BUZZER1);
		LedOff(YELLOW);
		LedOff(GREEN);
		LedOff(RED);
		LedPWM(LCD_RED, LED_PWM_100);
		LedPWM(LCD_GREEN, LED_PWM_100);
		LedPWM(LCD_BLUE, LED_PWM_100);
		
		AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
		UserApp1_StateMachine = UserApp1SM_Idle;
	}
}
/*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{

} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/