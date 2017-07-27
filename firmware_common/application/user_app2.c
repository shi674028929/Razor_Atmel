/**********************************************************************************************************************
File: user_app2.c                                                                

Description:
This is a user_app2.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp2Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp2RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp2"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp2Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u8 G_u8DebugScanfCharCount;
/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp2_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp2_StateMachine;            /* The state machine function pointer */
//static u32 UserApp2_u32Timeout;                      /* Timeout counter used across states */


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
Function: UserApp2Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp2Initialize(void)
{
   LedOff(YELLOW);   
   LedOff(WHITE);
   LedOff(RED);
   LedOff(GREEN);
   LedOff(PURPLE);
   PWMAudioOn(BUZZER1);





   

  
 
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp2_StateMachine = UserApp2SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp2_StateMachine = UserApp2SM_FailedInit;
  }

} /* end UserApp2Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp2RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp2RunActiveState(void)
{
  UserApp2_StateMachine();

} /* end UserApp2RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp2SM_Idle(void)
{
	static u8 u8RealPassword[]={1,2,3,1,2,3};
	static u8 u8UserPassword[]={0,0,0,0,0,0};
	static u8 u8Index=0;
	static u8 u8Comfirm=0;
	static u16 u16Counter=0;
	static u16 u16Counter2=0;
	static bool bPressed=FALSE;
	static bool bPressed2=FALSE;
	static bool bIsOk=TRUE;
	u8 u8TempIndex;
	static u8 au8PressPassword[5];
	static bool bISOK2=FALSE;
	
	if( IsButtonHeld(BUTTON3, 2000) ) 
	{ 
	  	LedBlink(RED, LED_2HZ); 
		LedBlink(GREEN, LED_2HZ); 
		bISOK2=TRUE;
	}
	
	if(bISOK2)
	{
		if(G_u8DebugScanfCharCount==6)   //scanf your password//
		{
			DebugScanf(au8PressPassword);
			u8RealPassword[0]=au8PressPassword[0]-'0';
			u8RealPassword[1]=au8PressPassword[1]-'0';
			u8RealPassword[2]=au8PressPassword[2]-'0';
			u8RealPassword[3]=au8PressPassword[3]-'0';
			u8RealPassword[4]=au8PressPassword[4]-'0';
			u8RealPassword[5]=au8PressPassword[5]-'0';
			DebugLineFeed();
			G_u8DebugScanfCharCount=0;
			DebugLineFeed();
			LedOff(RED);
			LedOff(GREEN);
			bISOK2=FALSE;
		}
	}

	if(WasButtonPressed(BUTTON3))  //confirm//
	{
		ButtonAcknowledge(BUTTON3);	
		u8Index=0;
		u8Comfirm++;
		LedOff(WHITE);
		LedOff(PURPLE);		
	}
	
	if(u8Comfirm==2)   //compare your password//
	{
		for(u8TempIndex=0;u8TempIndex<6;u8TempIndex++)
		{
			if(u8RealPassword[u8TempIndex]!=
			   u8UserPassword[u8TempIndex])
			{
				bIsOk=FALSE;
				break;
			}
		}
		
		if(bIsOk)
		{
			LedOn(WHITE);
			LedOff(PURPLE);
		}
		else
		{
			LedOff(WHITE);
			LedOn(PURPLE);
		}
		LedOff(BLUE);
		u8Comfirm=0;
		u8Index=0;
		u8TempIndex=0;
		bIsOk=TRUE;
	}
	
	if(u8Comfirm==1)
	{
		LedOn(BLUE);
		
		if(u8Index<6)
		{
			if(WasButtonPressed(BUTTON0))
			{
				ButtonAcknowledge(BUTTON0);
				LedOn(RED);
				PWMAudioOn(BUZZER1);
				u16Counter2=0;
				PWMAudioSetFrequency(BUZZER1,250);
				bPressed=TRUE;
				bPressed2=TRUE;
				u8UserPassword[u8Index]=1;
				u8Index++;
			}
		
			if(WasButtonPressed(BUTTON1))
			{
				ButtonAcknowledge(BUTTON1);
				LedOn(RED);
				PWMAudioOn(BUZZER1);
				u16Counter2=0;
				PWMAudioSetFrequency(BUZZER1,250);
				bPressed=TRUE;
				bPressed2=TRUE;
				u8UserPassword[u8Index]=2;
				u8Index++;
			}
		
			if(WasButtonPressed(BUTTON2))
			{
				ButtonAcknowledge(BUTTON2);
				LedOn(RED);
				PWMAudioOn(BUZZER1);
				u16Counter2=0;
				PWMAudioSetFrequency(BUZZER1,250);
				bPressed=TRUE;
				bPressed2=TRUE;
				u8UserPassword[u8Index]=3;
				u8Index++;
			}
		}

			
			if(bPressed==TRUE)
			{
				u16Counter++;
				
				if(u16Counter==100)
				{
					u16Counter=0;
					LedOff(RED);
					bPressed=FALSE;
				}
			}
		
			if(bPressed2=TRUE)
			{
				u16Counter2++;
				
				if(u16Counter2==200)
				{
				  	u16Counter2=0;
					PWMAudioOff(BUZZER1);
					bPressed2=FALSE;
				}				
			}
	}
} /* end UserAppSM_Idle() */

     
#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp2SM_Error(void) 
{
  
} /* end UserApp2SM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp2SM_FailedInit(void)          
{
    
} /* end UserApp2SM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
