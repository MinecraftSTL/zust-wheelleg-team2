#include "MyHeadfile.h"

int g_camera_mid_err;

Image image;
Image image1;
BinImage binImage;
Image cutShowImage;
Rgb565Image showImage;

uint16 lStartPoint[2];
uint16 rStartPoint[2];

uint16 lLine[MT9V03X_W*3][2];
uint8 lLineDir[MT9V03X_W*3];
uint16 lLineL = 0;
uint16 rLine[MT9V03X_W*3][2];
uint8 rLineDir[MT9V03X_W*3];
uint16 rLineL = 0;
uint16 lrMeet[2];
uint8 L_Stop_Flag = 0;
uint8 R_Stop_Flag = 0;
uint16 lBorder[MT9V03X_H];
uint16 rBorder[MT9V03X_H];
uint16 mLine[MT9V03X_H];

uint8 camera_process_cnt = 0;

int camera_err = 0;              //����ͷ����ƫ��



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
    if(camera_process_cnt){
        camera_process_cnt=0;
        Image_cut(&image, &cutShowImage, binImage.r, binImage.r);
        Image_toRgb565Image(&cutShowImage, &showImage);
        if(lStartPoint[0]&&rStartPoint[0]){
            Rgb565Image_set(&showImage, lStartPoint[0], lStartPoint[1], RGB565_RED);
            Rgb565Image_set(&showImage, rStartPoint[0], rStartPoint[1], RGB565_RED);
        }
//        for(uint16 i=0; i<lLineL; ++i){
//            Rgb565Image_set(&showImage, lLine[i][0], lLine[i][1], RGB565_YELLOW);
//        }
//        for(uint16 i=0; i<rLineL; ++i){
//            Rgb565Image_set(&showImage, rLine[i][0], rLine[i][1], RGB565_YELLOW);
//        }
        for(uint16 i=0; i<showImage.h; ++i){
            Rgb565Image_set(&showImage, i, lBorder[i], RGB565_GREEN);
        }
        for(uint16 i=0; i<showImage.h; ++i){
            Rgb565Image_set(&showImage, i, rBorder[i], RGB565_GREEN);
        }
        for(uint16 i=0; i<showImage.h; ++i){
            Rgb565Image_set(&showImage, i, mLine[i], RGB565_BLUE);
        }
        ips200_show_rgb565_image(0, start_y, showImage.image, showImage.w, showImage.h, showImage.w, showImage.h, 0);
    }
}
uint8 inline Image_get(Image *this, uint16 y, uint16 x){
    return this->image[x + this->w*y];
}
void inline Image_set(Image *this, uint16 y, uint16 x, uint8 value){
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
void Image_toRgb565Image(Image *this, Rgb565Image *target){
    target->h = this->h;
    target->w = this->w;
    for(uint32 i = 0; i < this->h*this->w; ++i){
        target->image[i] = this->image[i]>>(8-5)<<(5+6)|this->image[i]>>(8-6)<<5|this->image[i]>>(8-5);
    }
}
void inline Rgb565Image_set(Rgb565Image *this, uint16 y, uint16 x, uint16 value){
    this->image[x + this->w*y] = value;
}
void Image_binaryzation(Image *this, Image *target, uint16 r, int16 deltaT){
    zf_assert(this && target);
    zf_assert(2 * r < this->h && 2 * r < this->w); // ȷ��ԭͼ�㹻��
    target->h = this->h - r*2;
    target->w = this->w - r*2;
    BinImage binImage;
    BinImage_init(&binImage, this, r, deltaT);
    for(uint16 i = 0; i < target->h; ++i){
        for(uint16 j = 0; j < target->w; ++j){
            Image_set(target,i,j,BinImage_get(&binImage, i, j)?0xFF:0);
        }
    }
}
void BinImage_init(BinImage *this, Image *image, uint16 r, int16 deltaT){
    this->image = image;
    this->h = image->h - r*2;
    this->w = image->w - r*2;
    this->r = r;
    this->deltaT = deltaT;
}
uint8 BinImage_get(BinImage *this, uint16 y, uint16 x){
    zf_assert(this && this->image && y < this->h && x < this->w);
    uint64 sum = 0;
    for(uint16 i = y; i<=y+2*this->r; ++i){
        for(uint16 j = x; j<=x+2*this->r; ++j){
            sum += Image_get(this->image,i,j);
        }
    }
    int16 threshold = sum/((this->r*2+1)*(this->r*2+1));
    threshold+=this->deltaT;
    return Image_get(this->image,y+this->r,x+this->r)>threshold;
}
uint8 Bly_BinImage_get(BinImage *this, uint16 y, uint16 x){
    if(y < 2 || x < 2 || y>=this->h-2 || x >= this->w-2){
        return 0;
    }
    return BinImage_get(this, y, x);
}
void BinImage_getStartPoint(BinImage *this, uint16 lStartPoint[2], uint16 rStartPoint[2]){
    uint16 y = this->h-3;
    uint16 x = this->w/2;
    for(uint16 i = x; i >= 1; --i){
        if(!BinImage_get(this, y, i) && BinImage_get(this, y, i+1) && !BinImage_get(this, y, i-1) && BinImage_get(this, y, i+2)){
            lStartPoint[0]=y;
            lStartPoint[1]=i;
            break;
        }
    }
    for(uint16 i = x-1; i < this->w-1; ++i){
        if(!BinImage_get(this, y, i) && BinImage_get(this, y, i-1) && !BinImage_get(this, y, i+1) && BinImage_get(this, y, i-2)){
            rStartPoint[0]=y;
            rStartPoint[1]=i;
            break;
        }
    }
}

void BinImage_bly(BinImage *this, uint16 maxL, uint16 lLine[MT9V03X_W*3][2], uint16 rLine[MT9V03X_W*3][2],
        uint8 lDir[MT9V03X_W*3], uint8 rDir[MT9V03X_W*3], uint16 *lLineL, uint16 *rLineL, uint16 *meet,
        uint16 lStart[2], uint16 rStart[2])
{

    uint8 i = 0, j = 0;
    //��߱���
    uint16 search_filds_l[8][2] = { { 0 } };
    uint16 center_point_l[2] = {  0 };
    uint16 index_l = 0;
    uint16 temp_l[8][2] = { {  0 } };
    uint16 l_data_statics;//ͳ�����
    //����˸�����
    static int8 seeds_l[8][2] = { {1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1} };
    //        static int8 seeds_l[8][2] = { {-1,0}, {-1,-1}, {0,-1}, {1,-1}, {+1,0}, {1,1}, {0,1}, {-1,1} };
    //{-1,-1},{0,-1},{+1,-1},
    //{-1, 0},       {+1, 0},
    //{-1,+1},{0,+1},{+1,+1},
    //�����˳ʱ��

    //�ұ߱���
    uint16 search_filds_r[8][2] = { {  0 } };
    uint16 center_point_r[2] = { 0 };//���������
    uint16 index_r = 0;//�����±�
    uint16 temp_r[8][2] = { {  0 } };
    uint16 r_data_statics;//ͳ���ұ�
    //����˸�����
    static int8 seeds_r[8][2] = { {1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}, };
    //        static int8 seeds_r[8][2] = { {1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1} };
    //        static int8 seeds_l[8][2] = { {1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1} };
    //        static int8 seeds_r[8][2] = { {-1,0}, {-1,-1}, {0,-1}, {1,-1}, {+1,0}, {1,1}, {0,1}, {-1,1} };
    //{-1,-1},{0,-1},{+1,-1},
    //{-1, 0},       {+1, 0},
    //{-1,+1},{0,+1},{+1,+1},
    //�������ʱ��

    l_data_statics = 0;//ͳ���ҵ��˶��ٸ��㣬��������ѵ�ȫ��������
    r_data_statics = 0;//ͳ���ҵ��˶��ٸ��㣬��������ѵ�ȫ��������

    //��һ�θ��������  ���ҵ������ֵ������
    center_point_l[0] = lStart[0];//x
    center_point_l[1] = lStart[1];//y
    center_point_r[0] = rStart[0];//x
    center_point_r[1] = rStart[1];//y

       //��������ѭ��
    while (maxL--)
    {

       //���
       for (i = 0; i < 8; i++)//����8F����
       {
           search_filds_l[i][0] = center_point_l[0] + seeds_l[i][0];//x
           search_filds_l[i][1] = center_point_l[1] + seeds_l[i][1];//y
       }
       //�����������䵽�Ѿ��ҵ��ĵ���
       lLine[l_data_statics][0] = center_point_l[0];//x
       lLine[l_data_statics][1] = center_point_l[1];//y
       l_data_statics++;//������һ

       //�ұ�
       for (i = 0; i < 8; i++)//����8F����
       {
           search_filds_r[i][0] = center_point_r[0] + seeds_r[i][0];//x
           search_filds_r[i][1] = center_point_r[1] + seeds_r[i][1];//y
       }
       //�����������䵽�Ѿ��ҵ��ĵ���
       rLine[r_data_statics][0] = center_point_r[0];//x
       rLine[r_data_statics][1] = center_point_r[1];//y


       index_l = 0;//�����㣬��ʹ��
       for (i = 0; i < 8; i++)
       {
           temp_l[i][0] = 0;//�����㣬��ʹ��
           temp_l[i][1] = 0;//�����㣬��ʹ��
       }

       //����ж�
       for (i = 0; i < 8; i++)
       {
           if(search_filds_l[(i + 1) & 7][0] >= this->h){
               continue;
           }
           if (!Bly_BinImage_get(this,search_filds_l[i][0],search_filds_l[i][1])
               && Bly_BinImage_get(this,search_filds_l[(i + 1) & 7][0],search_filds_l[(i + 1) & 7][1]))
           {
               temp_l[index_l][0] = search_filds_l[(i)][0];
               temp_l[index_l][1] = search_filds_l[(i)][1];
               index_l++;
               lDir[l_data_statics - 1] = (i);//��¼��������
           }

           if (index_l)
           {
               //���������
               center_point_l[0] = temp_l[0][0];//x
               center_point_l[1] = temp_l[0][1];//y
               for (j = 0; j < index_l; j++)
               {
                   if (center_point_l[1] > temp_l[j][1])
                   {
                       center_point_l[0] = temp_l[j][0];//x
                       center_point_l[1] = temp_l[j][1];//y
                   }
               }
           }

       }
       if ((rLine[r_data_statics][0]== rLine[r_data_statics-1][0]&& rLine[r_data_statics][0] == rLine[r_data_statics - 2][0]
           && rLine[r_data_statics][1] == rLine[r_data_statics - 1][1] && rLine[r_data_statics][1] == rLine[r_data_statics - 2][1])
           ||(lLine[l_data_statics-1][0] == lLine[l_data_statics - 2][0] && lLine[l_data_statics-1][0] == lLine[l_data_statics - 3][0]
               && lLine[l_data_statics-1][1] == lLine[l_data_statics - 2][1] && lLine[l_data_statics-1][1] == lLine[l_data_statics - 3][1]))
       {
    //                printf("���ν���ͬһ���㣬�˳�\n");
           break;
       }
       if (abs(rLine[r_data_statics][0] - lLine[l_data_statics - 1][0]) < 2
           && abs(rLine[r_data_statics][1] - lLine[l_data_statics - 1][1] < 2)
           )
       {
    //                printf("\n���������˳�\n");
           *meet = (rLine[r_data_statics][1] + lLine[l_data_statics - 1][1]) >> 1;//ȡ����ߵ�
    //                printf("\n��y=%d���˳�\n",*Endline);
           break;
       }
       if ((rLine[r_data_statics][1] < lLine[l_data_statics - 1][1]))
       {
    //                printf("\n�����߱��ұ߸��ˣ���ߵȴ��ұ�\n");
           continue;//�����߱��ұ߸��ˣ���ߵȴ��ұ�
       }
       if (lDir[l_data_statics - 1] == 7
           && (rLine[r_data_statics][1] > lLine[l_data_statics - 1][1]))//��߱��ұ߸����Ѿ�����������
       {
    //                printf("\n��߿�ʼ�����ˣ��ȴ��ұߣ��ȴ���... \n");
           center_point_l[0] = (uint8)lLine[l_data_statics - 1][0];//x
           center_point_l[1] = (uint8)lLine[l_data_statics - 1][1];//y
           l_data_statics--;
       }
       r_data_statics++;//������һ

       index_r = 0;//�����㣬��ʹ��
       for (i = 0; i < 8; i++)
       {
           temp_r[i][0] = 0;//�����㣬��ʹ��
           temp_r[i][1] = 0;//�����㣬��ʹ��
       }

       //�ұ��ж�
       for (i = 0; i < 8; i++)
       {
           if(search_filds_r[(i + 1) & 7][0] >= this->h){
               continue;
           }
           if (!Bly_BinImage_get(this,search_filds_r[i][0],search_filds_r[i][1])
               && Bly_BinImage_get(this,search_filds_r[(i + 1) & 7][0],search_filds_r[(i + 1) & 7][1]))
           {
               temp_r[index_r][0] = search_filds_r[(i)][0];
               temp_r[index_r][1] = search_filds_r[(i)][1];
               index_r++;//������һ
               rDir[r_data_statics - 1] = (i);//��¼��������
    //                    printf("dir[%d]:%d\n", r_data_statics - 1, dir_r[r_data_statics - 1]);
           }
           if (index_r)
           {

               //���������
               center_point_r[0] = temp_r[0][0];//x
               center_point_r[1] = temp_r[0][1];//y
               for (j = 0; j < index_r; j++)
               {
                   if (center_point_r[1] > temp_r[j][1])
                   {
                       center_point_r[0] = temp_r[j][0];//x
                       center_point_r[1] = temp_r[j][1];//y
                   }
               }

           }
       }
    }

    //ȡ��ѭ������
    *lLineL = l_data_statics;
    *rLineL = r_data_statics;
}
void BinImage_blyToBorder(BinImage *this, uint16 line[MT9V03X_W*3][2], uint16 lineL, uint8 dir, uint16 ret[MT9V03X_H]){
    for(uint16 i=0; i<this->h; ++i){
        ret[i] = dir?(this->w-1):0;
    }
    for(uint16 i=0; i<lineL; ++i){
        uint16 y = line[i][0];
        if(dir?(line[i][1] < ret[y]):(line[i][1] > ret[y])){
            ret[y]=line[i][1];
        }
    }
}
void BinImage_borderToMiddle(BinImage *this, uint16 lBorder[MT9V03X_H], uint16 rBorder[MT9V03X_H], uint16 ret[MT9V03X_H]){
    for(uint16 i=0; i<this->h; ++i){
        ret[i] = (lBorder[i]+rBorder[i])>>1;
    }
}

void Image_Process(void)
{
    if(mt9v03x_finish_flag == 1)              //�ж�һ��ͼ���Ƿ�������
    {
        Image_fromCamera(&image, mt9v03x_image);               //ͼ��ת�� 37nsһ֡
        mt9v03x_finish_flag = 0;
//        Image_cut(&image, &image1,0,0);
//        Image_zoom(&image1, &image,1);
        BinImage_init(&binImage, &image, 3, -4);
        lStartPoint[0]=rStartPoint[0]=0;
        BinImage_getStartPoint(&binImage, lStartPoint, rStartPoint);
        if(lStartPoint[0]&&rStartPoint[0]){
            BinImage_bly(&binImage, binImage.h*3, lLine, rLine, lLineDir, rLineDir,
                    &lLineL, &rLineL, lrMeet, lStartPoint, rStartPoint);
            BinImage_blyToBorder(&binImage, lLine, lLineL, 0, lBorder);
            BinImage_blyToBorder(&binImage, rLine, rLineL, 1, rBorder);
            BinImage_borderToMiddle(&binImage, lBorder, rBorder, mLine);
        }
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
        camera_process_cnt=1;
    }
}

