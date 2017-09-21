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
u8 au8State1[]="Entering state 1";
u8 au8State2[]="Entering state 2";
u8 au8State1Message[] = "SATATE 1";
u8 au8State2Message[] = "SATATE 2";
static u16 u16Counter = 0;
static u8 u8Counter2 = 0;
static bool bISOK=FALSE;
static bool bISOK2=FALSE;
static u8 au8CommandFormat[1];
/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */
extern u8 G_u8DebugScanfCharCount;


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/
static void UserApp1_state1(void)
{
  	DebugPrintf(au8State1);
	DebugLineFeed();
	LCDMessage(LINE1_START_ADDR, au8State1Message); 
	LedOn(WHITE);
	LedOn(BLUE);
	LedOn(CYAN);
	LedOn(LCD_RED);
	LedOn(LCD_BLUE);
	PWMAudioOff(BUZZER1);
	PWMAudioOff(BUZZER2);
	UserApp1_StateMachine = UserApp1SM_Idle;
}

static void UserApp1_state2(void)
{
  	DebugPrintf(au8State2);
	DebugLineFeed();
	LCDMessage(LINE1_START_ADDR, au8State2Message); 
	LedBlink(GREEN, LED_1HZ); 
	LedBlink(YELLOW , LED_2HZ); 
	LedBlink(ORANGE , LED_4HZ);
	LedBlink(RED , LED_8HZ);
	LedOn(LCD_RED);
	LedOn(LCD_GREEN);
	bISOK = TRUE;
	UserApp1_StateMachine = UserApp1SM_Idle;
}

void State2Audio()
{
  	u16Counter++;
	
  	if(u16Counter==1000)
	{
		bISOK2 = TRUE;
		u16Counter = 0;		
	}
	
	if(bISOK2)
	{
	  	u8Counter2++;
		PWMAudioSetFrequency(BUZZER1, 200); 
		PWMAudioSetFrequency(BUZZER2, 200); 
		PWMAudioOn(BUZZER1);
		PWMAudioOn(BUZZER2);
		
		if(u8Counter2==100)
		{
			PWMAudioOff(BUZZER1);
			PWMAudioOff(BUZZER2);
			u8Counter2 = 0;
			bISOK2 = FALSE;
		}
	}
}

void DataZero()
{
  	au8CommandFormat[0]= 0 ;
  	u16Counter = 0;
	u8Counter2 = 0;
	LedOff(WHITE);
	LedOff(BLUE);
	LedOff(CYAN);
	LedOff(RED);
	LedOff(GREEN);
	LedOff(YELLOW);
	LedOff(ORANGE);
	LedOff(LCD_RED);
	LedOff(LCD_GREEN);
	LedOff(LCD_BLUE);
	PWMAudioOff(BUZZER1);
	PWMAudioOff(BUZZER2); 
	LCDCommand(LCD_CLEAR_CMD);
	bISOK = FALSE;
	bISOK2 = FALSE;
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
static void UserApp1SM_Idle(void)
{	
  
	DebugScanf(au8CommandFormat);
	if(au8CommandFormat[0]=='1')
	{
	  	DebugLineFeed();
	  	DataZero();
	  	UserApp1_StateMachine = UserApp1_state1;
	}
	
	if(WasButtonPressed(BUTTON1))
	{
	  	ButtonAcknowledge(BUTTON1);
		DataZero();
		UserApp1_StateMachine = UserApp1_state1;
	}
	
	if(au8CommandFormat[0]=='2')
	{
	  	DebugLineFeed();
	  	DataZero();
	  	UserApp1_StateMachine = UserApp1_state2;
	}
	
	if(WasButtonPressed(BUTTON2))
	{
	  	ButtonAcknowledge(BUTTON2);
		DataZero();
		UserApp1_StateMachine = UserApp1_state2;
	}
	
	if(bISOK)
	{
	  	State2Audio();
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
