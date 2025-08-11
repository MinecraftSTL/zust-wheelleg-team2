#include "UART.h"

uint8 uart_get_data[64];                                                        // 串口接收数据缓冲区
uint8 fifo_get_data[64];                                                        // fifo 输出读出缓冲区

fifo_struct uart_data_fifo;

void UART_Init(void)
{
    fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, uart_get_data, 64);
//    uart_init(UART_INDEX, UART_BAUDRATE, UART_TX_PIN, UART_RX_PIN);             // 初始化串口
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
//调参发送demo：^P1=%.2$!
void Vofa_Adjust(void);
/***********************************************
* @brief : vofa调参解析
* @param : void
* @return: void
* @date  : 2025年1月14日23:51:21
* @author: SJX
* @note  ：需要在串口中断中调用
************************************************/
uint8 RxBuffer[1];//串口接收缓冲
uint16 RxLine = 0;//指令长度
uint8 DataBuff[256];//指令内容
void My_Vofa_CallBack(void)
{
    uart_query_byte(UART_0, &RxBuffer);

//    RxLine++; // 增加接收的数据长度
//    DataBuff[RxLine - 1] = RxBuffer; // 保存接收的数据到缓冲区

    static uint8_t isReceiving = 0; // 标志是否处于接收数据状态
    static uint16_t bufferIndex = 0; // 当前数据写入索引
    // 检测是否是数据包的起始标志
        if (RxBuffer[0] == '^')
        {
            isReceiving = 1; // 开始接收
            bufferIndex = 0; // 重置索引
        }

        if (isReceiving)
        {
            DataBuff[bufferIndex++] = RxBuffer[0]; // 保存当前字节

            // 如果检测到结束标志位，停止接收并解析
            if (RxBuffer[0] == '$')
            {
                isReceiving = 0; // 停止接收
                DataBuff[bufferIndex] = '\0'; // 字符串结束符

//                printf("RXLen=%d\r\n", bufferIndex);
//                for (int i = 0; i < bufferIndex; i++)
//                    printf("UART DataBuff[%d] = %c\r\n", i, DataBuff[i]);

                Vofa_Adjust(); // 数据解析和参数赋值
                memset(DataBuff, 0, sizeof(DataBuff)); // 清空缓冲区
            }

            // 防止缓冲区溢出
            if (bufferIndex >= sizeof(DataBuff))
            {
                isReceiving = 0; // 停止接收
                memset(DataBuff, 0, sizeof(DataBuff)); // 清空缓冲区
                bufferIndex = 0; // 重置索引
            }
        }

        RxBuffer[0] = 0; // 清空接收缓冲区
}

/***********************************************
* @brief : 参数赋值
* @param : void
* @return: void
* @date  : 2025年1月14日23:52:42
* @author: SJX
************************************************/
void Vofa_Adjust(void)
{
    char *start = (char *)DataBuff;
    char *end = NULL;

    while ((end = strchr(start, '$')) != NULL)
    {
//        *end = '\0'; // 将 `!` 替换为字符串结束符

        // 检查并解析单条指令
        if (start[0] == '^') // 检查是否以 `^` 开头
        {
            char tempBuff[256];
            strncpy(tempBuff, start, sizeof(tempBuff) - 1);
            tempBuff[sizeof(tempBuff) - 1] = '\0';
            char *tempEnd = tempBuff+strlen(tempBuff)-1;
            // 调用解析逻辑
            char *equal = strchr(tempBuff, '=');
            if(equal == NULL){
                equal = tempEnd;
            }
            *equal = '\0';
            // 处理指令类型（P1、I1等）
            Page *menu = PageKey_getByPath(&menu_main, tempBuff+1);
            if(menu == NULL){
                return;
            }
            switch(menu->type){
                case INT_TYPE:
                    *menu->extends.intValue.value = (int)atoi(equal+1);
                    break;
                case FLOAT_TYPE:
                    *menu->extends.floatValue.value = (float)atof(equal+1);
                    break;
                case DOUBLE_TYPE:
                    *menu->extends.doubleValue.value = (double)atof(equal+1);
                    break;
                case BOOL_TYPE:
                    *menu->extends.boolValue.value = (uint8)atoi(equal+1);
                    break;;
                case ENUM_TYPE:
                    *menu->extends.enumValue.value = (uint32)atoi(equal+1);
                    break;
                case FUNC_TYPE:
                    menu->extends.funcValue.value();
                    break;
            }
            beepShort();
            // 添加其他指令的处理逻辑
        }

        start = end + 1; // 移动到下一条指令
    }
}
