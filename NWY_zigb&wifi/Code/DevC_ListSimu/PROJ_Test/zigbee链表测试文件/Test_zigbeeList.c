#include <Test_zigbeeList.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

#define u8 		uint8_t
#define u16		uint16_t

typedef unsigned char uint8_t;
typedef unsigned int  uint16_t;

/*zigbee链表数据结构*/ 
typedef struct ZigB_nwkState_Form{

	u16	nwkAddr;
	u8	psyAddr;
	u8	extAddr;
	u8	onlineDectect_LCount;
	
	struct ZigB_nwkState_Form *next;
}nwkStateAttr_Zigb;

/*zigbee用于验证的指令表数据结构*/ 
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

/*zigbee链表重复节点去重优化*/ 
void delSame(nwkStateAttr_Zigb *head)	//网络地址 或 物理地址 去重 
{
    nwkStateAttr_Zigb *p,*q,*r;
    p = head->next; // 适用于有头节点的单链表；对于不带头节点的单链表，此处改为 p＝head 即可。
    while(p != NULL)    // p用于遍历链表
    {
        q = p;
        while(q->next != NULL) // q遍历p后面的结点，并与p数值比较
        {
            if(q->next->nwkAddr == p->nwkAddr || (q->next->psyAddr == p->psyAddr && q->next->psyAddr == p->extAddr))	   
            {
                r = q->next; // r保存需要删掉的结点
                q->next = r->next;   // 需要删掉的结点的前后结点相接
                free(r);
            }
            else
                q = q->next;
        }

        p = p->next;
    }
}

/*zigbee链表表尾新增节点*/ 
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

/*zigbee链表节点信息输出，根据网络地址*/ 
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

/*zigbee链表节点数据输出，根据PCB物理地址*/ 
nwkStateAttr_Zigb *eptPutout_BYpsy(nwkStateAttr_Zigb *pHead,u8 psyAddr,u8 extAddr,bool method){	//method = true,将直接返回节点地址，对其操作将影响到原节点信息； method = false,返回带原节点映射信息的地址，对其操作不影响源节点 
	
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

/*zigbee链表节点删除，根据网络地址*/ 
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

/*zigbee链表节点删除，根据PCB物理地址*/ 
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

/*zigbee链表长度测量*/ 
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
		
		printf("当前链表内节点数量为零，无数据\n"); 
		return;
	}
	
	printf("链表信息遍历输出开始\n===================================================\n"); 
	while(Disp->next != NULL){
		
		loop ++;
		
		Disp = Disp->next;
		printf("节点%02d信息输出：          |\n网络地址：0x%04X          |\n物理地址：%02X              |\n扩展地址：%02X              |\n在线检测倒计时：%02X        |\n",
				loop,Disp->nwkAddr,Disp->psyAddr,Disp->extAddr,Disp->onlineDectect_LCount);
		printf("__________________________|★\n"); 
	}
	printf("===================================================\n遍历结束\n"); 
}

void funValidation(void){
	
	#define cmdALen 9 
	const cmdStr strtemp[cmdALen] = {
	
		{"help",4},
		{"节点新增",8},
		{"根据网络地址删除节点",20},
		{"根据物理地址删除节点",20},
		{"根据网络地址输出节点信息",20},
		{"根据物理地址输出节点信息",20},
		{"根据物理地址更改节点信息",20},
		{"链表优化重复",12},
		{"遍历链表",8},	
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
		
		printf("请输入指令\r\n");
		scanf("%s",str);
//		temp.nwkAddr = temp.psyAddr = temp.extAddr = 0;
		for(loop = 0;loop < cmdALen;loop ++){
			
			if(!memcmp(strtemp[loop].content,str,strtemp[loop].cmpLen - 1)){
				
				break;
			}
		}printf("获取当前指令编号为：%d\r\n",loop);
		
		if(loop < cmdALen)printf("指令合法\r\n");
		else{
		
			printf("指令非法，请重新输入\r\n");
			continue; 
		} 
		
		if(loop != 0 && loop != 7 && loop != 8){
			
			printf("请输入指令对应数据\r\n");
			scanf("%d,%d,%d,%d",&(temp.nwkAddr),&(temp.psyAddr),&(temp.extAddr),&(temp.onlineDectect_LCount));
	//		printf("%s,0x%04X,%d,%d",str,temp.nwkAddr,temp.psyAddr,temp.extAddr);
	
			if(temp.nwkAddr == 0
			   || temp.psyAddr == 0){
			   	
					printf("数据非法\r\n");
					continue; 
			   }
		}
		
		switch(loop){
				
			case 0:{
				
				printf("使用方式：首先输入指令，后输入指令对应的数据\n-------------------------------------------------------\n指令向导：\n");
				printf("指令编号01：节点新增\n");
				printf("指令编号02：根据网络地址删除节点\n");
				printf("指令编号03：根据物理地址删除节点\n");
				printf("指令编号04：根据网络地址输出节点信息\n");
				printf("指令编号05：根据物理地址输出节点信息\n");
				printf("指令编号06：根据物理地址更改节点信息\n");
				printf("指令编号07：链表优化重复\n");
				printf("指令编号08：遍历链表\n");
			}break;
			
			case 1:{
				
				pLen = eptCreat(Head,temp);
				printf("新增节点完成\r\n当前链表节点总数目：%d\r\n\r\n\r\n",pLen);
			}break;
			
			case 2:{
				
				const u16 nwkAddr_S = temp.nwkAddr;
				
				if(eptRemove_BYnwk(Head,nwkAddr_S)){
					
					pLen = chatLenDectect(Head);
					printf("节点删除完成\r\n当前链表剩余节点总数目：%d\r\n\r\n\r\n",pLen);
				}else{
					
					printf("对不起，未寻得对应网络地址为0x%04X的链表节点，节点删除失败\r\n\r\n",nwkAddr_S);
				}
			}break;
			
			case 3:{
				
				const u16 psyAddr_S = temp.psyAddr;
				const u16 extAddr_S = temp.extAddr;
				
				if(eptRemove_BYpsy(Head,psyAddr_S,extAddr_S)){
					
					pLen = chatLenDectect(Head);
					printf("节点删除完成\r\n当前链表剩余节点总数目：%d\r\n\r\n\r\n",pLen);
				}else{
					
					printf("对不起，未寻得对应物理地址为0x%04X的链表节点，节点删除失败\r\n\r\n",psyAddr_S);
				}
			}break;
			
			case 4:{
				
				const u16 nwkAddr_S = temp.nwkAddr;
				
				Disp = (nwkStateAttr_Zigb *)eptPutout_BYnwk(Head,nwkAddr_S,1);
				
				if(NULL != Disp){
						
					printf("节点信息输出：\r\n网络地址：0x%04X\n物理地址：%02X\n扩展地址：%02X\n在线检测倒计时：%02X",Disp->nwkAddr,Disp->psyAddr,Disp->extAddr,Disp->onlineDectect_LCount);
					Disp = NULL;	//安全操作，避免链表被污染 
					free(Disp);	
				}else{
					
					printf("对不起，未寻得对应网络地址为0x%04X的链表节点，节点信息输出失败\r\n\r\n",nwkAddr_S);
				}
			}break;
			
			case 5:{
				
				const u16 psyAddr_S = temp.psyAddr;
				const u16 extAddr_S = temp.extAddr;
				
				Disp = (nwkStateAttr_Zigb *)eptPutout_BYpsy(Head,psyAddr_S,extAddr_S,1);
				
				if(NULL != Disp){
						
					printf("节点信息输出：\r\n网络地址：0x%04X\n物理地址：%02X\n扩展地址：%02X\n在线检测倒计时：%02X",Disp->nwkAddr,Disp->psyAddr,Disp->extAddr,Disp->onlineDectect_LCount);
					Disp = NULL;	//安全操作，避免链表被污染 
					free(Disp);	
				}else{
					
					printf("对不起，未寻得对应物理地址为0x%02X的链表节点，节点信息输出失败\r\n\r\n",psyAddr_S);
				}
			}break;
			
			case 6:{
				
				const u16 psyAddr_S = temp.psyAddr;
				const u16 extAddr_S = temp.extAddr;
				
				Disp = (nwkStateAttr_Zigb *)eptPutout_BYpsy(Head,psyAddr_S,extAddr_S,1);
				
				if(NULL != Disp){
						
					printf("节点当前信息输出：\r\n网络地址：0x%04X\n物理地址：%02X\n扩展地址：%02X\n在线检测倒计时：%02X\n",Disp->nwkAddr,Disp->psyAddr,Disp->extAddr,Disp->onlineDectect_LCount);
					printf("请输入您的更改信息，该节点数据将会被新数据覆盖\n");
					scanf("%d,%d,%d,%d",&(Disp->nwkAddr),&(Disp->psyAddr),&(Disp->extAddr),&(Disp->onlineDectect_LCount));
					printf("节点数据更新完成\n");
					Disp = NULL;	//安全操作，避免链表被污染 
					free(Disp);	
				}else{
					
					printf("对不起，未寻得对应物理地址为0x%02X的链表节点，节点信息输出失败\r\n\r\n",psyAddr_S);
				}
			}break;
		
			case 7:{
				
				delSame(Head);
				pLen = chatLenDectect(Head);
				
				printf("链表重复节点已被优化，链表当前长度：%d\n",pLen);
			}break;
			
			case 8:{
				
				dispList(Head);
			}break;
			

			
			default:printf("对不起，输入内容无法识别\r\n\r\n");break; 
		}printf("\n\n"); 
	}
}


