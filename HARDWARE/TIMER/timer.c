#include "timer.h"
#include "led.h"
#include "usart.h"
#include "delay.h"

void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

    TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ  ������5000Ϊ500ms
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
}

void TIM3_IRQHandler(void)   //TIM3�ж�
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ
        LED1=!LED1;
    }
}

void TIM3_PWM_Init(u16 arr,u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʹ�ܶ�ʱ��3ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��

    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5

    //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨�� GPIOB.5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

    //��ʼ��TIM3
    TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    //��ʼ��TIM3 Channel2 PWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2

    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

    TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
}

// ��������������������硷
const tNote MyScore[294]=
{
    {L6,TT/4},{L3,TT/8},{L5,TT/8},{L6,TT/4},{L6,TT/8},{M1,TT/8},{M1,TT/4},{M2,TT/8},{M1,TT/8},{L6,TT/2},{M1,TT/4},{M1,TT/8},{L5,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/8},{M5,TT/8},
    {M5,TT/8},{M3,TT/8},{M2,TT/4},{M3,TT/2},{M6,TT/8},{M6,TT/8},{M6,TT/8},{M5,TT/8},{M3,TT/8},{M3,TT/4},{M1,TT/8},{L6,TT/8},{L6,TT/8},{L6,TT/8},{M3,TT/8},//��ã���������ҵİ���������ѽ��ҡ
    {M2,TT/2},{M3,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},{L6,TT/4},{L5,TT/4},{L6,TT/2}, //�ڣ�ʲô���ĸ����������
    {L6,TT/4},{L3,TT/8},{L5,TT/8},{L6,TT/4},{L6,TT/8},{M1,TT/8},{M1,TT/4},{M2,TT/8},{M1,TT/8},{L6,TT/2},{M1,TT/4},{M1,TT/8},{L5,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/8},{M5,TT/8},
    {M5,TT/8},{M3,TT/8},{M2,TT/4},{M3,TT/2},{M6,TT/8},{M6,TT/8},{M6,TT/8},{M5,TT/8},{M3,TT/8},{M3,TT/4},{M1,TT/8},{L6,TT/8},{L6,TT/8},{L6,TT/8},{M3,TT/8},//��ã���������ҵİ���������ѽ��ҡ
    {M2,TT/2},{M3,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},{L6,TT/4},{L5,TT/4},{L6,TT/2}, //�ڣ�ʲô���ĸ����������
    {M3,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M3,TT/8},{M5,TT/8},{M5,TT/8},{M6,TT/8},{H1,TT/8},{M6,TT/8},{M5,TT/4},{M6,TT/2},{L6,TT/4},{L6,TT/8},
    {L5,TT/8},{L6,TT/4},{M1,TT/4},{M2,TT/8},{M3,TT/16},{M2,TT/16},{M1,TT/8},{M2,TT/8},{M3,TT/2},{L6,TT/8},{M6,TT/8},{M6,TT/8},{M5,TT/8},{M2,TT/8},{M3,TT/16},{M2,TT/16},
    {M1,TT/8},{M2,TT/8},{M3,TT/2},//������
    {M1,TT/8},{L6,TT/8},{L6,TT/8},{M1,TT/8},{M2,TT/4},{L5,TT/8},{L5,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/2},{L6,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},
    {L5,TT/8},{L3,TT/8},{L6,TT/2},{L6,TT/4},{L6,TT/8},{L5,TT/8},{L6,TT/4},{M1,TT/4},{M2,TT/8},{M3,TT/16},{M2,TT/16},{M1,TT/8},{M2,TT/8},{M3,TT/2},{L6,TT/8},{M6,TT/8},
    {M6,TT/8},{M5,TT/8},{M2,TT/8},{M3,TT/16},{M2,TT/16},
    {M1,TT/8},{M2,TT/8},{M3,TT/2},//������
    {M1,TT/8},{L6,TT/8},{L6,TT/8},{M1,TT/8},{M2,TT/4},{L5,TT/8},{L5,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/4+TT/8},{M1,TT/8},{L6,TT/8},
    {M1,TT/8},{M2,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M3,TT/8},{M5,TT/8},{M6,TT/2},{M6,TT/2},{L6,TT/4},{L6,TT/8},{L5,TT/8},{L6,TT/4},
    {L6,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/16},{M2,TT/16},{M1,TT/8},{M2,TT/8},{M3,TT/2},
    {M6,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},{M1,TT/2},//����������������̨
    {L6,TT/4},{L6,TT/8},{L5,TT/8},{L6,TT/4},{M1,TT/4},{M2,TT/8},{M3,TT/16},{M2,TT/16},{M1,TT/8},{M2,TT/8},{M3,TT/2},{L6,TT/8},{M6,TT/8},{M6,TT/8},{M5,TT/8},{M2,TT/8},{M3,TT/16},{M2,TT/16},
    {M1,TT/8},{M2,TT/8},{M3,TT/2},//������
    {M1,TT/8},{L6,TT/8},{L6,TT/8},{M1,TT/8},{M2,TT/4},{L5,TT/8},{L5,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/2},{L6,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},
    {L5,TT/8},{L3,TT/8},{L6,TT/2},{L6,TT/4},{L6,TT/8},{L5,TT/8},{L6,TT/4},{M1,TT/4},{M2,TT/8},{M3,TT/16},{M2,TT/16},{M1,TT/8},{M2,TT/8},{M3,TT/2},{L6,TT/8},{M6,TT/8},
    {M6,TT/8},{M5,TT/8},{M2,TT/8},{M3,TT/16},{M2,TT/16},
    {M1,TT/8},{M2,TT/8},{M3,TT/2},//������
    {M1,TT/8},{L6,TT/8},{L6,TT/8},{M1,TT/8},{M2,TT/4},{L5,TT/8},{L5,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/4+TT/8},{M1,TT/8},{L6,TT/8},
    {M1,TT/8},{M2,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M3,TT/8},{M5,TT/8},{M6,TT/2},{M6,TT/2},
    {0,0},
};

const tNote MyScoreMXBC[55 * 3 + 1]=
{
    #if 0
    {H3,TT/16},{H4,TT/16},
    {H5,TT/8},{H5,TT/8},{H5,TT/8},{H6,TT/16},
    {H5,TT/8},{H3,TT/8},{H1,TT/8},{H1,TT/16},{H2,TT/16},
    {H3,TT/8},{H3,TT/8},{H2,TT/8},{H2,TT/8},
    {H1,TT/4},
    #endif
    
    //�㰮��ѽ�Ұ��㣬��ѩ����������
    {H3,TT/8},{H5,TT/8},{H5,TT/8},{H6,TT/16},
    {H5,TT/8},{H3,TT/8},{H1,TT/8},{H1,TT/16},{H2,TT/16},
    {H3,TT/8},{H3,TT/8},{H2,TT/8},{H1,TT/8},
    {H2,TT/2},
    //�㰮��ѽ�Ұ��㣬��ѩ����������
    {H3,TT/8},{H5,TT/8},{H5,TT/8},{H6,TT/16},
    {H5,TT/8},{H3,TT/8},{H1,TT/8},{H1,TT/16},{H2,TT/16},
    {H3,TT/8},{H3,TT/8},{H2,TT/8},{H2,TT/8},
    {H1,TT/2},
    //�㰮��ѽ�Ұ���
    {H4,TT/4},{H4,TT/4},
    {H4,TT/8},{H6,TT/8},{H6,TT/4},
    {H5,TT/4},{H5,TT/8},{H3,TT/8},
    {H2,TT/2},
    //�㰮���Ұ�����ѩ����������
    {H3,TT/8},{H5,TT/8},{H5,TT/8},{H6,TT/16},
    {H5,TT/8},{H3,TT/8},{H1,TT/8},{H1,TT/16},{H2,TT/16},
    {H3,TT/8},{H3,TT/8},{H2,TT/8},{H2,TT/8},
    {H1,TT/2},
    
    //÷������
    
    //�㰮��ѽ�Ұ��㣬��ѩ����������
    {H3,TT/8},{H5,TT/8},{H5,TT/8},{H6,TT/16},
    {H5,TT/8},{H3,TT/8},{H1,TT/8},{H1,TT/16},{H2,TT/16},
    {H3,TT/8},{H3,TT/8},{H2,TT/8},{H1,TT/8},
    {H2,TT/2},
    //�㰮��ѽ�Ұ��㣬��ѩ����������
    {H3,TT/8},{H5,TT/8},{H5,TT/8},{H6,TT/16},
    {H5,TT/8},{H3,TT/8},{H1,TT/8},{H1,TT/16},{H2,TT/16},
    {H3,TT/8},{H3,TT/8},{H2,TT/8},{H2,TT/8},
    {H1,TT/2},
    //�㰮��ѽ�Ұ���
    {H4,TT/4},{H4,TT/4},
    {H4,TT/8},{H6,TT/8},{H6,TT/4},
    {H5,TT/4},{H5,TT/8},{H3,TT/8},
    {H2,TT/2},
    //�㰮���Ұ�����ѩ����������
    {H3,TT/8},{H5,TT/8},{H5,TT/8},{H6,TT/16},
    {H5,TT/8},{H3,TT/8},{H1,TT/8},{H1,TT/16},{H2,TT/16},
    {H3,TT/8},{H3,TT/8},{H2,TT/8},{H2,TT/8},
    {H1,TT/2},
    
    //÷������
    
    //�㰮��ѽ�Ұ��㣬��ѩ����������
    {H3,TT/8},{H5,TT/8},{H5,TT/8},{H6,TT/16},
    {H5,TT/8},{H3,TT/8},{H1,TT/8},{H1,TT/16},{H2,TT/16},
    {H3,TT/8},{H3,TT/8},{H2,TT/8},{H1,TT/8},
    {H2,TT/2},
    //�㰮��ѽ�Ұ��㣬��ѩ����������
    {H3,TT/8},{H5,TT/8},{H5,TT/8},{H6,TT/16},
    {H5,TT/8},{H3,TT/8},{H1,TT/8},{H1,TT/16},{H2,TT/16},
    {H3,TT/8},{H3,TT/8},{H2,TT/8},{H2,TT/8},
    {H1,TT/2},
    //�㰮��ѽ�Ұ���
    {H4,TT/4},{H4,TT/4},
    {H4,TT/8},{H6,TT/8},{H6,TT/4},
    {H5,TT/4},{H5,TT/8},{H3,TT/8},
    {H2,TT/2},
    //�㰮���Ұ�����ѩ����������
    {H3,TT/8},{H5,TT/8},{H5,TT/8},{H6,TT/16},
    {H5,TT/8},{H3,TT/8},{H1,TT/8},{H1,TT/16},{H2,TT/16},
    {H3,TT/8},{H3,TT/8},{H2,TT/8},{H2,TT/8},
    {H1,TT/2},
    
    {0,0},
};

void TIM3_Init(u16 arr,u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);    //ʹ��GPIO�����AFIO���ù���ģ��ʱ��ʹ��

    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);            //Timer3������ӳ��  TIM3_CH2->PB5
    //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                       //TIM_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                 //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    TIM_TimeBaseStructure.TIM_Period = arr;                         //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ  80K
    TIM_TimeBaseStructure.TIM_Prescaler =psc;                       //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                    //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                 //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ


    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;               //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_Pulse = 0;                              //���ô�װ�벶��ȽϼĴ���������ֵ
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;        //�������:TIM����Ƚϼ��Ե�
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);                        //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);               //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
    TIM_ARRPreloadConfig(TIM3, ENABLE);                             //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
    TIM_Cmd(TIM3, ENABLE);                                          //ʹ��TIMx����
}

// ������ֹͣ����
void buzzerQuiet(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    TIM_Cmd(TIM3, DISABLE);        //ֹͣTIM3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;   //PB.5 �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);    //�����趨������ʼ��GPIOB.5
    GPIO_ResetBits(GPIOB,GPIO_Pin_5);     //PB.5 �����
}

//����������ָ��Ƶ�ʵ�����
//usFreq�Ƿ���Ƶ�ʣ�ȡֵ (ϵͳʱ��/65536)+1 �� 20000����λ��Hz
void buzzerSound(unsigned short usFreq)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    unsigned long  ulVal;
    if((usFreq<=8000000/65536UL)||(usFreq>20000))
    {
        buzzerQuiet();           // ����������
    }
    else
    {
        GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);    //Timer3������ӳ��  TIM3_CH2->PB5
        //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨�� GPIOB.5
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;               //TIM_CH2
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //�����������
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);                  //��ʼ��GPIO
        ulVal=8000000/usFreq;
        TIM3->ARR =ulVal ;              //�����Զ���װ�ؼĴ������ڵ�ֵ��������
        TIM_SetCompare2(TIM3,ulVal /2); //����
        TIM_Cmd(TIM3, ENABLE);          //����TIM3
    }
}

// ��������
void musicPlay(void)
{
    uint16_t i=0;
    while(i<51)
    {
        if (MyScoreMXBC[i].mTime == 0)
            break;
        buzzerSound(MyScoreMXBC[i].mName);
        delay_ms(MyScoreMXBC[i].mTime);
        i++;
        buzzerQuiet();  // ����������
    }
    delay_ms(600);
    i=0;
    while(i<51)
    {
        if (MyScoreMXBC[i].mTime == 0)
            break;
        buzzerSound(MyScoreMXBC[i].mName);
        delay_ms(MyScoreMXBC[i].mTime);
        i++;
        buzzerQuiet();  // ����������
    }
    delay_ms(600);
    i=0;
    while(i<51)
    {
        if (MyScoreMXBC[i].mTime == 0)
            break;
        buzzerSound(MyScoreMXBC[i].mName);
        delay_ms(MyScoreMXBC[i].mTime);
        i++;
        buzzerQuiet();  // ����������
    }
}
