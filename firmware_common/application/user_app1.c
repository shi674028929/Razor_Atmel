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
pLedCurrentType2 pHead;
pLedCurrentType2 pTail;

	static LedCurrentType aeLedUser[]=
	{
	  {0,0,0,0},
	  {0,0,0,0}	
	};

/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s; 
extern u8 G_u8DebugScanfCharCount;

/* From board-specific source file */


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


/*        Create a linked list                                                                                        */
void CreateList()
{

	for(u16a = 0; u16a < 2; u16a++)
	{
		pLedCurrentType2 pNew = (pLedCurrentType2)malloc(sizeof(LedCurrentType2));
		pNew->eLED2 = aeLedUser[u16a].eLED;
		pNew->u32Time2 = aeLedUser[u16a].u32Time;
		pNew->bOn2 = aeLedUser[u16a].bOn;
		pNew->eCurrentRate2 = aeLedUser[u16a].eCurrentRate;
		pNew->psNext=pTail->psNext;
		pTail->psNext = pNew;
		pTail=pTail->psNext;
		pTail->psNext=NULL;
		
	}
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
    LedOff(RED);
    LedOff(GREEN);
	static u8 au8LedOn[]="1";
	static u8 au8LedOff[]="0";
	u8 au8Initialize[]="************************************************************";
	u8 au8Initialize2[]="LED Programming Interface";
	u8 au8Initialize3[]="Press 1 to program LED command sequence";
	u8 au8Initialize4[]="Press 2 to show current USER program";
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(1, au8LedOff);
	LCDMessage(3, au8LedOff);
	LCDMessage(6, au8LedOff);
	LCDMessage(8, au8LedOff);
	LCDMessage(11, au8LedOff);
	LCDMessage(13, au8LedOff);
	LCDMessage(15, au8LedOff);
	LCDMessage(18, au8LedOff);
	DebugPrintf(au8Initialize);
	DebugLineFeed();
	DebugPrintf(au8Initialize2);
	DebugLineFeed();
	DebugPrintf(au8Initialize3);
	DebugLineFeed();
	DebugPrintf(au8Initialize4);
	DebugLineFeed();
	DebugPrintf(au8Initialize);
	pHead = (pLedCurrentType2)malloc(sizeof(LedCurrentType2));
	pTail = pHead;
	pTail->psNext = NULL;
 
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
	u8 au8Initialize5[]="Enter commands as LED-ONTIME-OFFTIME and press Enter";
	u8 au8Initialize6[]="LED colors: R,O,Y,G,C,B,P,W";
	u8 au8Initialize7[]="Example: R-1000-2000";
	u8 au8Initialize8[]="Press Enter on blank line to end ";
	u8 au8Initialize9[]="If the input is finished, press the fourth button";
	u8 au8Initialize10[]="Press the first button to display the DEMO";
	u8 au8Initialize11[]="Press the second button to display USER";
	u8 au8Initialize12[]="Press the third button to pause the LED";
	u8 au8Initialize13[]="Press the third button to restore the LED";
	u8 au8Initialize14[]="DEMO";
	u8 au8Initialize15[]="---------------------";
	u8 au8Initialize16[]="USER";
	u8 au8Initialize[]="************************************************************";
  	static u32 u32Counter=0;
	static u8 au8LedOn[]="1";
	static u8 au8LedOff[]="0";
	static u8 au8Time[]={'0','0','0','0','0'};
	static u16 u16Counter2=0;
	static u8 au8CommandFormat[1];
  	static u8 u8PressNum=0;
 	static u8 u8CompareNum=0;
  	static u8 u8FalseNum=0;
	static u8 u8TrueNum=0;
	static bool bISOK=FALSE;		//bISOK use to Run the USER LED//
	static u8 u8PressLedNum=0;
	static u8 u8Num=0;
	static u8 u8Num2=0;
	static u8 u8ButtonPress=0;
	static u32 u32LedTime=0;
	static bool bISOK2=FALSE;		//bISOK2 use to the input operation when LED on//
	static bool bISOK3=FALSE;		//bISOK2 use to the input operation when LED off//
	static u32 u32LedTime2=0;
	static u8 au8Button01[1];
	static u8 au8Button02[1];
	static u8 au8Button03[1];
  	static bool bISOK7=FALSE;		//bISOK7 use to Enter user LED instruction//
	static bool bISOK6=FALSE;		//bISOK6 use to Print the user input instructions//
	static u8 au8UserPress[1];
	static bool bISOK5=TRUE;		//bISOK5 use to Whether to enter the control panel//
	static bool bISOK8=FALSE;		//bISOK8 use to Run the demo LED//
	static bool bISOK9=FALSE;		//bISOK9 use to Printing demo instructions//
	static bool bISOK10=TRUE;
	static bool bISOK11=FALSE;
	static bool bISOK12=FALSE;
	static u8 au8LedNum[64];
	static u8 u8t=0;
	static u8 u8t2=0;
	static u16 u16LedDemoTime=0;
	static u8 au8LedOnTime3[3];
	static u8 au8LedOnTime4[4];
	static u8 au8LedOffTime3[3];
	static u8 au8LedOffTime4[4];
	static u8 au8LedOnPrintf3[6];
	static u8 au8LedOnPrintf4[7];
	static u8 au8LedOffPrintf3[5];
	static u8 au8LedOffPrintf4[5];
	u8 au8ErrorPrint[]="Your input is wrong";

	
	static LedCurrentType aeLedUser2[]=
	{
	  {0,0,0,0}	,
	  {0,0,0,0}	
	};

	
	LedCurrentType aeLedDemo[]=
	{
	  {RED,1000,TRUE,LED_PWM_100},
	  {RED,1500,FALSE,LED_PWM_0},
	  {BLUE,2000,TRUE,LED_PWM_100},
	  {BLUE,7000,FALSE,LED_PWM_0}
	};

	
	if(bISOK5)
	{
	  	DebugScanf(au8UserPress);
		
		if(au8UserPress[0]=='1')        //Enter the input command panel//
		{
			bISOK7=TRUE;        
			bISOK=FALSE;		
			bISOK8=FALSE;		
			bISOK5=FALSE;
			DebugLineFeed();
			DebugPrintf(au8Initialize5);
			DebugLineFeed();
			DebugPrintf(au8Initialize6);
			DebugLineFeed();
			DebugPrintf(au8Initialize7);
			DebugLineFeed();
			DebugPrintf(au8Initialize8);
			DebugLineFeed();
			DebugPrintf(au8Initialize9);
			DebugLineFeed();
			LedOff(RED);
			LedOff(WHITE);
			LedOff(PURPLE);
			LedOff(BLUE);
			LedOff(CYAN);
			LedOff(GREEN);
			LedOff(YELLOW);
			LedOff(ORANGE);
			au8UserPress[0]=0;
		}	
		
		if(au8UserPress[0]=='2')    //Enter control LED control panel//
		{
		  	if(bISOK10)		   
			{
				ButtonAcknowledge(BUTTON0);
				DebugLineFeed();
				DebugPrintf(au8Initialize10);
				DebugLineFeed();
				DebugPrintf(au8Initialize11);
				DebugLineFeed();
				DebugPrintf(au8Initialize12);
				DebugLineFeed();
				DebugPrintf(au8Initialize13);
				DebugLineFeed();
				DebugPrintf(au8Initialize);
				DebugLineFeed();
				bISOK10=FALSE;
			}
			
			if(WasButtonPressed(BUTTON0))    //Used to display DEMO//
			{
			  	ButtonAcknowledge(BUTTON0);
				DebugLineFeed();
				DebugPrintf(au8Initialize14);
				DebugLineFeed();
				DebugPrintf(au8Initialize15);
				DebugLineFeed();
				u8t2=0;
				LedOff(RED);
				LedOff(WHITE);
				LedOff(PURPLE);
				LedOff(BLUE);
				LedOff(CYAN);
				LedOff(GREEN);
				LedOff(YELLOW);
				LedOff(ORANGE);
				bISOK8=TRUE;
				bISOK9=TRUE;
				bISOK6=FALSE;
				bISOK=FALSE;
				bISOK11=TRUE;
				bISOK12=FALSE;
			}		  
		  
			if(WasButtonPressed(BUTTON1))      //Used to display USER//
			{
				ButtonAcknowledge(BUTTON1);
				DebugLineFeed();
				DebugPrintf(au8Initialize16);
				DebugLineFeed();
				DebugPrintf(au8Initialize15);
				DebugLineFeed();
				u8t=0;
				LedOff(RED);
				LedOff(WHITE);
				LedOff(PURPLE);
				LedOff(BLUE);
				LedOff(CYAN);
				LedOff(GREEN);
				LedOff(YELLOW);
				LedOff(ORANGE);
				bISOK6=TRUE;
				bISOK=TRUE;
				bISOK8=FALSE;
				bISOK9=FALSE;
				bISOK11=FALSE;
				bISOK12=TRUE;
			}
			
			if(WasButtonPressed(BUTTON2))   //Used to suspend LED//
			{
				ButtonAcknowledge(BUTTON2);
				if(bISOK11)
				{
				 	bISOK8=FALSE;
				}
				if(bISOK12)
				{
				  	bISOK=FALSE;
				}
			}
			
			if(WasButtonPressed(BUTTON3))    //Used to restore LED//
			{
				ButtonAcknowledge(BUTTON3);
				if(bISOK11)
				{
				 	bISOK8=TRUE;
				}
				if(bISOK12)
				{
				  	bISOK=TRUE;
				}
				
			}
		}
	}
	
	   
	
	if(bISOK7)     
	{
		if(G_u8DebugScanfCharCount==1)
		{
			DebugScanf(au8CommandFormat);
			u8PressNum++;
			u8Num++;
			u8Num2++;
			if(u8CompareNum==1)
			{
				bISOK2=TRUE;
			}
			if(u8CompareNum==2)
			{
				bISOK3=TRUE;
			}
			
		}	


		if(au8CommandFormat[0]=='-')   // when'-' ,   change the math//
		{			
			u8CompareNum++;
			au8CommandFormat[0]='NULL';
			u8PressNum=0;
			u8Num=0;
			u8Num2=0;
		}

		if(u8CompareNum==0)    //  choose the led//
		{
			if(u8PressNum==1)
			{
				if(au8CommandFormat[0]=='w' ||au8CommandFormat[0]=='W'||au8CommandFormat[0]=='p'||au8CommandFormat[0]=='P'
				   ||au8CommandFormat[0]=='b'||au8CommandFormat[0]=='B'||au8CommandFormat[0]=='c'||au8CommandFormat[0]=='C'
					 ||au8CommandFormat[0]=='g'||au8CommandFormat[0]=='G'||au8CommandFormat[0]=='y'||au8CommandFormat[0]=='Y'
					   ||au8CommandFormat[0]=='o'||au8CommandFormat[0]=='O'||au8CommandFormat[0]=='r'||au8CommandFormat[0]=='R')
				{
					u8TrueNum=1;
					
					if(u8Num==1)
					{
						if(au8CommandFormat[0]== 'w' || au8CommandFormat[0]== 'W')
						{
							aeLedUser[0].eLED = WHITE;
							aeLedUser[1].eLED = WHITE;
			
							u8Num=0;
						}
						
						if(au8CommandFormat[0]== 'p' || au8CommandFormat[0]== 'P')
						{
							aeLedUser[0].eLED = PURPLE;
							aeLedUser[1].eLED = PURPLE;

							u8Num=0;
						}
						
						if(au8CommandFormat[0]== 'b' || au8CommandFormat[0]== 'B')
						{
							aeLedUser[0].eLED = BLUE;
							aeLedUser[1].eLED = BLUE;

							u8Num=0;
						}
						
						if(au8CommandFormat[0]== 'c' || au8CommandFormat[0]== 'C')
						{
							aeLedUser[0].eLED = CYAN;
							aeLedUser[1].eLED = CYAN;

							u8Num=0;
						}
						
						if(au8CommandFormat[0]== 'g' || au8CommandFormat[0]== 'G')
						{
							aeLedUser[0].eLED = GREEN;
							aeLedUser[1].eLED = GREEN;

							u8Num=0;
						}
						
						if(au8CommandFormat[0]== 'y' || au8CommandFormat[0]== 'Y')
						{
							aeLedUser[0].eLED = YELLOW;
							aeLedUser[1].eLED = YELLOW;

							u8Num=0;
						}
						
						if(au8CommandFormat[0]== 'o' || au8CommandFormat[0]== 'O')
						{
							aeLedUser[0].eLED = ORANGE;
							aeLedUser[1].eLED = ORANGE;

							u8Num=0;
						}
						
						if(au8CommandFormat[0]== 'r' || au8CommandFormat[0]== 'R')
						{
							aeLedUser[0].eLED = RED;
							aeLedUser[1].eLED = RED;

							u8Num=0;
						}
					}
				}
				else
				{
					u8FalseNum=1;
				}
			}
			
			if(u8PressNum>1)
			{
				u8FalseNum=1;
			}
			
		}

		if(u8CompareNum==1)  //Enter the time to turn on the light //
		{
			aeLedUser[0].bOn=TRUE;
			aeLedUser[0].eCurrentRate=LED_PWM_100;
			
			if(u8PressNum>=1&&u8PressNum<=5)
			{
				if(au8CommandFormat[0]>='0'&& au8CommandFormat[0] <= '9')
				{
					u8TrueNum=1;							
					
					if(u8Num==1)
					{
						u32LedTime = au8CommandFormat[0]-'0';
						aeLedUser[0].u32Time = u32LedTime;
						bISOK2=FALSE;
						
					}
					else
					{
						if(bISOK2)
						{
							u32LedTime*=10;
							u32LedTime+=(au8CommandFormat[0]-'0');
							aeLedUser[0].u32Time = u32LedTime;
							bISOK2=FALSE;
						}
					}
					
				}
				else
				{
					u8FalseNum=1;
				}
			}
			
			if(u8PressNum>5)
			{
				u8FalseNum=1;
			}
		}

		if(u8CompareNum==2)   //Enter the time to turn off the lights //
		{
			aeLedUser[1].bOn=FALSE;
			aeLedUser[1].eCurrentRate=LED_PWM_0;
			if(u8PressNum>=1&&u8PressNum<=5)
			{
				if(au8CommandFormat[0]!='\r')
				{
					if(au8CommandFormat[0]>='0'&& au8CommandFormat[0] <= '9')
					{
						u8TrueNum=1;
						

					
						if(u8Num2==1)
						{
							u32LedTime2 = au8CommandFormat[0]-'0';
							aeLedUser[1].u32Time = u32LedTime2;
							bISOK3=FALSE;
							
						}
						else
						{
							if(bISOK3)
							{
								u32LedTime2*=10;
								u32LedTime2+=(au8CommandFormat[0]-'0');
								aeLedUser[1].u32Time = u32LedTime2;
								bISOK3=FALSE;
								
							}
						}
					}
					else
					{
						u8FalseNum=1;
					}
				}
			}
			
			if(u8PressNum>5)
			{
				u8FalseNum=1;
			}
			  
		}

		if(au8CommandFormat[0]=='\r')  // the next input //
		{
			if(u8FalseNum==1)    //If you type in an error//
			{
				u8PressNum=0;
				u8CompareNum=0;
				u8FalseNum=0;
				u8TrueNum=0;
				u8Num=0;
				u8Num2=0;
				u32LedTime=0;
				bISOK2=FALSE;
				bISOK3=FALSE;
				u32LedTime2=0;
				DebugPrintf(au8ErrorPrint);
				DebugLineFeed();
			}
			else
			{
				CreateList();					//If the input is correct, put the content into the list//
				au8CommandFormat[0]='NULL';
				u8PressNum=0;
				u8CompareNum=0;
				u8FalseNum=0;
				u8TrueNum=0;				
				u8Num=0;
				u8Num2=0;
				u32LedTime=0;
				bISOK2=FALSE;
				bISOK3=FALSE;
				u32LedTime2=0;
			}
		}
		
		if(WasButtonPressed(BUTTON3))    //After the input is completed, press BUTTON3 to The back panel//
		{
			ButtonAcknowledge(BUTTON3);
			bISOK7=FALSE;
			bISOK5=TRUE;
			bISOK10=TRUE;
		}
	}
	
	
	if(bISOK6)        //Print the user input instructions//
	{
	  	pLedCurrentType2 pUser = pHead->psNext;
		
		while(NULL != pUser)
		{
		 	aeLedUser2[0].eLED = pUser->eLED2;
			aeLedUser2[0].u32Time = pUser->u32Time2;
			aeLedUser2[0].bOn = pUser->bOn2;
			aeLedUser2[0].eCurrentRate = pUser->eCurrentRate2;
			
			if(aeLedUser2[0].eLED == RED)
			{
		 	 	au8LedNum[0]='R';
			}
			
			if(aeLedUser2[0].eLED == WHITE)
			{
		 	 	au8LedNum[0]='W';
			}
			
			if(aeLedUser2[0].eLED == BLUE)
			{
		 	 	au8LedNum[0]='B';
			}
			
			if(aeLedUser2[0].eLED == PURPLE)
			{
		 	 	au8LedNum[0]='P';
			}
			
			if(aeLedUser2[0].eLED == CYAN)
			{
		 	 	au8LedNum[0]='C';
			}
			
			if(aeLedUser2[0].eLED == ORANGE)
			{
		 	 	au8LedNum[0]='O';
			}
			
			if(aeLedUser2[0].eLED == YELLOW)
			{
		 	 	au8LedNum[0]='Y';
			}
			
			if(aeLedUser2[0].eLED == GREEN)
			{
		 	 	au8LedNum[0]='G';
			}
			
			pUser = pUser->psNext;
			aeLedUser2[1].eLED = pUser->eLED2;
			aeLedUser2[1].u32Time = pUser->u32Time2;
			aeLedUser2[1].bOn = pUser->bOn2;
			aeLedUser2[1].eCurrentRate = pUser->eCurrentRate2;
			
			if(aeLedUser2[1].eLED == RED)
			{
		 	 	au8LedNum[1]='R';
			}
			
			if(aeLedUser2[1].eLED == WHITE)
			{
		 	 	au8LedNum[1]='W';
			}
			
			if(aeLedUser2[1].eLED == BLUE)
			{
		 	 	au8LedNum[1]='B';
			}
			
			if(aeLedUser2[1].eLED == PURPLE)
			{
		 	 	au8LedNum[1]='P';
			}
			
			if(aeLedUser2[1].eLED == CYAN)
			{
		 	 	au8LedNum[1]='C';
			}
			
			if(aeLedUser2[1].eLED == ORANGE)
			{
		 	 	au8LedNum[1]='O';
			}
			
			if(aeLedUser2[1].eLED == YELLOW)
			{
		 	 	au8LedNum[1]='Y';
			}
			
			if(aeLedUser2[1].eLED == GREEN)
			{
		 	 	au8LedNum[1]='G';
			}			
			pUser = pUser->psNext;
			
			if(aeLedUser2[0].u32Time<1000)
			{
				u16LedDemoTime=aeLedUser2[0].u32Time;
				au8LedOnTime3[2]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;
				au8LedOnTime3[1]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;
				au8LedOnTime3[0]=u16LedDemoTime%10+'0';			
				au8LedOnPrintf3[0]=au8LedNum[0];				
				au8LedOnPrintf3[1]='-';
				au8LedOnPrintf3[2]=au8LedOnTime3[0];
				au8LedOnPrintf3[3]=au8LedOnTime3[1];
				au8LedOnPrintf3[4]=au8LedOnTime3[2];
				au8LedOnPrintf3[5]='-';
				DebugPrintf(au8LedOnPrintf3);			
			}
			
			if(aeLedUser2[0].u32Time>=1000)
			{
				u16LedDemoTime=aeLedUser2[0].u32Time;
				au8LedOnTime4[3]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;
				au8LedOnTime4[2]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;
				au8LedOnTime4[1]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;
				au8LedOnTime4[0]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;			
				au8LedOnPrintf4[0]=au8LedNum[0];				
				au8LedOnPrintf4[1]='-';
				au8LedOnPrintf4[2]=au8LedOnTime4[0];
				au8LedOnPrintf4[3]=au8LedOnTime4[1];
				au8LedOnPrintf4[4]=au8LedOnTime4[2];
				au8LedOnPrintf4[5]=au8LedOnTime4[3];  
				au8LedOnPrintf4[6]='-';
				DebugPrintf(au8LedOnPrintf4);
			}
			
			if(aeLedUser2[1].u32Time<1000)
			{
				u16LedDemoTime=aeLedUser2[1].u32Time;
				au8LedOffTime3[2]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;
				au8LedOffTime3[1]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;
				au8LedOffTime3[0]=u16LedDemoTime%10+'0';				
				au8LedOffPrintf3[0]=au8LedOffTime3[0];
				au8LedOffPrintf3[1]=au8LedOffTime3[1];
				au8LedOffPrintf3[2]=au8LedOffTime3[2];
				DebugPrintf(au8LedOffPrintf3);
				DebugLineFeed();
			}
			
			if(aeLedUser2[1].u32Time>=1000)
			{
				u16LedDemoTime=aeLedUser2[1].u32Time;
				au8LedOffTime4[3]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;
				au8LedOffTime4[2]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;
				au8LedOffTime4[1]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;
				au8LedOffTime4[0]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;				
				au8LedOffPrintf4[0]=au8LedOffTime4[0];
				au8LedOffPrintf4[1]=au8LedOffTime4[1];
				au8LedOffPrintf4[2]=au8LedOffTime4[2];
				au8LedOffPrintf4[3]=au8LedOffTime4[3];
				DebugPrintf(au8LedOffPrintf4);
				DebugLineFeed();	
			}
		}
		
		bISOK6 = FALSE;			
	}	
		 
	if(bISOK9)        // Printing demo instructions//
	{
	  	for(u8 n=0;n<4;n++)
		{
		  	if(aeLedDemo[n].eLED == RED)
			{
			  	au8LedNum[n]= 'R';
			}
			
			if(aeLedDemo[n].eLED == WHITE)
			{
			  	au8LedNum[n]= 'W';
			}
			
			if(aeLedDemo[n].eLED ==  PURPLE)
			{
			  	au8LedNum[n]='P';
			}
			
			if(aeLedDemo[n].eLED ==BLUE)
			{
			  	au8LedNum[n]='B';
			}
			
			if(aeLedDemo[n].eLED == CYAN)
			{
			  	au8LedNum[n]='C';
			}
			
			if(aeLedDemo[n].eLED == GREEN)
			{
			  	au8LedNum[n]='G';
			}
			
			if(aeLedDemo[n].eLED == YELLOW)
			{
			  	au8LedNum[n]='Y';
			}
			
			if(aeLedDemo[n].eLED == ORANGE)
			{
			  	au8LedNum[n]='O';
			}
		}
		
		if(u8t2<4)
		{				
			if(aeLedDemo[u8t2].u32Time<1000)
			{
				u16LedDemoTime=aeLedDemo[u8t2].u32Time;
				au8LedOnTime3[2]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;
				au8LedOnTime3[1]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;
				au8LedOnTime3[0]=u16LedDemoTime%10+'0';			
				au8LedOnPrintf3[0]=au8LedNum[u8t2];				
				au8LedOnPrintf3[1]='-';
				au8LedOnPrintf3[2]=au8LedOnTime3[0];
				au8LedOnPrintf3[3]=au8LedOnTime3[1];
				au8LedOnPrintf3[4]=au8LedOnTime3[2];
				au8LedOnPrintf3[5]='-';
				DebugPrintf(au8LedOnPrintf3);			
			}
			
			if(aeLedDemo[u8t2].u32Time>=1000)
			{
				u16LedDemoTime=aeLedDemo[u8t2].u32Time;
				au8LedOnTime4[3]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;
				au8LedOnTime4[2]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;
				au8LedOnTime4[1]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;
				au8LedOnTime4[0]=u16LedDemoTime%10+'0';
				au8LedOnPrintf4[0]=au8LedNum[u8t2];				
				au8LedOnPrintf4[1]='-';
				au8LedOnPrintf4[2]=au8LedOnTime4[0];
				au8LedOnPrintf4[3]=au8LedOnTime4[1];
				au8LedOnPrintf4[4]=au8LedOnTime4[2];
				au8LedOnPrintf4[5]=au8LedOnTime4[3];  
				au8LedOnPrintf4[6]='-';
				DebugPrintf(au8LedOnPrintf4);
			}
			
			if(aeLedDemo[u8t2+1].u32Time<1000)
			{
				u16LedDemoTime=aeLedDemo[u8t2+1].u32Time;
				au8LedOffTime3[2]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;
				au8LedOffTime3[1]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;
				au8LedOffTime3[0]=u16LedDemoTime%10+'0';				
				au8LedOffPrintf3[0]=au8LedOffTime3[0];
				au8LedOffPrintf3[1]=au8LedOffTime3[1];
				au8LedOffPrintf3[2]=au8LedOffTime3[2];
				DebugPrintf(au8LedOffPrintf3);
				DebugLineFeed();						
			}
			
			if(aeLedDemo[u8t2+1].u32Time>=1000)
			{
				u16LedDemoTime=aeLedDemo[u8t2+1].u32Time;
				au8LedOffTime4[3]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;
				au8LedOffTime4[2]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;
				au8LedOffTime4[1]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;
				au8LedOffTime4[0]=u16LedDemoTime%10+'0';
				u16LedDemoTime/=10;		
				au8LedOffPrintf4[0]=au8LedOffTime4[0];
				au8LedOffPrintf4[1]=au8LedOffTime4[1];
				au8LedOffPrintf4[2]=au8LedOffTime4[2];
				au8LedOffPrintf4[3]=au8LedOffTime4[3];
				DebugPrintf(au8LedOffPrintf4);
				DebugLineFeed();						
			}
			u8t2+=2;				
		}
		else
		{
			bISOK9=FALSE;
		}							
	}
	
	
	if(bISOK)                //Run the user's instructions//
	{
	  	u32Counter++;
		u16Counter2++;
		if(u16Counter2==100)
		{
			if(au8Time[2]<'9')
			{
				au8Time[2]++;		
				u16Counter2=0;
			}
			
			else
			{
				au8Time[2]='0';
				u16Counter2=0;	
				if(au8Time[1]<'9')
				{
					au8Time[1]++;
				}
				else
				{
					au8Time[1]='0';
				}
			}
			LCDMessage(47,au8Time);	    
		}
		
		
		if(u32Counter == 10000)
		{
			u32Counter = 0;
		}
		
		pLedCurrentType2 pUserLed = pHead->psNext;
		
		while (NULL != pUserLed)
		{

		  if(u32Counter == pUserLed->u32Time2)
		  {
			    LedPWM(pUserLed->eLED2,pUserLed->eCurrentRate2);
		   
			    if(pUserLed->eLED2 == WHITE)
			    {
				 	if(pUserLed->bOn2)
					{
						LCDMessage(1, au8LedOn);
					}
					else
					{
						LCDMessage(1, au8LedOff);
					}
			    }
			   
				if(pUserLed->eLED2 == PURPLE)
				{
					if(pUserLed->bOn2)
					{
						LCDMessage(3, au8LedOn);
					}
					else
					{
						LCDMessage(3, au8LedOff);
					}
				}
		   
				if(pUserLed->eLED2 == BLUE)
				{
					if(pUserLed->bOn2)
					{
						LCDMessage(6, au8LedOn);
					}
					else
					{
						LCDMessage(6, au8LedOff);
					}
				}
		   
				if(pUserLed->eLED2 == CYAN)
				{
					if(pUserLed->bOn2)
					{
						LCDMessage(8, au8LedOn);
					}
					else
					{
						LCDMessage(8, au8LedOff);
					}
				}
		   
				if(pUserLed->eLED2 == GREEN)
				{
					if(pUserLed->bOn2)
					{
						LCDMessage(11, au8LedOn);
					}
					else
					{
						LCDMessage(11, au8LedOff);
					}
				}
		   
				if(pUserLed->eLED2 == YELLOW)
				{
					if(pUserLed->bOn2)
					{
						LCDMessage(13, au8LedOn);
					}
					else
					{
						LCDMessage(13, au8LedOff);
					}
				}
		   
				if(pUserLed->eLED2 == ORANGE)
				{
					if(pUserLed->bOn2)
					{
						LCDMessage(15, au8LedOn);
					}
					else
					{
						LCDMessage(15, au8LedOff);
					}
				}
		   
				if(pUserLed->eLED2 == RED)
				{
					if(pUserLed->bOn2)
					{
						LCDMessage(18, au8LedOn);
					}
					else
					{
						LCDMessage(18, au8LedOff);
					}
				}
		  }
		  pUserLed = pUserLed->psNext;
		}
	}

	if(bISOK8)                //Run the demo instructions//
	{
	  	u32Counter++;
		u16Counter2++;
		if(u16Counter2==100)
		{
			if(au8Time[2]<'9')
			{
				au8Time[2]++;		
				u16Counter2=0;
			}			
			else
			{
				au8Time[2]='0';
				u16Counter2=0;	
				if(au8Time[1]<'9')
				{
					au8Time[1]++;
				}
				else
				{
					au8Time[1]='0';
				}
			}
			LCDMessage(47,au8Time);	    
		}
		
		
		if(u32Counter == 10000)
		{
			u32Counter = 0;
		}
		
		for(u8 i=0;i<5;i++)
		{
		  if(u32Counter == aeLedDemo[i].u32Time)
		  {
				LedPWM(aeLedDemo[i].eLED,aeLedDemo[i].eCurrentRate);
		   
			    if(aeLedDemo[i].eLED == WHITE)
			    {
					if(aeLedDemo[i].bOn)
					{
						LCDMessage(1, au8LedOn);
					}
					else
					{
						LCDMessage(1, au8LedOff);
					}
			    }
			   
				if(aeLedDemo[i].eLED == PURPLE)
				{
					if(aeLedDemo[i].bOn)
					{
						LCDMessage(3, au8LedOn);
					}
					else
					{
						LCDMessage(3, au8LedOff);
					}
				}
		   
				if(aeLedDemo[i].eLED == BLUE)
				{
					if(aeLedDemo[i].bOn)
					{
						LCDMessage(6, au8LedOn);
					}
					else
					{
						LCDMessage(6, au8LedOff);
					}
				}
		   
				if(aeLedDemo[i].eLED == CYAN)
				{
					if(aeLedDemo[i].bOn)
					{
						LCDMessage(8, au8LedOn);
					}
					else
					{
						LCDMessage(8, au8LedOff);
					}
				}
		   
				if(aeLedDemo[i].eLED == GREEN)
				{
					if(aeLedDemo[i].bOn)
					{
						LCDMessage(11, au8LedOn);
					}
					else
					{
						LCDMessage(11, au8LedOff);
					}
				}
		   
				if(aeLedDemo[i].eLED == YELLOW)
				{
					if(aeLedDemo[i].bOn)
					{
						LCDMessage(13, au8LedOn);
					}
					else
					{
						LCDMessage(13, au8LedOff);
					}
				}
		   
				if(aeLedDemo[i].eLED == ORANGE)
				{
					if(aeLedDemo[i].bOn)
					{
						LCDMessage(15, au8LedOn);
					}
					else
					{
						LCDMessage(15, au8LedOff);
					}
				}
		   
				if(aeLedDemo[i].eLED == RED)
				{
					if(aeLedDemo[i].bOn)
					{
						LCDMessage(18, au8LedOn);
					}
					else
					{
						LCDMessage(18, au8LedOff);
					}
				}
		  }
		}
	}	
}
/* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
