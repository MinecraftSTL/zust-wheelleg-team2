#include "MyHeadfile.h"

int g_camera_mid_err;

Image image;
Image image1;
Image image2;
Image imageShow;

uint16 pro_time;            //����ʱ��

int camera_miderr;              //����ͷ����ƫ��



void MyCamera_Init(void)
{
    ips200_show_string(0, 0, "mt9v03x init.");
    while(1)
    {
        if(mt9v03x_init())
            ips200_show_string(0, 80, "mt9v03x reinit.");
        else
            break;
        system_delay_ms(500);                                                   // ����ʱ�������Ʊ�ʾ�쳣
    }
    ips200_show_string(0, 16, "init success.");
}

void MyCamera_Show(uint16 start_y)
{
    Image_clone(&image1, &imageShow);
    ips200_show_gray_image(0, start_y, &imageShow.image, imageShow.w, imageShow.h, imageShow.w, imageShow.h, 0);
}
uint8 Image_get(Image *this, uint16 y, uint16 x){
    return this->image[x + this->w*y];
}
void Image_set(Image *this, uint16 y, uint16 x, uint8 value){
    this->image[x + this->w*y] = value;
}
void Image_fromCamera(Image *this, uint8 *mt9v03x_image)
{
    this->h = MT9V03X_H;
    this->w = MT9V03X_W;
    memcpy(this->image, mt9v03x_image, MT9V03X_IMAGE_SIZE);
}
void Image_clone(Image *this, Image *target)
{
    zf_assert(this && target);
    target->h = this->h;
    target->w = this->w;
    memcpy(target->image, this->image, this->h*this->w);
}
void Image_cut(Image *this, Image *target, uint16 h, uint16 w){
    zf_assert(this && target && 2*h <= this->h && 2*w <= this->w);
    target->h = this->h - 2*h;
    target->w = this->w - 2*w;
    for(uint16 i = 0; i < target->h; ++i){
        for(uint16 j = 0; j < target->w; ++j){
            Image_set(target,i,j,Image_get(this,i+h,j+w));
        }
    }
}
void Image_zoom(Image *this, Image *target, float zoom){
    zf_assert(this && target && zoom > 0.0f);
    target->h = (this->h-1)*zoom+1;
    target->w = (this->w-1)*zoom+1;
    for(uint16 i = 0; i < target->h; ++i){
        for(uint16 j = 0; j < target->w; ++j){
            Image_set(target,i,j,Image_get(this,i/zoom,j/zoom));
        }
    }
}
void Image_binaryzation(Image *this, Image *target, uint16 r, int16 deltaT){
    zf_assert(this && target);
    zf_assert(2 * r < this->h && 2 * r < this->w); // ȷ��ԭͼ�㹻��
    target->h = this->h - r*2;
    target->w = this->w - r*2;
    for(uint16 i = 0; i < target->h; ++i){
        for(uint16 j = 0; j < target->w; ++j){
            uint64 sum = 0;
            for(uint16 k = i; k<=i+2*r; ++k){
                for(uint16 l = j; l<=j+2*r; ++l){
                    sum += Image_get(this,k,l);
                }
            }
            int16 threshold = sum/((r*2+1)*(r*2+1));
            threshold+=deltaT;
            Image_set(target,i,j,Image_get(this,i+r,j+r)>threshold ? 0xFF : 0);
        }
    }
}

void Image_drawRectan(Image *this)
{

    uint8 i = 0;
    for (i = 0; i < this->h; i++)     //�����0��1�к��ұ�186��187�л��ڿ�
    {
        Image_set(this,i,0,0);
        Image_set(this,i,1,0);
        Image_set(this,i,this->w - 1,0);
        Image_set(this,i,this->w - 2,0);

    }
    for (i = 0; i < this->w; i++)     //���Ϸ�0��1�л��ڿ�
    {
        Image_set(this,0,i,0);
        Image_set(this,1,i,0);
    }
}
//
//uint8 get_start_point(uint8 start_row)
//{
//    uint8 First_start_row;
//    First_start_row = start_row;
//    uint8 i = 0,l_found = 0,r_found = 0;
//    //����
//    start_point_l[0] = 0;//x
//    start_point_l[1] = 0;//y
//
//    start_point_r[0] = 0;//x
//    start_point_r[1] = 0;//y
//
//        //���м�����ߣ��������
//    for (i = image_w / 2; i > Border_Min; i--)
//    {
//        start_point_l[0] = i;//x
//        start_point_l[1] = start_row;//y
//        if (image[start_row][i] == 255 && image[start_row][i - 1] == 0&&
//            image[start_row][i+1] == 255 && image[start_row][i - 2] == 0)
//        {
////            printf("�ҵ�������image[%d][%d]\n", start_row,i);
//            l_found = 1;
//            break;
//        }
//    }
//
//    for (i = image_w / 2; i < Border_Max; i++)
//    {
//        start_point_r[0] = i;//x
//        start_point_r[1] = start_row;//y
//        if (image[start_row][i-1] == 255 && image[start_row][i] == 255 &&
//            image[start_row][i + 1] == 0 && image[start_row][i + 2] == 0)
//        {
////            printf("�ҵ��ұ����image[%d][%d]\n",start_row, i);
//            r_found = 1;
//            break;
//        }
//    }
//
//    if(l_found&&r_found)return 1;
//    else {
////        printf("δ�ҵ����\n");
//        return 0;
//    }
//}
//
///*********************��ʼ���Ⱥͼ���*********************/
////bool Point_CBH(uint8 row, uint8 col)
////{
////    int8 point_cbh_sum;
////    if(row == image_h - 1 || row == 0)
////        return 0;
////
////
////}
//
//
//
//// �������     ������Ѳ��
//// ����˵��     Berak_Flag      Ѳ�ߴ�������ֹ��ѭ��
//// ����˵��     image_w         ͼ����
//// ���ز���     void
//// ʹ��ʾ��
//// ��ע��Ϣ
//void Search_Line_BLY(uint16 break_flag, uint8(*image)[image_w], uint16 *l_stastic, uint16 *r_stastic, uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y, int*Endline)
//{
////    uint8 i = 0, j = 0;
////    /***********�����top************/
////
////
////    //{-1,-1},{0,-1},{+1,-1},
////    //{-1, 0},       {+1, 0},
////    //{-1,+1},{0,+1},{+1,+1},
////    //�������ʱ��
////    static int8 seeds_l[8][2] = { {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}, };
////    /***********�����end************/
////
////
////    /***********�ұ���top************/
////    uint8 search_filds_r[8][2] = { {  0  } };
////
////    //����˸�����
////    //{-1,-1},{0,-1},{+1,-1},
////    //{-1, 0},       {+1, 0},
////    //{-1,+1},{0,+1},{+1,+1},
////    //�����˳ʱ��
////    static int8 seeds_r[8][2] = { {0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},{1,1} };
////
////    /***********�ұ���end************/
//        data_stastics_l = 0;
//        data_stastics_r = 0;
//
//        uint8 i = 0, j = 0;
//        //��߱���
//        uint8 search_filds_l[8][2] = { { 0 } };
//        uint8 index_l = 0;
//        uint8 temp_l[8][2] = { {  0 } };
//        uint8 center_point_l[2] = {  0 };
//        uint16 l_data_statics;//ͳ�����
//        //����˸�����
//        static int8 seeds_l[8][2] = { {0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},{1,1} };
////        static int8 seeds_l[8][2] = { {-1,0}, {-1,-1}, {0,-1}, {1,-1}, {+1,0}, {1,1}, {0,1}, {-1,1} };
//        //{-1,-1},{0,-1},{+1,-1},
//        //{-1, 0},       {+1, 0},
//        //{-1,+1},{0,+1},{+1,+1},
//        //�����˳ʱ��
//
//        //�ұ߱���
//        uint8 search_filds_r[8][2] = { {  0 } };
//        uint8 center_point_r[2] = { 0 };//���������
//        uint8 index_r = 0;//�����±�
//        uint8 temp_r[8][2] = { {  0 } };
//        uint16 r_data_statics;//ͳ���ұ�
//        //����˸�����
//        static int8 seeds_r[8][2] = { {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}, };
////        static int8 seeds_r[8][2] = { {1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1} };
////        static int8 seeds_l[8][2] = { {1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1} };
////        static int8 seeds_r[8][2] = { {-1,0}, {-1,-1}, {0,-1}, {1,-1}, {+1,0}, {1,1}, {0,1}, {-1,1} };
//        //{-1,-1},{0,-1},{+1,-1},
//        //{-1, 0},       {+1, 0},
//        //{-1,+1},{0,+1},{+1,+1},
//        //�������ʱ��
//
//        l_data_statics = *l_stastic;//ͳ���ҵ��˶��ٸ��㣬��������ѵ�ȫ��������
//        r_data_statics = *r_stastic;//ͳ���ҵ��˶��ٸ��㣬��������ѵ�ȫ��������
//
//        //��һ�θ��������  ���ҵ������ֵ������
//        center_point_l[0] = l_start_x;//x
//        center_point_l[1] = l_start_y;//y
//        center_point_r[0] = r_start_x;//x
//        center_point_r[1] = r_start_y;//y
//
//            //��������ѭ��
//        while (break_flag--)
//        {
//
//            //���
//            for (i = 0; i < 8; i++)//����8F����
//            {
//                search_filds_l[i][0] = center_point_l[0] + seeds_l[i][0];//x
//                search_filds_l[i][1] = center_point_l[1] + seeds_l[i][1];//y
//            }
//            //�����������䵽�Ѿ��ҵ��ĵ���
//            points_l[l_data_statics][0] = center_point_l[0];//x
//            points_l[l_data_statics][1] = center_point_l[1];//y
//            l_data_statics++;//������һ
//
//            //�ұ�
//            for (i = 0; i < 8; i++)//����8F����
//            {
//                search_filds_r[i][0] = center_point_r[0] + seeds_r[i][0];//x
//                search_filds_r[i][1] = center_point_r[1] + seeds_r[i][1];//y
//            }
//            //�����������䵽�Ѿ��ҵ��ĵ���
//            points_r[r_data_statics][0] = center_point_r[0];//x
//            points_r[r_data_statics][1] = center_point_r[1];//y
//
//            index_l = 0;//�����㣬��ʹ��
//            for (i = 0; i < 8; i++)
//            {
//                temp_l[i][0] = 0;//�����㣬��ʹ��
//                temp_l[i][1] = 0;//�����㣬��ʹ��
//            }
//
//            //����ж�
//            for (i = 0; i < 8; i++)
//            {
//                if (image[search_filds_l[i][1]][search_filds_l[i][0]] == 0
//                    && image[search_filds_l[(i + 1) & 7][1]][search_filds_l[(i + 1) & 7][0]] == 255)
//                {
//                    temp_l[index_l][0] = search_filds_l[(i)][0];
//                    temp_l[index_l][1] = search_filds_l[(i)][1];
//                    index_l++;
//                    dir_l[l_data_statics - 1] = (i);//��¼��������
//                }
//
//                if (index_l)
//                {
//                    //���������
//                    center_point_l[0] = temp_l[0][0];//x
//                    center_point_l[1] = temp_l[0][1];//y
//                    for (j = 0; j < index_l; j++)
//                    {
//                        if (center_point_l[1] > temp_l[j][1])
//                        {
//                            center_point_l[0] = temp_l[j][0];//x
//                            center_point_l[1] = temp_l[j][1];//y
//                        }
//                    }
//                }
//
//            }
//            if ((points_r[r_data_statics][0]== points_r[r_data_statics-1][0]&& points_r[r_data_statics][0] == points_r[r_data_statics - 2][0]
//                && points_r[r_data_statics][1] == points_r[r_data_statics - 1][1] && points_r[r_data_statics][1] == points_r[r_data_statics - 2][1])
//                ||(points_l[l_data_statics-1][0] == points_l[l_data_statics - 2][0] && points_l[l_data_statics-1][0] == points_l[l_data_statics - 3][0]
//                    && points_l[l_data_statics-1][1] == points_l[l_data_statics - 2][1] && points_l[l_data_statics-1][1] == points_l[l_data_statics - 3][1]))
//            {
////                printf("���ν���ͬһ���㣬�˳�\n");
//                break;
//            }
//            if (absolute(points_r[r_data_statics][0] - points_l[l_data_statics - 1][0]) < 2
//                && absolute(points_r[r_data_statics][1] - points_l[l_data_statics - 1][1] < 2)
//                )
//            {
////                printf("\n���������˳�\n");
//                *Endline = (points_r[r_data_statics][1] + points_l[l_data_statics - 1][1]) >> 1;//ȡ����ߵ�
////                printf("\n��y=%d���˳�\n",*Endline);
//                break;
//            }
//            if ((points_r[r_data_statics][1] < points_l[l_data_statics - 1][1]))
//            {
////                printf("\n�����߱��ұ߸��ˣ���ߵȴ��ұ�\n");
//                continue;//�����߱��ұ߸��ˣ���ߵȴ��ұ�
//            }
//            if (dir_l[l_data_statics - 1] == 7
//                && (points_r[r_data_statics][1] > points_l[l_data_statics - 1][1]))//��߱��ұ߸����Ѿ�����������
//            {
////                printf("\n��߿�ʼ�����ˣ��ȴ��ұߣ��ȴ���... \n");
//                center_point_l[0] = (uint8)points_l[l_data_statics - 1][0];//x
//                center_point_l[1] = (uint8)points_l[l_data_statics - 1][1];//y
//                l_data_statics--;
//            }
//            r_data_statics++;//������һ
//
//            index_r = 0;//�����㣬��ʹ��
//            for (i = 0; i < 8; i++)
//            {
//                temp_r[i][0] = 0;//�����㣬��ʹ��
//                temp_r[i][1] = 0;//�����㣬��ʹ��
//            }
//
//            //�ұ��ж�
//            for (i = 0; i < 8; i++)
//            {
//                if (image[search_filds_r[i][1]][search_filds_r[i][0]] == 0
//                    && image[search_filds_r[(i + 1) & 7][1]][search_filds_r[(i + 1) & 7][0]] == 255)
//                {
//                    temp_r[index_r][0] = search_filds_r[(i)][0];
//                    temp_r[index_r][1] = search_filds_r[(i)][1];
//                    index_r++;//������һ
//                    dir_r[r_data_statics - 1] = (i);//��¼��������
////                    printf("dir[%d]:%d\n", r_data_statics - 1, dir_r[r_data_statics - 1]);
//                }
//                if (index_r)
//                {
//
//                    //���������
//                    center_point_r[0] = temp_r[0][0];//x
//                    center_point_r[1] = temp_r[0][1];//y
//                    for (j = 0; j < index_r; j++)
//                    {
//                        if (center_point_r[1] > temp_r[j][1])
//                        {
//                            center_point_r[0] = temp_r[j][0];//x
//                            center_point_r[1] = temp_r[j][1];//y
//                        }
//                    }
//
//                }
//            }
//        }
//
//        //ȡ��ѭ������
//        *l_stastic = l_data_statics;
//        *r_stastic = r_data_statics;
//}
//
//void get_left(uint16 total_L)
//{
//    uint8 i = 0;
//    uint16 j = 0;
//    uint8 h = 0;
//
//    //��ʼ��
//    for (i = 0;i<image_h;i++)
//    {
//        left[i] = Border_Min;
//        left_copy[i] = Border_Min;
//    }
//    h = image_h - 2;               //��118�п�ʼ��ȡ
//    //���
//    for (j = 0; j < total_L; j++)
//    {
//        //printf("%d\n", j);
//        if (points_l[j][1] == h)
//        {
//            left[h] = points_l[j][0]+1;   //��ȡ�����ڱ߽��ߵ��ڲ�
//            left_copy[h] = points_l[j][0]+1;
//        }
//        else continue; //ÿ��ֻȡһ���㣬û����һ�оͲ���¼
//        h--;
//        if (h == 0)
//        {
//            break;//�����һ���˳�
//        }
//    }
//}
//void get_right(uint16 total_R)
//{
//    uint8 i = 0;
//    uint16 j = 0;
//    uint8 h = 0;
//
//    for (i = 0; i < image_h; i++)
//    {
//        right[i] = Border_Max;//�ұ��߳�ʼ���ŵ����ұߣ�����߷ŵ�����ߣ�����������պ�����������߾ͻ����м䣬������ŵõ�������
//        right_copy[i] = Border_Max;
//    }
//    h = image_h - 2;
//    //�ұ�
//    for (j = 0; j < total_R; j++)
//    {
//        if (points_r[j][1] == h)
//        {
//            right[h] = points_r[j][0] - 1;
//            right_copy[h] = points_r[j][0] - 1;
//        }
//        else continue;//ÿ��ֻȡһ���㣬û����һ�оͲ���¼
//        h--;
//        if (h == 0)break;//�����һ���˳�
//    }
//}
//
//void middle_line(void)
//{
//    int i;
//    memset(middle,0,sizeof(middle));
//    for(i=119;i>Endline;i--)           //��Ч��ȫ����ȡ����
//    {
//        middle[i]=(right[i]+left[i])/2;
//        middle_copy[i]=(right_copy[i]+left_copy[i])/2;
//    }
//}
//
//void Lost_Right(void)
//{
//    uint8 i = 0;  // ����ѭ������ i
//    right_lost_num = 0;  // ��ʼ���Ҳඪ�߼�����
//    Lost_right_Flag = 0;  // ��ʼ���Ҳඪ�߱�־
//    right_lost_num2 = 0;  // ��ʼ���ڶ����Ҳඪ�߼�����
//    Lost_right_Flag2 = 0;  // ��ʼ���ڶ����Ҳඪ�߱�־
//
//    // ��һ���֣�����Ҳඪ�����
//    for (i = 90; i > 10; i--)
//    {
//        if (right[i] >= 185)
//        {  // �ж��ұߵ� i �е�ֵ�Ƿ���ڵ��� 185����ʾ��ɫ��
//            right_lost_num++;  // ͳ�ƶ��ߴ���
//            lost_point_R_scan_line = i + 5;  // ��¼���ߵ�ɨ����
//        }
//        if (right_lost_num > 15)
//        {  // ������ߴ������� 15
//            Lost_right_Flag = 1;  // ���ö��߱�־
//            break;  // �˳�ѭ��
//        }
//    }
//
//    // �ڶ����֣���һ������Ҳඪ�����
//    for (i = 100; i > 40; i--) {
//        if (right[i] >= 185) {  // �ж��ұߵ� i �е�ֵ�Ƿ���ڵ��� 185����ʾ��ɫ��
//            right_lost_num2++;  // ͳ�ƶ��ߴ���
//        }
//        if (right_lost_num2 > 15) {  // ������ߴ������� 15
//            Lost_right_Flag2 = 1;  // ���õڶ������߱�־
//            break;  // �˳�ѭ��
//        }
//    }
//
//    // �������֣�����Ҳඪ�����
//    for (i = 90; i > 40; i--) {
//        if (right[i] >= 185) {  // �ж��ұߵ� i �е�ֵ�Ƿ���ڵ��� 185����ʾ��ɫ��
//            right_lost_num3++;  // ͳ�ƶ��ߴ���
//        }
//        if (right_lost_num3 > 15) {  // ������ߴ������� 15
//            Lost_right_Flag3 = 1;  // ���õ��������߱�־
//            break;  // �˳�ѭ��
//        }
//    }
//}
//void Lost_Left(void)
//{
//    uint8 i=0;
//    left_lost_num=0;
//    left_lost_num2=0;
//    left_lost_num3=0;
//    Lost_left_Flag=0;
//    Lost_left_Flag2=0;
//    Lost_left_Flag3=0;
//    //1
//    for(i=90;i>10;i--)
//    {
//        if(left[i]<=3)     //��ߵ�2�е��ڰ׾Ͷ���
//        {
//            left_lost_num++;
//            lost_point_L_scan_line=i+5;
//        }
//        if(left_lost_num>15)
//        {
//            Lost_left_Flag=1; //�ж�����·��Ƿ���
//            break;
////            return;
//        }
//    }
//    ///2
//    for(i=100;i>40;i--)
//    {
//        if(left[i]<=3)     //�ұߵ�185�е��ڰ׾Ͷ���
//        {
//            left_lost_num2++;
//        }
//        if(left_lost_num2>10)
//        {
//            Lost_left_Flag2=1; //�ж�����·��Ƿ���
//            break;
////            return;
//        }
//    }
//    ///3
//    for(i=90;i>40;i--)
//    {
//        if(left[i]<=3)     //�ұߵ�185�е��ڰ׾Ͷ���
//        {
//            left_lost_num3++;
//        }
//        if(left_lost_num3>10)
//        {
//            Lost_left_Flag3=1; //�ж�����·��Ƿ���
//            break;
////            return;
//        }
//    }
//
//}
//
//
///***********************************************
//* @brief : ��ȡ����ͷ�������,ʹ�� λ��/Ȩ��
//* @param : void
//* @return: int
//* @date  : 2024��10��25��12:28:00
//* @author: SJX
//************************************************/
//int camera_horizon = 40;           //ǰհ
//int delta_camera_horizon = 40;
//int Camera_Get_MidErr(void)
//{
//
//#if(MIDDLE_LINE_MODE == 1)
//    int i;
//    int err_sum = 0, err = 0;
//    for(i=camera_horizon;i < camera_horizon+delta_camera_horizon; i++)
//    {
//        err_sum += middle_copy[i] ;
//    }
//    err = err_sum / delta_camera_horizon;
//    return err;
//#endif
//#if(MIDDLE_LINE_MODE == 2)
//    int i;
//    int weight_sum=0;
//    for(i=119;i >= 0;i--)
//    {
//        if(middle_copy[i]!=0)
//        {
//            camera_miderr += weight[i] * (Target_Column-middle[i]);
//            weight_sum += weight[i];
//        }
//    }
//    camera_miderr /= weight_sum;
//    return camera_miderr;
//#endif
//}
///***********************************************
//* @brief : ��ֱ��
//* @param : void
//* @return: void
//* @date  : 2024��10��28��19:08:58
//* @author: SJX Copy
//************************************************/
//float l_k = 0;
//void Left_Straight(void)
//{
//    float k1,k2;
//    left_straight_flag = 0;
//    float l_slope2=0,l_slope3=0,l_distance2=0,l_distance3=0;
//    caculate_distance(25,80,left,&l_slope3,&l_distance3);
//    k2=l_slope3;
////    caculate_distance(30,40,left,&l_slope2,&l_distance2);
////    k1=l_slope2;
////    if(absolute(k1-k2)<0.8)
//////        left_straight_flag = 1;
////    l_k=absolute(k1-k2);
//    if(k2 <= 0.95)
//    {
//        left_straight_flag = 1;
//    }
////    printf("%f\r\n",k2);
////        float k1,k2=0;
////        left_straight_flag = 0;
////        float l_slope2=0,l_slope3=0,l_distance2=0,l_distance3=0;
////        caculate_distance(4,80,left,&l_slope3,&l_distance3);
////        k2=l_slope3;
////        printf("%d\r\n",k2);
//}
///***********************************************
//* @brief : ��ֱ��
//* @param : void
//* @return: void
//* @date  : 2024��10��28��19:10:17
//* @author: SJX Copy
//************************************************/
//float r_k = 0;
//void Right_Straight(void)
//{
//        float k1,k2;
//        right_straight_flag=0;
//        float l_slope2=0,l_slope3=0,l_distance2=0,l_distance3=0;
//        caculate_distance(25,80,right,&l_slope3,&l_distance3);
//        k2=l_slope3;
////        caculate_distance(5,50,right,&l_slope2,&l_distance2);
////        k1=l_slope2;
////        if(absolute(k1-k2)<0.3)
////            right_straight_flag=1;
////        r_k=absolute(k1-k2);
//        if(k2 <= 0.96)
//        {
//            right_straight_flag = 1;
//        }
//
//}
//
//
///***********************************************
//* @brief : ָ���յ����㲹��
//* @param : void
//* @return: choice ���߻����Ҳ��ߣ�1��2��
//*          startX ��ʼX
//*          startY ��ʼY
//           endX ��ֹX
//           endY ��ֹY
//* @date  : 2024��10��27��20:21:54
//* @author: SJX
//************************************************/
//void Appoint_Adding_Line( uint8 choice, uint8 startX, uint8 startY, uint8 endX, uint8 endY)
//{
//    y = 0;
//    // ֱ�� x = ky + b
//    float k = 0;
//    float b = 0;
//    switch(choice)
//    {
//      case 1://����
//        {
//            k = (float)((float)endX - (float)startX) / (float)((float)endY - (float)startY);
//            b = (float)startX - (float)startY * k;
//            for(y = startY; y < endY; y++)
//            {
//                if( (uint8)(k * y + b)>185)
//                {
////                    left[y] = 185;
//                    left_copy[y] = 185;
//                }
//
//                else if( (uint8)(k * y + b)<2)
//                {
////                    left[y] = 2;
//                    left_copy[y] = 2;
//                }
//
//                else
//                {
////                    left[y] = (k * y + b);
//                    left_copy[y] = (k * y + b);
//                }
//            }
//            break;
//        }
//      case 2://�Ҳ���
//        {
//            k = (float)((float)endX - (float)startX) / (float)((float)endY - (float)startY);
//            b = (float)startX - (float)startY * k;
//            for(y = startY; y < endY; y++)
//            {
//                if( (uint8)(k * y + b)>185)
//                {
////                    right[y]=185;
//                    right_copy[y]=185;
//                }
//
//                else if ( (uint8)(k * y + b)<2)
//                {
////                    right[y]=2;
//                    right_copy[y]=2;
//                }
//
//                else
//                {
////                    right[y]= (k * y + b);
//                    right_copy[y]= (k * y + b);
//                }
//            }
//            break;
//        }
//    }
//}
///***********************************************
//* @brief : ֱ������ж�
//* @param : void
//* @return: void
//* @date  : 2024��11��1��20:18:14
//* @author: �Ͻ���
//************************************************/
//float straight_k_err=0;
//float straight_k_1=0;
//float straight_k_2=0;
//void Bend_Straight_Opinion(void)
//{
//    float k1,k2;
//    float l_slope2=0,l_slope3=0,l_distance2=0,l_distance3=0;
//    bend_straight_flag=0;
//    caculate_distance(15,35,middle,&l_slope3,&l_distance3);
//    k2=l_slope3;
//
//    caculate_distance(45,80,middle,&l_slope2,&l_distance2);
//    k1=l_slope2;
//
//    if((absolute(k1-k2)<0.08))//&&(Lost_left_Flag==0)&&(Lost_right_Flag==0)
//    bend_straight_flag=1;
//    else
//    bend_straight_flag=0;
//    straight_k_err=absolute(k1-k2);
////    printf("%f\r\n", straight_k_err);
//}
///***********************************************
//* @brief : ������
//* @param : void
//* @return: void
//* @date  : 2024��11��1��21:03:49
//* @author: SJX
//************************************************/
//void Zebra_Crossing(void)
//{
//    uint8 zebra_row;
//    int i;
//    int edge_left_num = 0;
//    int edge_right_num = 0;
//    int edge_sum = 0;
//    zebra_crossing_flag = 0;
//    for(zebra_row = ZERBA_CROSSING_ROW; zebra_row < ZERBA_CROSSING_ROW+3; zebra_row++)
//    {
//        for(i=left[zebra_row];i<94;i++)
//        {
//            if((image[zebra_row][i] == white_pixel && image[zebra_row][i+1] == white_pixel) &&
//               (image[zebra_row][i+2] == black_pixel && image[zebra_row][i+3] == black_pixel))
//            {
//                edge_left_num++;
//            }
////            printf("%d,%d\r\n",image[zebra_row][i]);
//        }
//        for(i=right[zebra_row];i>94;i--)
//        {
//            if((image[zebra_row][i] == white_pixel && image[zebra_row][i-1] == white_pixel) &&
//               (image[zebra_row][i-2] == black_pixel && image[zebra_row][i-3] == black_pixel))
//            {
//                edge_right_num++;
//            }
//        }
//    }
//    edge_sum = edge_left_num + edge_right_num;
//    if(edge_sum > 200)
//        edge_sum = 0;
//    if(edge_left_num > 200)
//        edge_left_num = 0;
//    if(edge_sum >= 16 && edge_left_num > 5 && edge_right_num > 5)                      //ͣ��
//    {
//        static Encoder_Distance_Typedef Zebra_Crossing_Structure;
//        static uint8 zebra_crossing_break_flag = 0;
////        system_delay_ms(210);
//        zebra_crossing_flag = 1;
//        Zebra_Crossing_Structure.distance_record_status = close_status;
//
//        Car_Stop();
//        beepMid();
////        system_delay_ms(1000);
//
//    }
//    else
//        zebra_crossing_flag = 0;
////    printf("%d, %d, %d ,%d\r\n",edge_left_num, edge_right_num, left[zebra_row], right[zebra_row]);
////    printf("%d, %d, %d\r\n",edge_left_num, edge_right_num, edge_sum);
//}

void Image_Process(void)
{
    static Encoder_Distance_Typedef Truck_Out_Distance_Structure;
    if(mt9v03x_finish_flag == 1)              //�ж�һ��ͼ���Ƿ�������
    {

        Image_fromCamera(&image, mt9v03x_image);               //ͼ��ת�� 37nsһ֡
//        Image_cut(&image, &image1,0,0);
//        Image_zoom(&image1, &image,0);
        Image_binaryzation(&image, &image1,3,-6);
//        Image_clone(&image1, &image);
        mt9v03x_finish_flag = 0;
//        Binaryzation();                 //��ֵ��       5.7msһ֡
////        image_filter(image);            //���͸�ʴ�˲�      10msһ֡��������
//        image_draw_rectan(image);       //���ڿ�           20nsһ֡
//        start_point_detection_flag = get_start_point(image_h - 2) ;         //�ұ߽���㣬25nsһ֡
//        if(start_point_detection_flag)
//        {
//            //������   Բ��3msһ֡  ֱ��1.8msһ֡
//            Search_Line_BLY((uint16)USE_num, image, &data_stastics_l, &data_stastics_r, start_point_l[0], start_point_l[1], start_point_r[0], start_point_r[1], &Endline);
////            Growth_Direction();
//            get_left(data_stastics_l);    //��߽���ȡ
//            get_right(data_stastics_r);   //�ұ߽���ȡ
//            Zebra_Crossing();
//            Lost_Left();                  //���·������ж�
//            Lost_Right();                 //���·������ж�
//
//            Lower_left();                   //���¶ϵ�
//            Lower_right();                  //���¶ϵ�
//            Upper_left();                   //���϶ϵ�
//            Upper_right();                  //���϶ϵ�
//
//            Cross_Road();                   //ʮ��
//            Left_Straight();              //��ֱ���ж�
//            Right_Straight();             //��ֱ���ж�
//
////            Inflection_Point();           //�ϵ��ܶ�
//            Find_Circle();                  //��Բ��
//
//            middle_line();                  //��ȡ����
//            Middle_Empty();
////            Bend_Straight_Opinion();        //�ж��Ƿ���ֱ��
//            g_camera_mid_err = Camera_Get_MidErr();
////            printf("%d\r\n",g_camera_mid_err);
////            printf("%d ,%d ,%d\r\n ",target_left,target_right,g_camera_mid_err);
//            camera_process_cnt++;
//            camera_process_cnt_show++;
//        }
////        pro_time = stop_time - start_time ;

    }
}

