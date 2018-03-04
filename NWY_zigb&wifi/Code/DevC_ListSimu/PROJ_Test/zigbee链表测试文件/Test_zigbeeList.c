#include <Test_zigbeeList.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

#define u8 		uint8_t
#define u16		uint16_t

typedef unsigned char uint8_t;
typedef unsigned int  uint16_t;

/*zigbee�������ݽṹ*/ 
typedef struct ZigB_nwkState_Form{

	u16	nwkAddr;
	u8	psyAddr;
	u8	extAddr;
	u8	onlineDectect_LCount;
	
	struct ZigB_nwkState_Form *next;
}nwkStateAttr_Zigb;

/*zigbee������֤��ָ������ݽṹ*/ 
typedef struct cmdString{
	
	char content[30];
	u8	 cmpLen;
}cmdStr;

void *memmem(void *start, unsigned int s_len, void *find,unsigned int f_len){
	
	char *p, *q;
	unsigned int len;
	p = start, q = find;
	len = 0;
	while((p - (char *)start + f_len) <= s_len){
			while(*p++ == *q++){
					len++;
					if(len == f_len)
							return(p - f_len);
			};
			q = find;
			len = 0;
	};
	return(NULL);
}

int strloc(u8 str2[],u8 num_s2,u8 str1[],u8 num_s1)
{
	int la = num_s1;
	int i, j;
	int lb = num_s2;
	for(i = 0; i < lb; i ++)
	{
		for(j = 0; j < la && i + j < lb && str1[j] == str2[i + j]; j ++);
		if(j == la)return i;
	}
	return -1;
}

/*zigbee�����ظ��ڵ�ȥ���Ż�*/ 
void delSame(nwkStateAttr_Zigb *head)	//�����ַ �� �����ַ ȥ�� 
{
    nwkStateAttr_Zigb *p,*q,*r;
    p = head->next; // ��������ͷ�ڵ�ĵ��������ڲ���ͷ�ڵ�ĵ������˴���Ϊ p��head ���ɡ�
    while(p != NULL)    // p���ڱ�������
    {
        q = p;
        while(q->next != NULL) // q����p����Ľ�㣬����p��ֵ�Ƚ�
        {
            if(q->next->nwkAddr == p->nwkAddr || (q->next->psyAddr == p->psyAddr && q->next->psyAddr == p->extAddr))	   
            {
                r = q->next; // r������Ҫɾ���Ľ��
                q->next = r->next;   // ��Ҫɾ���Ľ���ǰ�������
                free(r);
            }
            else
                q = q->next;
        }

        p = p->next;
    }
}

/*zigbee�����β�����ڵ�*/ 
u8 eptCreat(nwkStateAttr_Zigb *pHead,nwkStateAttr_Zigb pNew){
	
	nwkStateAttr_Zigb *pAbove = pHead;
	nwkStateAttr_Zigb *pFollow;
	u8 nCount = 0;
	
	nwkStateAttr_Zigb *pNew_temp =  (nwkStateAttr_Zigb *)malloc(1 * sizeof(nwkStateAttr_Zigb));
	pNew_temp->nwkAddr 				= pNew.nwkAddr;
	pNew_temp->psyAddr 				= pNew.psyAddr;
	pNew_temp->extAddr 				= pNew.extAddr;
	pNew_temp->onlineDectect_LCount = pNew.onlineDectect_LCount;
	pNew_temp->next	   = NULL;
	
	while(pAbove->next != NULL){
		
		nCount ++;
		pFollow = pAbove;
		pAbove	= pFollow->next;
	}
	
	pAbove->next = pNew_temp;
	return ++nCount;
}

/*zigbee����ڵ���Ϣ��������������ַ*/ 
nwkStateAttr_Zigb *eptPutout_BYnwk(nwkStateAttr_Zigb *pHead,u16 nwkAddr,bool method){
	
	nwkStateAttr_Zigb *pAbove = pHead;
	nwkStateAttr_Zigb *pFollow;
	
	nwkStateAttr_Zigb *pTemp = (nwkStateAttr_Zigb *)malloc(1 * sizeof(nwkStateAttr_Zigb));
	pTemp->next = NULL;
	
	while(!(pAbove->nwkAddr == nwkAddr) && pAbove->next != NULL){
		
		pFollow = pAbove;
		pAbove	= pFollow->next;
	}
	
	if(pAbove->nwkAddr == nwkAddr){
		
		if(!method){
			
			pTemp->nwkAddr 				= pAbove->nwkAddr;
			pTemp->psyAddr 				= pAbove->psyAddr;
			pTemp->extAddr 				= pAbove->extAddr;
			pTemp->onlineDectect_LCount = pAbove->onlineDectect_LCount;
		}else{
			
			pTemp = pAbove;	
		}
		
		return pTemp;
	}else{
		
		free(pTemp);
		return NULL;
	}	
} 

/*zigbee����ڵ��������������PCB�����ַ*/ 
nwkStateAttr_Zigb *eptPutout_BYpsy(nwkStateAttr_Zigb *pHead,u8 psyAddr,u8 extAddr,bool method){	//method = true,��ֱ�ӷ��ؽڵ��ַ�����������Ӱ�쵽ԭ�ڵ���Ϣ�� method = false,���ش�ԭ�ڵ�ӳ����Ϣ�ĵ�ַ�����������Ӱ��Դ�ڵ� 
	
	nwkStateAttr_Zigb *pAbove = pHead;
	nwkStateAttr_Zigb *pFollow;
	
	nwkStateAttr_Zigb *pTemp = (nwkStateAttr_Zigb *)malloc(1 * sizeof(nwkStateAttr_Zigb));
	pTemp->next = NULL;
	
	while(!(pAbove->psyAddr == psyAddr && pAbove->extAddr == extAddr) && pAbove->next != NULL){
		
		pFollow = pAbove;
		pAbove	= pFollow->next;
	}
	
	if(pAbove->psyAddr == psyAddr && pAbove->extAddr == extAddr){
		
		if(!method){
			
			pTemp->nwkAddr 				= pAbove->nwkAddr;
			pTemp->psyAddr 				= pAbove->psyAddr;
			pTemp->extAddr 				= pAbove->extAddr;
			pTemp->onlineDectect_LCount = pAbove->onlineDectect_LCount;
		}else{
			
			pTemp = pAbove;	
		}
		
		return pTemp;
	}else{
		
		free(pTemp);
		return NULL;
	}	
}

/*zigbee����ڵ�ɾ�������������ַ*/ 
bool eptRemove_BYnwk(nwkStateAttr_Zigb *pHead,u16 nwkAddr){
	
	nwkStateAttr_Zigb *pAbove = pHead;
	nwkStateAttr_Zigb *pFollow;
	
	nwkStateAttr_Zigb *pTemp;
	
	while(!(pAbove->nwkAddr == nwkAddr) && pAbove->next != NULL){
		
		pFollow = pAbove;
		pAbove	= pFollow->next;
	}
	
	if(pAbove->nwkAddr == nwkAddr){
		
		 pTemp = pAbove;
		 pFollow->next = pAbove->next;
		 free(pTemp);
		 return true;
	}else{
		
		return false;
	}
}

/*zigbee����ڵ�ɾ��������PCB�����ַ*/ 
bool eptRemove_BYpsy(nwkStateAttr_Zigb *pHead,u16 psyAddr,u16 extAddr){
	
	nwkStateAttr_Zigb *pAbove = pHead;
	nwkStateAttr_Zigb *pFollow;
	
	nwkStateAttr_Zigb *pTemp;
	
	while(!(pAbove->psyAddr == psyAddr && pAbove->extAddr == extAddr) && pAbove->next != NULL){
		
		pFollow = pAbove;
		pAbove	= pFollow->next;
	}
	
	if(pAbove->psyAddr == psyAddr && pAbove->extAddr == extAddr){
		
		 pTemp = pAbove;
		 pFollow->next = pAbove->next;
		 free(pTemp);
		 return true;
	}else{
		
		return false;
	}
}

/*zigbee�����Ȳ���*/ 
u8 chatLenDectect(nwkStateAttr_Zigb *pHead){
	
	nwkStateAttr_Zigb *pAbove = pHead;
	nwkStateAttr_Zigb *pFollow;
	u8 loop;
	
	while(pAbove->next != NULL){
		
		loop ++;
		pFollow = pAbove;
		pAbove	= pFollow->next;
	}

	return loop;
}

void dispList(nwkStateAttr_Zigb *pHead){
	
	nwkStateAttr_Zigb *Disp = pHead;
	u8 loop;
	
	if(0 == chatLenDectect(pHead)){
		
		printf("��ǰ�����ڽڵ�����Ϊ�㣬������\n"); 
		return;
	}
	
	printf("������Ϣ���������ʼ\n===================================================\n"); 
	while(Disp->next != NULL){
		
		loop ++;
		
		Disp = Disp->next;
		printf("�ڵ�%02d��Ϣ�����          |\n�����ַ��0x%04X          |\n�����ַ��%02X              |\n��չ��ַ��%02X              |\n���߼�⵹��ʱ��%02X        |\n",
				loop,Disp->nwkAddr,Disp->psyAddr,Disp->extAddr,Disp->onlineDectect_LCount);
		printf("__________________________|��\n"); 
	}
	printf("===================================================\n��������\n"); 
}

void funValidation(void){
	
	#define cmdALen 9 
	const cmdStr strtemp[cmdALen] = {
	
		{"help",4},
		{"�ڵ�����",8},
		{"���������ַɾ���ڵ�",20},
		{"���������ַɾ���ڵ�",20},
		{"���������ַ����ڵ���Ϣ",20},
		{"���������ַ����ڵ���Ϣ",20},
		{"���������ַ���Ľڵ���Ϣ",20},
		{"�����Ż��ظ�",12},
		{"��������",8},	
	}; 
	
	#define cmdLen 60
	char str[cmdLen] = {0};
	nwkStateAttr_Zigb temp;
	nwkStateAttr_Zigb *Disp;
	nwkStateAttr_Zigb *Head = (nwkStateAttr_Zigb *)malloc(1 * sizeof(nwkStateAttr_Zigb));
	u8 pLen = 0; 
	u8 loop = 0; 
	
	Head->next = NULL;
	
	while(1){
		
		memset(str,0,cmdLen * sizeof(char));
		
		printf("������ָ��\r\n");
		scanf("%s",str);
//		temp.nwkAddr = temp.psyAddr = temp.extAddr = 0;
		for(loop = 0;loop < cmdALen;loop ++){
			
			if(!memcmp(strtemp[loop].content,str,strtemp[loop].cmpLen - 1)){
				
				break;
			}
		}printf("��ȡ��ǰָ����Ϊ��%d\r\n",loop);
		
		if(loop < cmdALen)printf("ָ��Ϸ�\r\n");
		else{
		
			printf("ָ��Ƿ�������������\r\n");
			continue; 
		} 
		
		if(loop != 0 && loop != 7 && loop != 8){
			
			printf("������ָ���Ӧ����\r\n");
			scanf("%d,%d,%d,%d",&(temp.nwkAddr),&(temp.psyAddr),&(temp.extAddr),&(temp.onlineDectect_LCount));
	//		printf("%s,0x%04X,%d,%d",str,temp.nwkAddr,temp.psyAddr,temp.extAddr);
	
			if(temp.nwkAddr == 0
			   || temp.psyAddr == 0){
			   	
					printf("���ݷǷ�\r\n");
					continue; 
			   }
		}
		
		switch(loop){
				
			case 0:{
				
				printf("ʹ�÷�ʽ����������ָ�������ָ���Ӧ������\n-------------------------------------------------------\nָ���򵼣�\n");
				printf("ָ����01���ڵ�����\n");
				printf("ָ����02�����������ַɾ���ڵ�\n");
				printf("ָ����03�����������ַɾ���ڵ�\n");
				printf("ָ����04�����������ַ����ڵ���Ϣ\n");
				printf("ָ����05�����������ַ����ڵ���Ϣ\n");
				printf("ָ����06�����������ַ���Ľڵ���Ϣ\n");
				printf("ָ����07�������Ż��ظ�\n");
				printf("ָ����08����������\n");
			}break;
			
			case 1:{
				
				pLen = eptCreat(Head,temp);
				printf("�����ڵ����\r\n��ǰ����ڵ�����Ŀ��%d\r\n\r\n\r\n",pLen);
			}break;
			
			case 2:{
				
				const u16 nwkAddr_S = temp.nwkAddr;
				
				if(eptRemove_BYnwk(Head,nwkAddr_S)){
					
					pLen = chatLenDectect(Head);
					printf("�ڵ�ɾ�����\r\n��ǰ����ʣ��ڵ�����Ŀ��%d\r\n\r\n\r\n",pLen);
				}else{
					
					printf("�Բ���δѰ�ö�Ӧ�����ַΪ0x%04X������ڵ㣬�ڵ�ɾ��ʧ��\r\n\r\n",nwkAddr_S);
				}
			}break;
			
			case 3:{
				
				const u16 psyAddr_S = temp.psyAddr;
				const u16 extAddr_S = temp.extAddr;
				
				if(eptRemove_BYpsy(Head,psyAddr_S,extAddr_S)){
					
					pLen = chatLenDectect(Head);
					printf("�ڵ�ɾ�����\r\n��ǰ����ʣ��ڵ�����Ŀ��%d\r\n\r\n\r\n",pLen);
				}else{
					
					printf("�Բ���δѰ�ö�Ӧ�����ַΪ0x%04X������ڵ㣬�ڵ�ɾ��ʧ��\r\n\r\n",psyAddr_S);
				}
			}break;
			
			case 4:{
				
				const u16 nwkAddr_S = temp.nwkAddr;
				
				Disp = (nwkStateAttr_Zigb *)eptPutout_BYnwk(Head,nwkAddr_S,1);
				
				if(NULL != Disp){
						
					printf("�ڵ���Ϣ�����\r\n�����ַ��0x%04X\n�����ַ��%02X\n��չ��ַ��%02X\n���߼�⵹��ʱ��%02X",Disp->nwkAddr,Disp->psyAddr,Disp->extAddr,Disp->onlineDectect_LCount);
					Disp = NULL;	//��ȫ����������������Ⱦ 
					free(Disp);	
				}else{
					
					printf("�Բ���δѰ�ö�Ӧ�����ַΪ0x%04X������ڵ㣬�ڵ���Ϣ���ʧ��\r\n\r\n",nwkAddr_S);
				}
			}break;
			
			case 5:{
				
				const u16 psyAddr_S = temp.psyAddr;
				const u16 extAddr_S = temp.extAddr;
				
				Disp = (nwkStateAttr_Zigb *)eptPutout_BYpsy(Head,psyAddr_S,extAddr_S,1);
				
				if(NULL != Disp){
						
					printf("�ڵ���Ϣ�����\r\n�����ַ��0x%04X\n�����ַ��%02X\n��չ��ַ��%02X\n���߼�⵹��ʱ��%02X",Disp->nwkAddr,Disp->psyAddr,Disp->extAddr,Disp->onlineDectect_LCount);
					Disp = NULL;	//��ȫ����������������Ⱦ 
					free(Disp);	
				}else{
					
					printf("�Բ���δѰ�ö�Ӧ�����ַΪ0x%02X������ڵ㣬�ڵ���Ϣ���ʧ��\r\n\r\n",psyAddr_S);
				}
			}break;
			
			case 6:{
				
				const u16 psyAddr_S = temp.psyAddr;
				const u16 extAddr_S = temp.extAddr;
				
				Disp = (nwkStateAttr_Zigb *)eptPutout_BYpsy(Head,psyAddr_S,extAddr_S,1);
				
				if(NULL != Disp){
						
					printf("�ڵ㵱ǰ��Ϣ�����\r\n�����ַ��0x%04X\n�����ַ��%02X\n��չ��ַ��%02X\n���߼�⵹��ʱ��%02X\n",Disp->nwkAddr,Disp->psyAddr,Disp->extAddr,Disp->onlineDectect_LCount);
					printf("���������ĸ�����Ϣ���ýڵ����ݽ��ᱻ�����ݸ���\n");
					scanf("%d,%d,%d,%d",&(Disp->nwkAddr),&(Disp->psyAddr),&(Disp->extAddr),&(Disp->onlineDectect_LCount));
					printf("�ڵ����ݸ������\n");
					Disp = NULL;	//��ȫ����������������Ⱦ 
					free(Disp);	
				}else{
					
					printf("�Բ���δѰ�ö�Ӧ�����ַΪ0x%02X������ڵ㣬�ڵ���Ϣ���ʧ��\r\n\r\n",psyAddr_S);
				}
			}break;
		
			case 7:{
				
				delSame(Head);
				pLen = chatLenDectect(Head);
				
				printf("�����ظ��ڵ��ѱ��Ż�������ǰ���ȣ�%d\n",pLen);
			}break;
			
			case 8:{
				
				dispList(Head);
			}break;
			

			
			default:printf("�Բ������������޷�ʶ��\r\n\r\n");break; 
		}printf("\n\n"); 
	}
}


