#ifndef CODE_KEY_H_
#define CODE_KEY_H_
//----°´¼ü----
#define K1    P22_0
#define K2    P22_2
#define K3    P22_1
#define K4    P22_3

uint8 Button_Pressed(void);
uint8 Button_Press(void);//°´¼üÉ¨Ãè
void beep_once();

extern const uint8 long_press_time;
extern uint8 pressed_;
extern uint16 pressed_time[4];

extern int8 speed_qidong;

#endif /* CODE_KEY_H_ */
