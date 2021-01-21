//#include "hardware.h"
//int reset_count;
//
//void Hardware_Init(struct HARDWARE *hardware)
//{
//	Hardware_Mapping(hardware);
//	HAL_UART_Receive_DMA(hardware->uartRPI.huart, (uint8_t *)hardware->uartRPI.rx, 4*(hardware->uartRPI.rx_length)+4);
////	HAL_UART_Receive_DMA(hardware->uartRPI.huart, (uint8_t *)hardware->uartRPI.rx_temp, 1);
////	hardware->uartRPI.shift = 0;
//}
//
//void Hardware_Mapping(struct HARDWARE *hardware)
//{
//	hardware->uartRPI.huart = &huart1;
//	hardware->uartRPI.tx_length = 6;
//	hardware->uartRPI.rx_length = 6;
//	hardware->uartRPI.trans_count = 0;
//	hardware->uartRPI.recev_count = 0;
//	hardware->uartRPI.error_count = 0;
//	hardware->uartRPI.start = 1;
//	hardware->uartRPI.tx[hardware->uartRPI.tx_length + 2] = 0x3F3E0000 ;  //0x0A000000; // '\n\0\0\0'
//	hardware->uartRPI.tx[hardware->uartRPI.tx_length + 1] = 0; // flag
//}
//
//void Uart_Transmit(struct UART* uart)
//{
//	uart->tx[uart->tx_length] = HAL_CRC_Calculate(&hcrc, (uint32_t *)uart->tx, uart->tx_length);
//	HAL_UART_Transmit_DMA(uart->huart, (uint8_t *)uart->tx, 4*(uart->tx_length)+12);
//	uart->trans_count ++;
//}
//
////void Uart_Rate_Count(struct UART* uart)
////{
////	if(uart->trans_count == 1000){
////		uart->recev_suc_rate = ((double)100*(uart->recev_count - uart->error_count))/(uart->recev_count);
////		uart->recev_count = 0;
////		uart->error_count = 0;
////		uart->trans_count = 0;
////	}
////}
//
//short Uart_Crc_Check(struct UART* uart)
//{
//	for(int i=0; i<(uart->rx_length)+1; i++){
//		uart->buf[i] = uart->rx[i];
//	}
//
//	if(uart->rx[uart->rx_length] == HAL_CRC_Calculate(&hcrc, (uint32_t *)uart->buf, uart->rx_length)){
//		for(int i=0; i<(uart->rx_length)+1; i++)
//			uart->checked_rx[i] = uart->buf[i];
//		return 1; //crc success
//	}
//	return 0; //crc fail
//}
//
//void Uart_RxCplt(UART_HandleTypeDef *huart, struct UART* uart)
//{
//	if(huart == uart->huart){
//		if(uart->start){
//			uart->recev_count ++;
//			if(!Uart_Crc_Check(uart)){
//				reset_count++;
//				uart->error_count ++;
//				if(uart->error_count %10 == 9){ //%2
//					HAL_UART_AbortReceive(uart->huart);
//					uart->start = 0;
//					HAL_UART_Receive_DMA(uart->huart, &uart->reset_check, 1);
//				}
//			}
//		}
//		else{ // reset receive
//			if(uart->reset_check == 0x31){ // ETB
//				 uart->tx[uart->tx_length + 1] = 255;
//				 HAL_UART_AbortReceive(uart->huart);
//			}
//			else{
//				HAL_UART_AbortReceive(uart->huart);
//				HAL_UART_Receive_DMA(uart->huart, &uart->reset_check, 1);
//			}
//		}
//	}
//}



/* following is new code */
#include "hardware.h"

extern int p;
void Hardware_Init(struct HARDWARE *hardware)
{
	Hardware_Mapping(hardware);
//	HAL_UART_Receive_DMA(hardware->uartRPI.huart, (uint8_t *)hardware->uartRPI.rx, 4*(hardware->uartRPI.rx_length)+8);
	HAL_UART_Receive_DMA(hardware->uartRPI.huart, (uint8_t *)hardware->uartRPI.rx_single, 4);
	hardware->uartRPI.flag = 0;
	hardware->uartRPI.single_count = 0;
	//	HAL_UART_Receive(hardware->uartRPI.huart, (uint8_t *)hardware->uartRPI.rx, 4*(hardware->uartRPI.rx_length)+8, 10); //
	hardware->uartRPI.reset_count = 0;
}

void Hardware_Mapping(struct HARDWARE *hardware)
{
	hardware->uartRPI.huart = &huart1;
	hardware->uartRPI.tx_length = 6;
	hardware->uartRPI.rx_length = 6;
	hardware->uartRPI.trans_count = 0;
	hardware->uartRPI.recev_count = 0;
	hardware->uartRPI.error_count = 0;
	hardware->uartRPI.start = 1;
	hardware->uartRPI.tx[hardware->uartRPI.tx_length + 2] = 0x3F3E0000 ;  //0x0A000000; // '\n\0\0\0'
	hardware->uartRPI.tx[hardware->uartRPI.tx_length + 1] = 0; // flag
}

void Uart_Transmit(struct UART* uart)
{
	uart->tx[uart->tx_length] = HAL_CRC_Calculate(&hcrc, (uint32_t *)uart->tx, uart->tx_length);
	HAL_UART_Transmit_DMA(uart->huart, (uint8_t *)uart->tx, 4*(uart->tx_length)+12);
	uart->trans_count ++;
	if(uart->trans_count >= 200 && p == 0){
		HAL_UART_AbortReceive(uart->huart);
		HAL_UART_Receive_DMA(uart->huart, (uint8_t *)uart->rx_single, 4);
	}

}

void Uart_Rate_Count(struct UART* uart)
{
	//if(uart->trans_count >= 100){//
	if(uart->trans_count == 1000){
		uart->recev_suc_rate = ((double)100*(uart->recev_count - uart->error_count))/(uart->recev_count);
		uart->recev_count = 0;
		uart->error_count = 0;
		uart->trans_count = 0;
	}
}

short Uart_Crc_Check(struct UART* uart)
{
	for(int i=0; i<(uart->rx_length)+1; i++){
		uart->buf[i] = uart->rx[i];
	}

	if(uart->rx[uart->rx_length] == HAL_CRC_Calculate(&hcrc, (uint32_t *)uart->buf, uart->rx_length)){
		for(int i=0; i<(uart->rx_length)+1; i++)
			uart->checked_rx[i] = uart->buf[i];
		return 1; //crc success
	}
	return 0; //crc fail
}

void Uart_RxCplt(UART_HandleTypeDef *huart, struct UART* uart)
{
	if(huart == uart->huart){
		uart->recev_count ++;
		if(uart->flag == 0){
			if(uart->single_count == 0 && uart->rx_single[0] != 0x31 && uart->reset_count < 20){
				uart->error_count ++;
				uart->reset_count ++;
				return;
			}
			else if(uart->single_count == 0 && uart->rx_single[0] != 0x31 && uart->reset_count == 20){
				uart->reset_count = 0;
				HAL_UART_AbortReceive(uart->huart);
//				HAL_UART_Receive_DMA(uart->huart, (uint8_t *)&uart->reset_check, 1);
//				if(uart->reset_check == -0x17){ // ETB
					 uart->tx[uart->tx_length + 1] = 255;
//					 HAL_UART_AbortReceive(uart->huart);
//				}
				return;
			}
			uart->rx[uart->single_count] = uart->rx_single[0];
			if(uart->single_count == uart->rx_length){
				uart->single_count = 0;
				if(Uart_Crc_Check(uart)){
					HAL_UART_AbortReceive(uart->huart);
//					HAL_UART_Receive_DMA(hardware->uartRPI.huart, (uint8_t *)hardware->uartRPI.rx, 4*(hardware->uartRPI.rx_length)+8);
					HAL_UART_Receive_DMA(uart->huart, (uint8_t *)uart->rx, 4*(uart->rx_length)+4);
					uart->flag = 1;
				}
				else{
					uart->error_count ++;
				}
			}
			else uart->single_count++;
		}
		else if(uart->flag == 1){
			if(!Uart_Crc_Check(uart)){
				uart->flag = 0;
				HAL_UART_AbortReceive(uart->huart);
				HAL_UART_Receive_DMA(uart->huart, (uint8_t *)uart->rx_single, 4);
				uart->error_count ++;
			}
		}
	}
}


