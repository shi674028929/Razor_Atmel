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
	u8 au8WelcomeMessage1[] = "Hide and Go Seek";
	u8 au8WelcomeMessage2[] = "Press B0 to Start";
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
		sAntSetupData.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
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
  	static bool bSeekOrHide = TRUE;
	
  	if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_OPEN)
  	{
		AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
	}
	
	/*Start the game and switch roles*/
	if(WasButtonPressed(BUTTON0))
	{
	  	ButtonAcknowledge(BUTTON0);
	  	LCDCommand(LCD_CLEAR_CMD);		
		AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
		
	  	if(bSeekOrHide)
		{	
		  	bSeekOrHide = FALSE ;
		  	UserApp1_StateMachine = UserApp1SM_SeekStartOpen;		//Seek 	Program entrance//
		}		
		else
		{	
		  	bSeekOrHide = TRUE ;
		  	UserApp1_StateMachine = UserApp1SM_HideStartOpen;		//Hide 	Program entrance//
		}
	}

	
    
} /* end UserApp1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
static void UserApp1SM_HideStartOpen(void)
{
  	u8 au8HideStartMessage[] = "Hide!";
	LCDMessage(LINE1_START_ADDR,au8HideStartMessage);
	UserApp1_StateMachine = UserApp1SM_HideOver;	
}
/*-------------------------------------------------------------------------------------------------------------------*/
static void UserApp1SM_HideOver(void)
{
  	u8 au8HideOverMessage[] = "You found me";
	static u8 au8TestMessage2[] = {0, 0, 0, 0, 0, 0, 0, 0};
	u8 au8WelcomeMessage5[] = "Hide and Go Seek";
	u8 au8WelcomeMessage6[] = "Press B0 to Start";
	static u16 u16CloseLedTime2;
	static bool bHideOver = FALSE;
	
	/* if all Data received are 0X01 Says he's been found*/
  	if( AntReadAppMessageBuffer() )	    
	{
	  	if(G_eAntApiCurrentMessageClass == ANT_DATA)
		{
		  	if(G_au8AntApiCurrentMessageBytes[0] == 0x01 )
			{
			  	LedOn(WHITE);
			}
			
			if(G_au8AntApiCurrentMessageBytes[0] == 0x00)
			{
			  	LedOff(WHITE);
			}
			
			if(G_au8AntApiCurrentMessageBytes[1] == 0x01 )
			{
			  	LedOn(PURPLE);
			}
			
			if(G_au8AntApiCurrentMessageBytes[1] == 0x00)
			{
			  	LedOff(PURPLE);
			}
			
			if(G_au8AntApiCurrentMessageBytes[2] == 0x01 )
			{
			  	LedOn(BLUE);
			}
			
			if(G_au8AntApiCurrentMessageBytes[2] == 0x00)
			{
			  	LedOff(BLUE);
			}
			
			if(G_au8AntApiCurrentMessageBytes[3] == 0x01 )
			{
			  	LedOn(CYAN);
			}
			
			if(G_au8AntApiCurrentMessageBytes[3] == 0x00)
			{
			  	LedOff(CYAN);
			}
			
			if(G_au8AntApiCurrentMessageBytes[4] == 0x01 )
			{
			  	LedOn(GREEN);
			}
			
			if(G_au8AntApiCurrentMessageBytes[4] == 0x00)
			{
			  	LedOff(GREEN);
			}
			
			if(G_au8AntApiCurrentMessageBytes[5] == 0x01 )
			{
			  	LedOn(YELLOW);
			}
			
			if(G_au8AntApiCurrentMessageBytes[5] == 0x00)
			{
			  	LedOff(YELLOW);
			}
			
			if(G_au8AntApiCurrentMessageBytes[6] == 0x01 )
			{
			  	LedOn(ORANGE);
			}
			
			if(G_au8AntApiCurrentMessageBytes[6] == 0x00)
			{
			  	LedOff(ORANGE);
			}
			
			if(G_au8AntApiCurrentMessageBytes[7] == 0x01 )
			{
			  	LedOn(RED);
				LCDMessage(LINE2_START_ADDR,au8HideOverMessage);
				bHideOver = TRUE;
			}
			
			if(G_au8AntApiCurrentMessageBytes[7] == 0x00)
			{
			  	LedOff(RED);
			}
		}
		
		if(G_eAntApiCurrentMessageClass == ANT_TICK)
		{
			au8TestMessage2[7]++;
			
			if(au8TestMessage2[7] == 0)
			{
				au8TestMessage2[6]++;
				if(au8TestMessage2[6] == 0)
				{
			  		au8TestMessage2[5]++;
				}
			}
			
			AntQueueAcknowledgedMessage(ANT_CHANNEL_USERAPP, au8TestMessage2);
		}
	}
	
	/*After the 4 s to enter Idle*/
	if(bHideOver)
	{
		u16CloseLedTime2++;
		
		if(u16CloseLedTime2 == 4000)
		{
			LedOff(WHITE);
			LedOff(PURPLE);
			LedOff(BLUE);
			LedOff(CYAN);
			LedOff(GREEN);
			LedOff(YELLOW);
			LedOff(ORANGE);
			LedOff(RED);
			LCDMessage(LINE1_START_ADDR, au8WelcomeMessage5); 
			LCDMessage(LINE2_START_ADDR, au8WelcomeMessage6); 
			au8TestMessage2[5] = 0;
			au8TestMessage2[6] = 0;
			au8TestMessage2[7] = 0;
			u16CloseLedTime2 = 0;
			AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
			bHideOver = FALSE;
			
			UserApp1_StateMachine = UserApp1SM_Idle;	
		}
	}
	
	if(WasButtonPressed(BUTTON3))			//Manual end game//
	{
	  	ButtonAcknowledge(BUTTON3);
		LedOff(WHITE);
		LedOff(PURPLE);
		LedOff(BLUE);
		LedOff(CYAN);
		LedOff(GREEN);
		LedOff(YELLOW);
		LedOff(ORANGE);
		LedOff(RED);
		
		UserApp1_StateMachine = UserApp1SM_Idle;		
	}
}
/*-------------------------------------------------------------------------------------------------------------------*/
static void UserApp1SM_SeekStartOpen(void)
{
  	u8 au8SeekStartMessage1[] = "Seeker  wait to 0";
	u8 au8SeekStartMessage2[] = "Ready or not";
	u8 au8SeekStartMessage3[] = "Here I come!";
	static u8 au8SeekCountdown[1];
	static u16 u16SeekStartTime = 0;
	static u8 u8Countdown = 10;
	static u8 u8Countdown1 = 1;

	/*Display countdown*/
	if(u16SeekStartTime == 0)
	{

		LCDMessage(LINE1_START_ADDR,au8SeekStartMessage1);
		
		if(u8Countdown == 10 )
		{
		  	u16SeekStartTime = 1;
			au8SeekCountdown[0] = '1';
			au8SeekCountdown[1] = '0';
			LCDMessage(LINE2_START_ADDR,au8SeekCountdown);	
		}
	}
	
	if( AntReadAppMessageBuffer() )				//Then the countdown begins to move after receiving the data//
	{
		if(G_eAntApiCurrentMessageClass == ANT_DATA)
		{
			u8Countdown1 = 2;				//Bool function failure,So I use this to Instead of bool //
		}
	}
	
	if(u8Countdown1 == 2)
	{
	  	u16SeekStartTime++;
	}
			
	if(u16SeekStartTime == 1000 )
	{
	  	u8Countdown = u8Countdown -1 ;
		au8SeekCountdown[0] = '0';
		au8SeekCountdown[1] = u8Countdown +48 ;
		LCDCommand(LCD_CLEAR_CMD);
		LCDMessage(LINE2_START_ADDR,au8SeekCountdown);		
		u16SeekStartTime = 0;			
	}
	
	if(u8Countdown == 0)			//Enter the next link at the end of the countdown//
	{	
		LCDCommand(LCD_CLEAR_CMD);
		LCDMessage(LINE1_START_ADDR,au8SeekStartMessage2);
		LCDMessage(LINE2_START_ADDR,au8SeekStartMessage3);
		u8Countdown = 10 ;
		u8Countdown1 = 1;
		
		UserApp1_StateMachine = UserApp1SM_SeekFindProcess;
	}
}
/*-------------------------------------------------------------------------------------------------------------------*/
static void UserApp1SM_SeekFindProcess(void)
{
	static u8 au8TestMessage[] = {0, 0, 0, 0, 0, 0, 0, 0};
	static u8 u8temp;
	static u16 u16WaitToOver;
	static u16 u16Buzzer1Time;
	static u16 u16Buzzer1Counter;
	
	PWMAudioSetFrequency(BUZZER1, 500);
	u16Buzzer1Counter++;
	
	if(u16Buzzer1Counter == u16Buzzer1Time)
	{
		PWMAudioOn(BUZZER1);
	}

	if(u16Buzzer1Counter > 2*u16Buzzer1Time)
	{
		PWMAudioOff(BUZZER1);
		u16Buzzer1Counter = 0;
	}
  
  	if(WasButtonPressed(BUTTON3))			//Manual end game//
	{
	  	ButtonAcknowledge(BUTTON3);
		LedOff(WHITE);
		LedOff(PURPLE);
		LedOff(BLUE);
		LedOff(CYAN);
		LedOff(GREEN);
		LedOff(YELLOW);
		LedOff(ORANGE);
		LedOff(RED);
		PWMAudioOff(BUZZER1);
		
		UserApp1_StateMachine = UserApp1SM_Idle;		
	}
	
	/*Light the light through the DBM size of the data*/
	if( AntReadAppMessageBuffer() )
	{
		if(G_eAntApiCurrentMessageClass == ANT_DATA)
		{
			u8temp = abs (G_sAntApiCurrentMessageExtData.s8RSSI);
			
			if(u8temp <= 90)
			{
				LedOn(WHITE);
				au8TestMessage[0] = 0x01;
				u16Buzzer1Time = 1800;
			}
			else
			{
				LedOff(WHITE);
				au8TestMessage[0] = 0x00;	
			}
			
			if(u8temp <= 87)
			{
				LedOn(PURPLE);
				au8TestMessage[1] = 0x01;
				u16Buzzer1Time = 1600;
			}
			else
			{
				LedOff(PURPLE);
				au8TestMessage[1] = 0x00;
			}
			
			if(u8temp <= 82)
			{
				LedOn(BLUE);
				au8TestMessage[2] = 0x01;
				u16Buzzer1Time = 1400;
			}
			else
			{
				LedOff(BLUE);
				au8TestMessage[2] = 0x00;
			}
			
			if(u8temp <= 77)
			{
				LedOn(CYAN);
				au8TestMessage[3] = 0x01;
				u16Buzzer1Time = 1200;
			}
			else
			{
				LedOff(CYAN);
				au8TestMessage[3] = 0x00;
			}
			
			if(u8temp <= 72)
			{
				LedOn(GREEN);
				au8TestMessage[4] = 0x01;
				u16Buzzer1Time = 1000;
			}
			else
			{
				LedOff(GREEN);
				au8TestMessage[4] = 0x00;
			}
			
			if(u8temp <= 67)
			{
				LedOn(YELLOW);
				au8TestMessage[5] = 0x01;
				u16Buzzer1Time = 800;
			}
			else
			{
				LedOff(YELLOW);
				au8TestMessage[5] = 0x00;
			}
			
			if(u8temp <= 62)
			{
				LedOn(ORANGE);
				au8TestMessage[6] = 0x01;
				u16Buzzer1Time = 600;
			}
			else
			{
				LedOff(ORANGE);
				au8TestMessage[6] = 0x00;
			}
			
			if(u8temp <= 55)
			{
				LedOn(RED);
				au8TestMessage[7] = 0x01;	
				u16Buzzer1Time = 400;
				u16WaitToOver++;	
			}
			else
			{
				LedOff(RED);
				au8TestMessage[7] = 0x00;
			}			
		}
				
		if(G_eAntApiCurrentMessageClass == ANT_TICK)
		{		  	
			AntQueueAcknowledgedMessage(ANT_CHANNEL_USERAPP, au8TestMessage);
		}
		
		if(u16WaitToOver == 10)
		{
			u16WaitToOver = 0;
			u8temp = 100;
			UserApp1_StateMachine = UserApp1SM_SeekFindOver;
		}								
	}
}
/*-------------------------------------------------------------------------------------------------------------------*/
static void UserApp1SM_SeekFindOver(void)
{
  	u8 au8SeekOverMessage[] = "Found you!";
	u8 au8WelcomeMessage3[] = "Hide and Go Seek";
	u8 au8WelcomeMessage4[] = "Press B0 to Start";
	static u16 u16CloseLedTime;
	
	/*Find and blink*/
	if(u16CloseLedTime == 0)
	{
		LedBlink(WHITE, LED_2HZ);
		LedBlink(PURPLE, LED_2HZ);
		LedBlink(BLUE, LED_2HZ);
		LedBlink(CYAN, LED_2HZ);
		LedBlink(GREEN, LED_2HZ);
		LedBlink(YELLOW, LED_2HZ);
		LedBlink(ORANGE, LED_2HZ);
		LedBlink(RED, LED_2HZ);
		LCDCommand(LCD_CLEAR_CMD);
		LCDMessage(LINE1_START_ADDR,au8SeekOverMessage);
	}
	
	/*After the 4 s to enter Idle*/
	u16CloseLedTime++;
	if(u16CloseLedTime == 4000)
	{
		LedOff(WHITE);
		LedOff(PURPLE);
		LedOff(BLUE);
		LedOff(CYAN);
		LedOff(GREEN);
		LedOff(YELLOW);
		LedOff(ORANGE);
		LedOff(RED);
		PWMAudioOff(BUZZER1);
		u16CloseLedTime = 0;
		LCDCommand(LCD_CLEAR_CMD);
		LCDMessage(LINE1_START_ADDR, au8WelcomeMessage3); 
		LCDMessage(LINE2_START_ADDR, au8WelcomeMessage4); 
		AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
		
		UserApp1_StateMachine = UserApp1SM_Idle;		
	}
}
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
