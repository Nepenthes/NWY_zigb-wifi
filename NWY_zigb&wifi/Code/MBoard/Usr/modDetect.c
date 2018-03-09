#include "modDetect.h"
#include "UART_dataTransfer.h"

osThreadId tid_modDetect_Thread;
osThreadDef(modDetect_Thread,osPriorityNormal,	1,	512);

void modDetect_Init(void){

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	PAout(0) = 0;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	PEout(5) = 0;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	PAout(11) = 0;
}

u8 portSlot_Detect(void){

	u8 pDet_VAL = 0;
	
	if(!DataTransP1_CHK)pDet_VAL |= 1 << 0;
	else pDet_VAL &= ~(1 << 0);
	
	if(!DataTransP2_CHK)pDet_VAL |= 1 << 1;
	else pDet_VAL &= ~(1 << 1);
	
	if(!DataTransP3_CHK)pDet_VAL |= 1 << 2;
	else pDet_VAL &= ~(1 << 2);
	
	return pDet_VAL;
}

void modDetect_Thread(const void *argument){
	
	u8 pDet_VAL_last = 0;
	u8 pDet_VAL = 0;
	bool ModTrig_flag = false;
	const u8 ModTrig_cfrmPeriod = 10;
	u8 ModTrig_count = 0;
	
	u8 loop;
	
	char disp[80] = {0};
	
	for(;;){
		
		if(pDet_VAL != portSlot_Detect()){
	
			ModTrig_flag = true;
		}
		
		if(ModTrig_flag == true && (pDet_VAL != portSlot_Detect())){
		
			if(ModTrig_count < ModTrig_cfrmPeriod){
			
				ModTrig_count ++;
			}else{
			
				u8 pDet_dif;
				
				ModTrig_flag = false;
				ModTrig_count = 0;
				
				pDet_VAL = portSlot_Detect();
				pDet_dif = pDet_VAL ^ pDet_VAL_last;
				pDet_VAL_last = pDet_VAL;
				
				for(loop = 0;loop < 3;loop ++){
				
					if(pDet_dif & 1 << loop){
						
						static type_DTMod Mod_Last[3] = {typeDT_NULL,typeDT_NULL,typeDT_NULL};
					
						switch(loop){
						
							case 0:{
							
								if(pDet_VAL & 1 << loop){
									
									switch(ModDEC_P1){
										
										case typeDT_ZIGB:{
										
											Mod_Last[0] = typeDT_ZIGB;
											
											memset(disp,0,80 * sizeof(char));
											sprintf(disp,"HW_Tips:P1卡槽内插入ZIGB模块\r\n");
											DbugP1TX(disp,strlen(disp));	
											osDelay(200);
											
											communicationActive(comObj_DataTransP1,ftOBJ_ZIGB);
										}break;
										
										case typeDT_WIFI:{
										
											Mod_Last[0] = typeDT_WIFI;
											
											memset(disp,0,80 * sizeof(char));
											sprintf(disp,"HW_Tips:P1卡槽内插入WIFI模块\r\n");
											DbugP1TX(disp,strlen(disp));	
											osDelay(200);
											
											communicationActive(comObj_DataTransP1,ftOBJ_WIFI);
										}break;
										
										case typeDT_NULL:
										
										default:break;
									}
									
									memset(disp,0,80 * sizeof(char));
									sprintf(disp,"HW_Tips:P1卡槽对应数据传输进程已激活，等待初始化完成...\r\n");
									DbugP1TX(disp,strlen(disp));	
									osDelay(500);
								}else{
								
									switch(Mod_Last[loop]){
									
										case typeDT_ZIGB:{
										
											memset(disp,0,80 * sizeof(char));
											sprintf(disp,"HW_Tips:P1卡槽内ZIGB模块已被拔除\r\n");
											DbugP1TX(disp,strlen(disp));	
											osDelay(20);
										}break;
										
										case typeDT_WIFI:{
										
											memset(disp,0,80 * sizeof(char));
											sprintf(disp,"HW_Tips:P1卡槽内WIFI模块已被拔除\r\n");
											DbugP1TX(disp,strlen(disp));	
											osDelay(20);
										}break;
										
										default:break;
									}
									
									osThreadTerminate(tid_com3DataTransP1_Thread);
									
									memset(disp,0,80 * sizeof(char));
									sprintf(disp,"HW_Tips:P1卡槽对应数据传输进程已终结\r\n");
									DbugP1TX(disp,strlen(disp));	
									osDelay(20);
								}
							}break;
							
							case 1:{
							
								if(pDet_VAL & 1 << loop){
									
									switch(ModDEC_P2){
										
										case typeDT_ZIGB:{
											
											Mod_Last[1] = typeDT_ZIGB;
										
											memset(disp,0,80 * sizeof(char));
											sprintf(disp,"HW_Tips:P2卡槽内插入ZIGB模块\r\n");
											DbugP1TX(disp,strlen(disp));	
											osDelay(20);
											
											osDelay(200);
											communicationActive(comObj_DataTransP2,ftOBJ_ZIGB);
										}break;
										
										case typeDT_WIFI:{
										
											Mod_Last[1] = typeDT_WIFI;
											
											memset(disp,0,80 * sizeof(char));
											sprintf(disp,"HW_Tips:P2卡槽内插入WIFI模块\r\n");
											DbugP1TX(disp,strlen(disp));	
											osDelay(20);
											
											osDelay(200);
											communicationActive(comObj_DataTransP2,ftOBJ_WIFI);
										}break;
										
										case typeDT_NULL:
											
										default:break;
									}
									
									osThreadTerminate(tid_com3DataTransP1_Thread);
									
									memset(disp,0,80 * sizeof(char));
									sprintf(disp,"HW_Tips:P2卡槽对应数据传输进程已激活，等待初始化完成...\r\n");
									DbugP1TX(disp,strlen(disp));	
									osDelay(500);
								}else{
								
									switch(Mod_Last[loop]){
									
										case typeDT_ZIGB:{
										
											memset(disp,0,80 * sizeof(char));
											sprintf(disp,"HW_Tips:P2卡槽内ZIGB模块已被拔除\r\n");
											DbugP1TX(disp,strlen(disp));	
											osDelay(20);
										}break;
										
										case typeDT_WIFI:{
										
											memset(disp,0,80 * sizeof(char));
											sprintf(disp,"HW_Tips:P2卡槽内WIFI模块已被拔除\r\n");
											DbugP1TX(disp,strlen(disp));	
											osDelay(20);
										}break;
										
										default:break;
									}
									
									memset(disp,0,80 * sizeof(char));
									sprintf(disp,"HW_Tips:P2卡槽对应数据传输进程已终结\r\n");
									DbugP1TX(disp,strlen(disp));	
									osDelay(20);
								}
							}break;
							
							case 2:{
							
								if(pDet_VAL & 1 << loop){
									
									switch(ModDEC_P3){
										
										case typeDT_ZIGB:{
											
											Mod_Last[2] = typeDT_ZIGB;
										
											memset(disp,0,80 * sizeof(char));
											sprintf(disp,"HW_Tips:P3卡槽内插入ZIGB模块\r\n");
											DbugP1TX(disp,strlen(disp));	
											osDelay(20);
											
											osDelay(200);
											communicationActive(comObj_DataTransP3,ftOBJ_ZIGB);
										}break;
										
										case typeDT_WIFI:{
											
											Mod_Last[2] = typeDT_WIFI;
										
											memset(disp,0,80 * sizeof(char));
											sprintf(disp,"HW_Tips:P3卡槽内插入WIFI模块\r\n");
											DbugP1TX(disp,strlen(disp));	
											osDelay(20);
											
											osDelay(200);
											communicationActive(comObj_DataTransP3,ftOBJ_WIFI);
										}break;
										
										case typeDT_NULL:
										
										default:break;
									}
									
									memset(disp,0,80 * sizeof(char));
									sprintf(disp,"HW_Tips:P3卡槽对应数据传输进程已激活，等待初始化完成...\r\n");
									DbugP1TX(disp,strlen(disp));	
									osDelay(500);
								}else{
								
									switch(Mod_Last[loop]){
									
										case typeDT_ZIGB:{
										
											memset(disp,0,80 * sizeof(char));
											sprintf(disp,"HW_Tips:P3卡槽内ZIGB模块已被拔除\r\n");
											DbugP1TX(disp,strlen(disp));	
											osDelay(20);
										}break;
										
										case typeDT_WIFI:{
										
											memset(disp,0,80 * sizeof(char));
											sprintf(disp,"HW_Tips:P3卡槽内WIFI模块已被拔除\r\n");
											DbugP1TX(disp,strlen(disp));	
											osDelay(20);
										}break;
										
										default:break;
									}
									
									osThreadTerminate(tid_com3DataTransP1_Thread);
									
									memset(disp,0,80 * sizeof(char));
									sprintf(disp,"HW_Tips:P3卡槽对应数据传输进程已终结\r\n");
									DbugP1TX(disp,strlen(disp));	
									osDelay(20);
								}
							}break;
							
							default:break;
						}
					}else{
					
						;
					}
				}	
			}
		}else{
	
			ModTrig_flag = false;
			ModTrig_count = 0;
		}	
		
		osDelay(100);
	}
}

void threadModDetect_Active(void){

	modDetect_Init();
	tid_modDetect_Thread = osThreadCreate(osThread(modDetect_Thread),NULL);
}

