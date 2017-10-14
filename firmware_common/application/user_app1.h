/**********************************************************************************************************************
File: user_app1.h                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
1. Follow the instructions at the top of user_app1.c
2. Use ctrl-h to find and replace all instances of "user_app1" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

Description:
Header file for user_app1.c

**********************************************************************************************************************/

#ifndef __USER_APP1_H
#define __USER_APP1_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
/* IF YOU CHANGE DEVICE TYPE OR TRANSMISSION TYPE, YOU MUST CHANGE 
THE STARTUP TEXT MESSAGE IN UserApp1SM_SetupAnt */
#define	ANT_DEVICEID_LO			       		    (u8)0xD5
#define ANT_DEVICEID_HI		         			(u8)0x11
#define ANT_DEVICEID_DEC          				(u32)4565

#define	ANT_DEVICE_TYPE					   		 DEVICE_TYPE
#define	ANT_TRANSMISSION_TYPE	    			(u8)0x01

/* Default channel configuration parameters */
#define ANT_CHANNEL               			    ANT_CHANNEL_1
#define ANT_CHANNEL_TYPE          				CHANNEL_TYPE_MASTER
#define ANT_NETWORK               				(u8)0

#define ANT_CHANNEL_PERIOD_DEC     				(u16)8192
#define ANT_CHANNEL_PERIOD_HEX      			(u16)0x2000
#define ANT_CHANNEL_PERIOD_HI	    			(u8)0x20
#define ANT_CHANNEL_PERIOD_LO		  		    (u8)0x00
  
#define ANT_FREQUENCY					        (50)
#define ANT_TX_POWER					  	    RADIO_TX_POWER_0DBM

/* MPG Board Test specific Device Types */
#define	DEVICE_TYPE		              			(u8)0x60

/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void UserApp1Initialize(void);
void UserApp1RunActiveState(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp1SM_Idle(void);    

static void UserApp1SM_Error(void);         
void UserApp1SM_SetupAnt(void);

#endif /* __USER_APP1_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
