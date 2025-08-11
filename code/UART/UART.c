#include "UART.h"

uint8 uart_get_data[64];                                                        // ���ڽ������ݻ�����
uint8 fifo_get_data[64];                                                        // fifo �������������

fifo_struct uart_data_fifo;

void UART_Init(void)
{
    fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, uart_get_data, 64);
//    uart_init(UART_INDEX, UART_BAUDRATE, UART_TX_PIN, UART_RX_PIN);             // ��ʼ������
//    uart_init(UART_2, UART_BAUDRATE, UART2_TX_P10_5, UART2_RX_P10_6);
    uart_init(UART_0, UART_BAUDRATE, UART0_TX_P14_0, UART0_RX_P14_1);
}

//void MyUART_Write_Buffer(float DATA1, float DATA2, float DATA3, float DATA4 )
//{
//    uint8 buff[4] ;
//    buff[0] = DATA1 ;
//    buff[1] = DATA2 ;
//    buff[2] = DATA3 ;
//    buff[3] = DATA4 ;
//    uart_write_buffer(UART_INDEX, buff, 4);
//    uart_write_string(UART_INDEX, "\r\n");
//}
//���η���demo��^P1=%.2$!
void Vofa_Adjust(void);
/***********************************************
* @brief : vofa���ν���
* @param : void
* @return: void
* @date  : 2025��1��14��23:51:21
* @author: SJX
* @note  ����Ҫ�ڴ����ж��е���
************************************************/
uint8 RxBuffer;//���ڽ��ջ���
uint16 RxLine = 0;//ָ���
uint8 DataBuff[256];//ָ������
void My_Vofa_CallBack(void)
{
    uart_query_byte(UART_INDEX, &RxBuffer);

//    RxLine++; // ���ӽ��յ����ݳ���
//    DataBuff[RxLine - 1] = RxBuffer; // ������յ����ݵ�������

    static uint8_t isReceiving = 0; // ��־�Ƿ��ڽ�������״̬
    static uint16_t bufferIndex = 0; // ��ǰ����д������
    // ����Ƿ������ݰ�����ʼ��־
        if (RxBuffer == '^')
        {
            isReceiving = 1; // ��ʼ����
            bufferIndex = 0; // ��������
        }

        if (isReceiving)
        {
            DataBuff[bufferIndex++] = RxBuffer; // ���浱ǰ�ֽ�

            // �����⵽������־λ��ֹͣ���ղ�����
            if (RxBuffer == '$')
            {
                isReceiving = 0; // ֹͣ����
                DataBuff[bufferIndex] = '\0'; // �ַ���������
                Vofa_Adjust(); // ���ݽ����Ͳ�����ֵ
                memset(DataBuff, 0, sizeof(DataBuff)); // ��ջ�����
            }

            // ��ֹ���������
            if (bufferIndex >= sizeof(DataBuff))
            {
                isReceiving = 0; // ֹͣ����
                memset(DataBuff, 0, sizeof(DataBuff)); // ��ջ�����
                bufferIndex = 0; // ��������
            }
        }

        RxBuffer = 0; // ��ս��ջ�����
}

/***********************************************
* @brief : ������ֵ
* @param : void
* @return: void
* @date  : 2025��1��14��23:52:42
* @author: SJX
************************************************/
void Vofa_Adjust(void)
{
    char *start = (char *)DataBuff;
    char *end = NULL;

    for (;(end = strchr(start, '$')) != NULL; start = end + 1)
    {
//        *end = '\0'; // �� `!` �滻Ϊ�ַ���������

        // ��鲢��������ָ��
        if (start[0] == '^') // ����Ƿ��� `^` ��ͷ
        {
            char tempBuff[256];
            strncpy(tempBuff, start, sizeof(tempBuff) - 1);
            tempBuff[sizeof(tempBuff) - 1] = '\0';
            char *tempEnd = tempBuff+strlen(tempBuff)-1;
            *tempEnd = '\0';
            printf("^/received %s$\r\n",tempBuff+1);
            if(tempBuff[1] == '/'){
                //TODO:����ָ���ң����
            }else{
                char path[256], value[256];
                char *equal = strchr(tempBuff, '=');
                if(equal == NULL){
                    equal = tempEnd;
                }
                strncpy(path, tempBuff+1, equal-tempBuff-1);
                path[equal-tempBuff-1] = '\0';
                strncpy(value, equal+1, tempEnd-equal);
                value[tempEnd-equal] = '\0';
                Page *menu = Page_getByPath(&menu_main, path);
                if(menu == NULL){
                    continue;
                }
                switch(menu->type){
                    case INT_TYPE:
                        *menu->extends.intValue.value = (int)atoi(value);
                        break;
                    case FLOAT_TYPE:
                        *menu->extends.floatValue.value = (float)atof(value);
                        break;
                    case DOUBLE_TYPE:
                        *menu->extends.doubleValue.value = (double)atof(value);
                        break;
                    case BOOL_TYPE:
                        *menu->extends.boolValue.value = (uint8)atoi(value);
                        break;;
                    case ENUM_TYPE:
                        *menu->extends.enumValue.value = (uint32)atoi(value);
                        break;
                    case FUNC_TYPE:
                        menu->extends.funcValue.value();
                        break;
                }
                if(menu->update){
                    menu->update(menu);
                }
                beepShort();
            }
        }
    }
}

void Vofa_pageSend(Page *page){
    char path[PAGE_PATH_MAX+1];
    Page_getPath(page, path);
    switch(page->type){
        case INT_TYPE:
            printf("^%s=%d$", path, *page->extends.intValue.value);
            break;
        case FLOAT_TYPE:
            printf("^%s=%f$", path, *page->extends.floatValue.value);
            break;
        case DOUBLE_TYPE:
            printf("^%s=%lf$", path, *page->extends.doubleValue.value);
            break;
        case BOOL_TYPE:
            printf("^%s=%d$", path, *page->extends.boolValue.value);
            break;;
        case ENUM_TYPE:
            printf("^%s=%d$", path, *page->extends.enumValue.value);
            break;
    }
}

void Vofa_pageAllSend(){
    Page_allSubRun(&menu_main, Vofa_pageSend);
    for(uint16 i=0; i<0x0fff; ++i){
        printf("\r\n");
    }
}

void Vofa_pageAllReceive(){
    printf("^/send$\r\n");
}
