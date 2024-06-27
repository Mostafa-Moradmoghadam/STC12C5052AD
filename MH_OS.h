#ifndef MH_OS_H
#define MH_OS_H

#ifdef __cplusplus
extern "C" {
#endif


	
#define AvailableTask_Max       5

typedef enum
{
 RunMail_Type_Normal=0,
 RunMail_Type_Delay,
}RunMail_Type;

typedef enum
{
  TaskStatus_Type_Active,
  TaskStatus_Type_WaitForDelay,
}TaskStatus_Type;

typedef void (*TaskFunc_t)(RunMail_Type runMail,unsigned char *paramsData);
/*----------------Example Of Task:-----------------------
void App_Task1(RunMail_Type runMail,uint8_t *paramsData)
{
  switch(Task_GetStepParam(paramsData))
  {
   case 0:
   {
		.
		.
		.
		.

------------------------------------------------------*/

typedef struct
{
 TaskFunc_t      Function;
 unsigned int        Delay;
 TaskStatus_Type Status;
 unsigned char  StepOpt;
}Task_t;
extern void Timer0_Int(void);
unsigned char Task_RegisterNew(TaskFunc_t taskFunc);
void    Task_RunCoOperativeSystem(void);
unsigned char Task_GetStepParam(unsigned char *paramIn);
void    Task_SetNextStepParam(unsigned char nexStep);
void    Task_Delay_ms( unsigned int delms,unsigned char stp);

/*------------------Private Used-------------*/
void Task_IncTick(void);
void Task_TickInit(void);



#ifdef __cplusplus
}
#endif
#endif
