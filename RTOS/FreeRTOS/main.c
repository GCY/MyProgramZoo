#include "stm32f4xx.h"
#include "arm_math.h"
#include "FreeRTOS.h"
#include "task.h"
#include "math.h"
#include "stdio.h"
#include "stdbool.h"
#include "stm32f4xx_usart.h"

#define __FPU_PRESENT
#define __FPU_USED
#define ARM_MATH_CM4
#define __CC_ARM
#define ARM_MATH_MATRIX_CHECK
#define ARM_MATH_ROUNDING

// Macro to use CCM (Core Coupled Memory) in STM32F4
#define CCM_RAM __attribute__((section(".ccmram")))

#define FPU_TASK_STACK_SIZE 256
#define LED_TASK_STACK_SIZE 256

StackType_t fpuTaskStack[FPU_TASK_STACK_SIZE] CCM_RAM;  // Put task stack in CCM
StaticTask_t fpuTaskBuffer CCM_RAM;  // Put TCB in CCM

StackType_t ledTaskStack[LED_TASK_STACK_SIZE] CCM_RAM;  // Put task stack in CCM
StaticTask_t ledTaskBuffer CCM_RAM;  // Put TCB in CCM

StackType_t ledTaskStack2[LED_TASK_STACK_SIZE] CCM_RAM;  // Put task stack in CCM
StaticTask_t ledTaskBuffer2 CCM_RAM;  // Put TCB in CCM

StackType_t ledTaskStack3[LED_TASK_STACK_SIZE] CCM_RAM;  // Put task stack in CCM
StaticTask_t ledTaskBuffer3 CCM_RAM;  // Put TCB in CCM

StackType_t ledTaskStack4[LED_TASK_STACK_SIZE] CCM_RAM;  // Put task stack in CCM
StaticTask_t ledTaskBuffer4 CCM_RAM;  // Put TCB in CCM


bool button_flag = false;

void init_USART3(void);

void test_FPU_test(void* p);
void LED1_TASK(void* p);
void LED2_TASK(void* p);
void LED3_TASK(void* p);
void LED4_TASK(void* p);


void Init_LED()
{
   //Enable the GPIOD Clock
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);


   // GPIOD Configuration
   GPIO_InitTypeDef GPIO_InitStruct;
   GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

   //GPIO_SetBits(GPIOD, GPIO_Pin_12);
   //GPIO_SetBits(GPIOD, GPIO_Pin_13);
   GPIO_Init(GPIOD, &GPIO_InitStruct);   
}

void EXTILine0_Config(void)
{
   EXTI_InitTypeDef   EXTI_InitStructure;
   GPIO_InitTypeDef   GPIO_InitStructure;
   NVIC_InitTypeDef   NVIC_InitStructure;

   /* Enable GPIOA clock */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
   /* Enable SYSCFG clock */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

   /* Configure PA0 pin as input floating */
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   /* Connect EXTI Line0 to PA0 pin */
   SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

   /* Configure EXTI Line0 */
   EXTI_InitStructure.EXTI_Line = EXTI_Line0;
   EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
   EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
   EXTI_InitStructure.EXTI_LineCmd = ENABLE;
   EXTI_Init(&EXTI_InitStructure);

   /* Enable and set EXTI Line0 Interrupt to the lowest priority */
   NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0xFF;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0xFF;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
}

int main(void) {
   SystemInit();
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
   Init_LED();
   EXTILine0_Config();
   init_USART3();

   // Create a task
   // Stack and TCB are placed in CCM of STM32F4
   // The CCM block is connected directly to the core, which leads to zero wait states
   xTaskCreateStatic(test_FPU_test, "FPU", FPU_TASK_STACK_SIZE, NULL, 1, fpuTaskStack, &fpuTaskBuffer);

   xTaskCreateStatic(LED1_TASK, "LED1", LED_TASK_STACK_SIZE, NULL, 2, ledTaskStack, &ledTaskBuffer);
   xTaskCreateStatic(LED2_TASK, "LED2", LED_TASK_STACK_SIZE, NULL, 2, ledTaskStack2, &ledTaskBuffer2);
   xTaskCreateStatic(LED3_TASK, "LED3", LED_TASK_STACK_SIZE, NULL, 2, ledTaskStack3, &ledTaskBuffer3);
   xTaskCreateStatic(LED4_TASK, "LED4", LED_TASK_STACK_SIZE, NULL, 2, ledTaskStack4, &ledTaskBuffer4);


   printf("System Started!\n");
   vTaskStartScheduler();  // should never return

   for (;;);
}

void vApplicationTickHook(void) {
}

/* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created.  It is also called by various parts of the
   demo application.  If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
void vApplicationMallocFailedHook(void) {
   taskDISABLE_INTERRUPTS();
   for(;;);
}

/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
   task.  It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()).  If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
void vApplicationIdleHook(void) {
}

void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName) {
   (void) pcTaskName;
   (void) pxTask;
   /* Run time stack overflow checking is performed if
      configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
      function is called if a stack overflow is detected. */
   taskDISABLE_INTERRUPTS();
   for(;;);
}

StaticTask_t xIdleTaskTCB CCM_RAM;
StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE] CCM_RAM;

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
   implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
   used by the Idle task. */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize) {
   /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
      state will be stored. */
   *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

   /* Pass out the array that will be used as the Idle task's stack. */
   *ppxIdleTaskStackBuffer = uxIdleTaskStack;

   /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
      Note that, as the array is necessarily of type StackType_t,
      configMINIMAL_STACK_SIZE is specified in words, not bytes. */
   *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

static StaticTask_t xTimerTaskTCB CCM_RAM;
static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH] CCM_RAM;

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
   application must provide an implementation of vApplicationGetTimerTaskMemory()
   to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize) {
   *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
   *ppxTimerTaskStackBuffer = uxTimerTaskStack;
   *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

void test_FPU_test(void* p) {
   static float mul = 1.0f;
   float x = 0.0f;
   printf("Start FPU test task.\n");
   while(1){
      while(button_flag){
	 button_flag = false;
	 printf("Push Button\n");
	 mul += 1.0f;
	 if(mul > 10.0f){mul = 0;}
	 vTaskDelay(100/portTICK_PERIOD_MS);
      }    
      //float s = arm_sin_f32(ff);//sinf(ff);
      float y1 = mul * sinf(x * 3.14159f/180.0f);
      float y2 = mul * arm_sin_f32(x * 3.14159f/180.0f);
      x++;
      if(x>360){x=0;}
      printf("%.2f , %.2f\n",y1,y2);
      // TODO some other test

      vTaskDelay(1/portTICK_PERIOD_MS);
   }

   vTaskDelete(NULL);
}

void LED1_TASK(void* p)
{
   while(1){
      GPIO_SetBits(GPIOD, GPIO_Pin_12);
      vTaskDelay(100/portTICK_PERIOD_MS);
      GPIO_ResetBits(GPIOD, GPIO_Pin_12);
      vTaskDelay(100/portTICK_PERIOD_MS);
   }
}

void LED2_TASK(void* p)
{
   while(1){
      GPIO_SetBits(GPIOD, GPIO_Pin_13);
      vTaskDelay(250/portTICK_PERIOD_MS);
      GPIO_ResetBits(GPIOD, GPIO_Pin_13);  
      vTaskDelay(250/portTICK_PERIOD_MS);
   }   
}

void LED3_TASK(void* p)
{
   while(1){
      GPIO_SetBits(GPIOD, GPIO_Pin_14);
      vTaskDelay(500/portTICK_PERIOD_MS);
      GPIO_ResetBits(GPIOD, GPIO_Pin_14);  
      vTaskDelay(500/portTICK_PERIOD_MS);
   }   
}

void LED4_TASK(void* p)
{
   while(1){
      GPIO_SetBits(GPIOD, GPIO_Pin_15);
      vTaskDelay(1000/portTICK_PERIOD_MS);
      GPIO_ResetBits(GPIOD, GPIO_Pin_15);  
      vTaskDelay(1000/portTICK_PERIOD_MS);
   }   
}


/*
 * Configure USART3(PB10, PB11) to redirect printf data to host PC.
 */
void init_USART3(void) {
   GPIO_InitTypeDef GPIO_InitStruct;
   USART_InitTypeDef USART_InitStruct;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

   GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init(GPIOB, &GPIO_InitStruct);

   GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

   USART_InitStruct.USART_BaudRate = 115200;
   USART_InitStruct.USART_WordLength = USART_WordLength_8b;
   USART_InitStruct.USART_StopBits = USART_StopBits_1;
   USART_InitStruct.USART_Parity = USART_Parity_No;
   USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
   USART_Init(USART3, &USART_InitStruct);
   USART_Cmd(USART3, ENABLE);
}

void EXTI0_IRQHandler(void)
{
   if( EXTI_GetITStatus( EXTI_Line0 ) != RESET ){
      button_flag = true;
   }
   EXTI_ClearITPendingBit( EXTI_Line0 );
}
