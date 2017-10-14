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

extern u8 G_au8MessageOK[];                            /* From utilities.c */
extern u8 G_au8MessageFAIL[];                          /* From utilities.c */

extern u32 G_u32AntFlags;                              /* From ant.c */

extern u32 G_u32AntApiCurrentMessageTimeStamp;                           /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;           /* From ant_api.c */
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES]; /* From ant_api.c */
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;               /* From ant_api.c  */

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 u32Timeout;                         /* Timeout counter used across states */

static  AntAssignChannelInfoType sChannelInfo; /* ANT channel configuration */
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
	LCDCommand(LCD_CLEAR_CMD);
	/* Configure the ANT radio */
	sChannelInfo.AntChannel          = ANT_CHANNEL;
	sChannelInfo.AntChannelType      = ANT_CHANNEL_TYPE;
	sChannelInfo.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO;
	sChannelInfo.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI;

	sChannelInfo.AntDeviceIdHi       = ANT_DEVICEID_HI;
	sChannelInfo.AntDeviceIdLo       = ANT_DEVICEID_LO;
	sChannelInfo.AntDeviceType       = ANT_DEVICE_TYPE;
	sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE;

	sChannelInfo.AntFrequency        = ANT_FREQUENCY;
	sChannelInfo.AntTxPower          = ANT_TX_POWER;  

	sChannelInfo.AntNetwork = ANT_NETWORK_DEFAULT;

	for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
	{
	sChannelInfo.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
	}

	/* Queue the channel assignment and go to wait state */
	AntAssignChannel(&sChannelInfo);
	u32Timeout = G_u32SystemTime1ms;
	DebugPrintf(" test task started\n\r");
	UserApp1_StateMachine = UserApp1SM_SetupAnt;

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
void UserApp1SM_SetupAnt(void)
{
	/* Check to see if the channel assignment is successful */
	if(AntRadioStatusChannel(ANT_CHANNEL) == ANT_CONFIGURED)
	{
		DebugPrintf("Board test ANT Master ready\n\r");
		DebugPrintf("Device ID: ");
		DebugPrintNumber(ANT_DEVICEID_DEC);
		DebugPrintf(", Device Type 96, Trans Type 1, Frequency 50\n\r");
		UserApp1_StateMachine = UserApp1SM_Idle;
	}

	/* Watch for timeout */
	if(IsTimeUp(&u32Timeout, 3000))
	{
		/* Init failed */
		DebugPrintf(" test cannot assign ANT channel\n\r");
		UserApp1_StateMachine = UserApp1SM_Idle;
	}
} /* end UserApp1SM_SetupAnt */
/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  	AntChannelStatusType eAntCurrentState;
	u8 au8TotalMessageCounter[19];
	u8 au8FailedMeesageCounter[19];
	static u32 u32FailedMessageCounter;
	static u32 u32TotalMessageCounter;
	static u32 u32TemporaryCounter;
	static u8 u8TemporaryCounter2;
	static u8 au8TestMessage[] = {0, 0, 0, 0, 0, 0, 0, 0};
	u8 au8LcdShowMessage[] = "ANT Master"; 
	static bool bStartTransmit = TRUE;
	
	/*open the Channel*/
	if(bStartTransmit)
	{
	  	AntOpenChannelNumber(ANT_CHANNEL);
		bStartTransmit = FALSE;
	}	
	
	if( AntReadAppMessageBuffer() )
	{	
		/* New data message: check what it is */
		if(G_eAntApiCurrentMessageClass == ANT_TICK)
		{

			/* Update and queue the new message data */
			au8TestMessage[0] = 0x5B;
			
			/*Check for failed messages*/
			if(G_au8AntApiCurrentMessageBytes[ANT_TICK_MSG_EVENT_CODE_INDEX]== EVENT_RX_FAIL)
			{
				au8TestMessage[3]++;
				if(au8TestMessage[3] == 0)
				{
					au8TestMessage[2]++;
					if(au8TestMessage[2] == 0)
					{
						au8TestMessage[1]++;
					}
				}

				u32FailedMessageCounter++;
				u32TemporaryCounter = u32FailedMessageCounter;
				
				/*LCD Character conversion*/
				for(u8 i=10;i!=0;i--)
				{
					u8TemporaryCounter2 = u32TemporaryCounter%10;
					u32TemporaryCounter = u32TemporaryCounter/10;
					au8FailedMeesageCounter[i] = u8TemporaryCounter2+'0';
				}
				LCDMessage(LINE2_START_ADDR, au8FailedMeesageCounter);
			}

			au8TestMessage[4] = 0xFF;
			
			/*Check for total messages*/
			au8TestMessage[7]++;

			if(au8TestMessage[7] == 0)
			{
				au8TestMessage[6]++;
				if(au8TestMessage[6] == 0)
				{
					au8TestMessage[5]++;
				}
			}

			u32TotalMessageCounter++;
			u32TemporaryCounter = u32TotalMessageCounter;

			/*LCD Character conversion*/
			for(u8 i=10;i!=0;i--)
			{
				u8TemporaryCounter2 = u32TemporaryCounter%10;
				u32TemporaryCounter = u32TemporaryCounter/10;
				au8TotalMessageCounter[i] = u8TemporaryCounter2+'0';						
			}

			LCDMessage(LINE1_START_ADDR, au8TotalMessageCounter);
			AntQueueAcknowledgedMessage(ANT_CHANNEL, au8TestMessage);
		}
	}	
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  static bool bErrorStateEntered = FALSE;
  
  /* Print error state entry message once; application hangs here */
  if(!bErrorStateEntered)
  {
   DebugPrintf("\n\r***BOARDTEST ERROR STATE***\n\n\r");
   bErrorStateEntered = TRUE;
  }
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
