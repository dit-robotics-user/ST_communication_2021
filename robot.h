#ifndef __ROBOT_H
#define __ROBOT_H

/* 	In robot.h & robot.c define:
		struct ROBOT includes ALL of the whole robot
		ALL interruption behavior													*/

#include "hardware.h"
//#include "controlsystem.h"
#include "dma.h"
#include <stdio.h>

//struct COMMAND
//{
//	volatile enum MODE		Mode;
//	volatile double 		X;
//	volatile double 		Y;
//	volatile double 		Theta;
//	volatile double			Speed;
//	volatile short	 		TurnDirection;						// 1 left, -1 right, 0 auto
//	volatile short	 		LinearDirection;					// 1 fw, -1 rv, 0 auto
//	volatile uint8_t		Script;
//};

struct ROBOT
{
//	volatile short 				NavigateComplete;
//	volatile short 				RepositionComplete;
//	volatile short 				BypassComplete;
//	struct COMMAND				Command;
//	struct NAVIGATION 			PosNavigate;
	struct HARDWARE 			hardware;
//	struct POSITION				position;
//	struct CONTROLSYSTEM 		controlsystem;
//	struct SCRIPTS				scripts;
};

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);


void Set_Transmit_Uart(struct UART *uart, int32_t *transmit);

void Set_Command_Uart(int32_t *originData, struct UART uart);

//void Get_Command( struct COMMAND cmd, struct CONTROLSYSTEM *sys, struct NAVIGATION *nav, struct SCRIPTS *scripts);

//void Set_PosNavigate(struct ROBOT *robot, struct NAVIGATION nav);

//void script_handler(struct ROBOT *robot);

#endif
