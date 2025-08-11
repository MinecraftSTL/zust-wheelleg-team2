/*
���ߣ�Charon and ������С��Ƭ
       δ����Ȩ��ֹת��
 */


#ifndef CODE_CAMERA_H_
#define CODE_CAMERA_H_

#define biao_column 94
//�궨��
#define image_h 120//ͼ��߶�
#define image_w 188//ͼ����

#define white_pixel 255    //ͼ���
#define black_pixel 0      //ͼ���

#define bin_jump_num    1     //�����ĵ���
#define border_max  image_w-2 //�߽����ֵ
#define border_min  1         //�߽���Сֵ
extern uint8 original_image[image_h][image_w];
extern uint8 bin_image[image_h][image_w];//ͼ������
extern uint8 Finish_Flag; //������ɱ�ʶλ
extern uint32 image_process_time;   //ͼ����ʱ��

extern void image_process(void);


void binaryzation(void);
void IPS_show(void);
float absolute(float z);
void right_straight(void);
void breadth_Addingline(uint8 choice);
void Addingline1( uint8 choice, uint8 startX, uint8 startY);
void Addingline( uint8 choice, uint8 startX, uint8 startY, uint8 endX, uint8 endY);
void Addingline2( uint8 choice, uint8 startX, uint8 startY);
void Element_recognition(void);
uint8 OtsuThreshold(uint8 *image, uint16 col, uint16 row);
void advanced_regression(int type, int startline1, int endline1, int startline2, int endline2);
int Judgment_symbol(float x, float y);
void ten_inflection(void);
void Lower_left(void);
void Lower_right(void);
void Upper_left(void);
void Upper_right(void);
void growth_direction(void);
void S_road(void);
void left_S_road_inflection(void);
void right_S_road_inflection(void);
void hill_juge(void);
void r_roadblock_deal(void);
void l_roadblock_deal(void);
void level_out_juge(void);//
void roadblock_addline(void);
void Endline_L_R_(void);
void show_point(uint8 x, uint8 y, rgb565_color_enum c);//����
extern int  left[120];
extern int  right[120];
extern int  middle[120];
extern uint8 imag[120][188];
extern uint8 threshold_value;
extern uint8 Effective_length;

extern const int weight[120];
extern int err;
extern uint8 xunxian;
extern uint8 bend_straight_flag;
extern uint8 zebra_crossing_flag;//�����߱�־

extern uint8 annulus_L_Flag;       //��Բ����־
extern uint8 annulus_R_Flag;       //��Բ����־

extern uint8 S_road_Flag;
extern int Endline;           //��ֹ��

extern int l_point1_juge[5],l_point2_juge[5],l_point3_juge[5];
extern int l_point1[2],l_point2[2],l_point3[2];
extern int l_point1_location,l_point2_location,l_point3_location;
extern char flag_l_roadblock_deal;//1����0������
extern char flag_l_roadblock_exist;//�����ж��Ƿ����  0��1û��
extern int r_point1_juge[5],r_point2_juge[5],r_point3_juge[5];
extern int r_point1[2],r_point2[2],r_point3[2];
extern int r_point1_location,r_point2_location,r_point3_location;
extern char flag_r_roadblock_deal;//1����0������
extern char flag_r_roadblock_exist;//�����ж��Ƿ����  0��1û��

#endif /* CODE_CAMERA_H_ */
