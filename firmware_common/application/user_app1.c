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

static u8 UserApp_CursorPosition;
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
  UserApp_CursorPosition = LINE1_START_ADDR;


  
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
	static u16 u16Counter=0;
	static u8 u8Num=0;
	static u8 u8Num2=0;
	static u8 au8Message[255];
	static u8 au8Exchange[1];
	static u8 au8Exchange2[1];
	static u8 au8Exchange3[1];
	static u8 au8Line1Message[20];
	static u8 au8Line2Message[20];
	static u8 u8PressNum=0;
	static u8 au8MessageNum[1];
	static u8 u8Line1Num=0;
	static u8 u8Line2Num=0;
	static u8 au8PressWord[255];
	static u8 u8Button3Num=0;
	static bool bISOK=FALSE;
	static bool bOK=FALSE;
	static u16 u16Counter2=0;
	static u16 u16Counter3=0;
	static u16 u16Counter4=0;
	static bool bISOK2=FALSE;
	static bool bISOK3=FALSE;
	static bool bISOK4=FALSE;
	
		
	if(WasButtonPressed(BUTTON3))      
    {
      ButtonAcknowledge(BUTTON3);
	  u8Button3Num++;

	}
    
	if(u8Button3Num==2)
	{
		DebugScanf(au8PressWord);	
		u8Button3Num=0;
		bOK=TRUE;
	}
	
	    if(bOK)
		{
			au8Message[u8Num2]=au8PressWord[u8Num2];
			u8Num2++;
			u16Counter2++;
			
			if(u16Counter2>256)
			{
				bISOK=TRUE;
				bOK=FALSE;
			}
			
		}
	
	
	
	
	if(bISOK)
	{
		u16Counter++;
		u16Counter3++;
		
		au8MessageNum[0]=au8Message[u8PressNum];
		
		if(au8MessageNum[0]!=NULL)
		{
			u8PressNum++;
	 	}
		else
		{
		  	if(u8PressNum>40)
			{
			  	bISOK2=TRUE;
				bISOK=FALSE;
			}
			
			if(u8PressNum<=40)
			{
			  	bISOK3=TRUE;
				bISOK=FALSE;
			}
		}
	}
		
		if(bISOK3)
		{
		  	u16Counter4++;
			if(u8Line1Num<20)
			{
				au8Exchange[0]=au8Message[u8Line1Num];			
				au8Line1Message[u8Line1Num]=au8Exchange[0];
				u8Line1Num++;
			}
			
			if(u8Line2Num<20)
			{
				au8Exchange3[0]=au8Message[u8Line2Num+20];
				au8Line2Message[u8Line2Num]=au8Exchange3[0];
				u8Line2Num++;
			}
			
			if(u16Counter4==21)
			{
			  	bISOK4=TRUE;
				
			}
			
			if(bISOK4)
			{
				LCDMessage(UserApp_CursorPosition, au8Line1Message);
				LCDMessage(LINE2_START_ADDR, au8Line2Message);
				LCDCommand(LCD_ADDRESS_CMD | UserApp_CursorPosition);
				bISOK3=FALSE;
			}

		}

	
		if(bISOK2)
		{
			u16Counter++;
			if(u16Counter==260)
			{
				LCDMessage(UserApp_CursorPosition, au8Line2Message);
				LCDMessage(LINE2_START_ADDR, au8Line1Message);
				LCDCommand(LCD_ADDRESS_CMD | UserApp_CursorPosition);
				u8Num=0;
				u8Line1Num=0;
				u8Line2Num=0;
				au8Exchange2[0]=au8Message[0];
			}
			
			if(u16Counter>200)
			{
				if(u8Num<u8PressNum-1)
				{
					au8Exchange[0]=au8Message[u8Num+1];
					au8Message[u8Num]=au8Exchange[0];
					u8Num++;			
				}
				
				if(u8Num==u8PressNum-1)
				{
					au8Message[u8PressNum-1]=au8Exchange2[0];
				}
			}
			
			if(u16Counter>200+u8PressNum)
			{
				if(u8Line1Num<20)
				{
					au8Exchange[0]=au8Message[u8Line1Num];			
					au8Line1Message[u8Line1Num]=au8Exchange[0];
					u8Line1Num++;
				}
				
				if(u8Line2Num<20)
				{
					au8Exchange3[0]=au8Message[u8Line2Num+20];
					au8Line2Message[u8Line2Num]=au8Exchange3[0];
					u8Line2Num++;
				}
			}
			
			if(u16Counter==1000)
			{
				u16Counter=0;
				u8PressNum=0;
			}
		}
	
}/* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
