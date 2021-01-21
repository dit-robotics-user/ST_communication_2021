#ifndef __HARDWARE_H
#define __HARDWARE_H

/*  In hardware.h & hardware.c define:
  Components of the robot
  Geometry constants of the robot
  Pin & Timer connection of circuit
  Basic operation of components         */

#include "gpio.h"
#include "tim.h"
#include "crc.h"
#include "usart.h"

//---------------------------------------------------------------------------
/*
struct MONSTERMOTOR
{
 GPIO_TypeDef    *Cw_port;
 uint16_t      Cw_pin;
 GPIO_TypeDef    *Ccw_port;
 uint16_t      Ccw_pin;
 TIM_HandleTypeDef *Timer;
 uint32_t      Channel;
};
void MonsterMotor_Init(struct MONSTERMOTOR* motor);
void MonsterMotor_Write(struct MONSTERMOTOR* motor, double duty);
void MonsterMotor_Stop(struct MONSTERMOTOR* motor);
*/
//---------------------------------------------------------------------------
/*
struct ENCODER
{
 TIM_HandleTypeDef *Timer;
 uint32_t      Channel1;
 uint32_t      Channel2;
 double      Radius;
 double      ReductionRatio;
 double       Resolution;
 short       Direction;     // +1 or -1
};
void Encoder_Init_16bit(struct ENCODER *encoder);
void Encoder_Init_32bit(struct ENCODER *encoder);
unsigned int Encoder_Read(struct ENCODER *encoder);
*/


//---------------------------------------------------------------------------
/*
struct SWITCH
{
 GPIO_TypeDef   *port;
 uint16_t    pin;
};
short Switch_Read(struct SWITCH sw);    // 1 short(on), 0 open(off)
*/

//---------------------------------------------------------------------------
struct UART
{
 UART_HandleTypeDef  *huart;
 uint8_t    tx_length, rx_length, start, flag, single_count, reset_count;   // count as 32-bits, without crc
 uint32_t   trans_count, recev_count, error_count;
 int8_t   reset_check;
 double    recev_suc_rate;
 volatile int32_t tx[20];
 volatile int32_t rx[20];
 volatile int32_t buf[20];
 volatile int32_t checked_rx[20];
 volatile int32_t    rx_single[2];
 volatile int32_t    rx_temp[4];
};
void Uart_Transmit(struct UART* uart);
void Uart_Rate_Count(struct UART* uart);
short Uart_Crc_Check(struct UART* uart);
void Uart_RxCplt(UART_HandleTypeDef *huart, struct UART *uart);


//---------------------------------------------------------------------------
struct HARDWARE
{
// struct MONSTERMOTOR  motorR;
// struct MONSTERMOTOR  motorL;
// struct ENCODER    encoderIR;
// struct ENCODER    encoderIL;
// struct ENCODER    encoderOR;
// struct ENCODER    encoderOL;
// struct SWITCH    switchReposition;
 struct UART    uartRPI;
};
void Hardware_Init(struct HARDWARE *hardware);
void Hardware_Mapping(struct HARDWARE *hardware);




#endif
