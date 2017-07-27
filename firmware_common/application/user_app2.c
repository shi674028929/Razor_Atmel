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
  static u8 au8PressName[1];
  static u32 u32CompareNum=0;
  static u8 u8PressNum=0;
  static u8 u8JumpNum=0;
  static u8 u8Counter=1;
  static u8 au8Line1[10];
  static u8 au8Line2[]='*';
  static u32 u32Num;
  static u16 u16Power=1;
  static u16 u16Power2=1;
  

  if(G_u8DebugScanfCharCount==1)
  {
		u8PressNum=1;
		DebugScanf(au8PressName);		
  }
  
  if(u8JumpNum==0)
  {
		if(u8PressNum==1)
		{
			if(au8PressName[0]=='s')
			{
				u8JumpNum++;
				u8PressNum=0;
			}
			else
			{
				u8JumpNum=0;
				u8PressNum=0;
			}
		}
  }
  
		
  if(u8JumpNum==1)
  {
		if(u8PressNum==1)
		{
			if(au8PressName[0]=='h')
			{
				u8JumpNum++;
				u8PressNum=0;
			}
			else
			{
				u8JumpNum=0;
				u8PressNum=0;
			}
		}
  }

  if(u8JumpNum==2)
  {
		if(u8PressNum==1)
		{
			if(au8PressName[0]=='i')
			{
				u8JumpNum++;
				u8PressNum=0;
			}
			else
			{
				u8JumpNum=0;
				u8PressNum=0;
			}
		}
  }  
  
  if(u8JumpNum==3)
  {
		if(u8PressNum==1)
		{
			if(au8PressName[0]=='j')
			{
				u8JumpNum++;
				u8PressNum=0;
			}
			else
			{
				u8JumpNum=0;
				u8PressNum=0;
			}
		}
  }
  
  if(u8JumpNum==4)
  {
		if(u8PressNum==1)
		{
			if(au8PressName[0]=='i')
			{
				u8JumpNum++;
				u8PressNum=0;
			}
			else
			{
				u8JumpNum=0;
				u8PressNum=0;
			}
		}
  }
  
  if(u8JumpNum==5)
  {
		if(u8PressNum==1)
		{
			if(au8PressName[0]=='a')
			{
				u8JumpNum++;
				u8PressNum=0;
			}
			else
			{
				u8JumpNum=0;
				u8PressNum=0;
			}
		}
  }
  
  if(u8JumpNum==6)
  {
	if(u8PressNum==1)
		{
			if(au8PressName[0]=='h')
			{
				u8JumpNum++;
				u8PressNum=0;
			}
			else
			{
				u8JumpNum=0;
				u8PressNum=0;
			}
		}
  }
  
  if(u8JumpNum==7)
  {
		if(u8PressNum==1)
		{
			if(au8PressName[0]=='a')
			{
				u8JumpNum++;
				u8PressNum=0;
			}
			else
			{
				u8JumpNum=0;
				u8PressNum=0;
			}
		}
  }
  
  if(u8JumpNum==8)
  {
		if(u8PressNum==1)
		{
			if(au8PressName[0]=='o')
			{
				u8JumpNum++;
				u8PressNum=0;
				u32CompareNum++;
				
				if(u16Power2<=u8Counter)
				{
				  	u16Power2++;
					u16Power*=10;
				}
				
				if(u32CompareNum>=u16Power)
				{
				  u32Num=u32CompareNum;
					while(u32Num>=10)
					{
						u8Counter++;
						u32Num=u32Num/10;
					}
				}
				
				au8Line1[u8Counter+1]='*';
				au8Line1[u8Counter-1]='*';
				au8Line1[u8Counter]='*';
				
				DebugLineFeed();
				DebugPrintf(au8Line1);
				DebugLineFeed();
				DebugPrintf(au8Line2);
				DebugPrintNumber(u32CompareNum);
				DebugPrintf(au8Line2);
				DebugLineFeed();
				DebugPrintf(au8Line1);
				DebugLineFeed();
				u8JumpNum=0;
			}
			else
			{
				u8JumpNum=0;
				u8PressNum=0;
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
