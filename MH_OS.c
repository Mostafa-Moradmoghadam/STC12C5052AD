#include "MH_OS.h"



static Task_t     AvailableTask[AvailableTask_Max];
static unsigned char    taskRegister_count=0;
static signed char     countRunTask=0;

static volatile   unsigned char TickcountLoop=0;   

unsigned char Task_RegisterNew(TaskFunc_t taskFunc)
{
  if(taskRegister_count==AvailableTask_Max)
    return 0;
  else
  {
    AvailableTask[taskRegister_count].Delay=0;
    AvailableTask[taskRegister_count].Status=TaskStatus_Type_Active;
    AvailableTask[taskRegister_count].StepOpt=0;
    AvailableTask[taskRegister_count].Function=taskFunc; 
    
    taskRegister_count++; 
    
    return 1;
  }
}
//Non Priority System and no task termination
void Task_RunCoOperativeSystem()
{
	Task_TickInit();	
    while(1)
    {
        //Normal Task Run
        for(countRunTask=0;countRunTask<taskRegister_count;countRunTask++)
        { 
         switch(AvailableTask[countRunTask].Status)
         {
            case TaskStatus_Type_Active:
            {
             AvailableTask[countRunTask].Function(RunMail_Type_Normal,&AvailableTask[countRunTask].StepOpt);
            }break;
            case TaskStatus_Type_WaitForDelay:
            { 
             if(AvailableTask[countRunTask].Delay==0)
             {
              AvailableTask[countRunTask].Status=TaskStatus_Type_Active;
              AvailableTask[countRunTask].Function(RunMail_Type_Delay,&AvailableTask[countRunTask].StepOpt);              
             }
            }break;            
         }
        } 
    }
}

void Task_SetNextStepParam(unsigned char nexStep)
{
 AvailableTask[countRunTask].StepOpt=nexStep;
}

void Task_Delay_ms(unsigned int delms,unsigned char stp)
{
   AvailableTask[countRunTask].Delay=delms;
   AvailableTask[countRunTask].Status=TaskStatus_Type_WaitForDelay;
   AvailableTask[countRunTask].StepOpt=stp;
}

unsigned char Task_GetStepParam(unsigned char *paramIn)
{
 return *paramIn;
}

void Task_IncTick()
{
    for(TickcountLoop=0;TickcountLoop<taskRegister_count; TickcountLoop++)
    { 
     //Delay Tick Control  
     if(AvailableTask[TickcountLoop].Status==TaskStatus_Type_WaitForDelay)
     {
       if(AvailableTask[TickcountLoop].Delay)
         AvailableTask[TickcountLoop].Delay--;
     }
    }
}
void Task_TickInit(void)
{
	Timer0_Int();	
		
}
