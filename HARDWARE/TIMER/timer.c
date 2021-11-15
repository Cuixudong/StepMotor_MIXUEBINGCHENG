#include "timer.h"
#include "led.h"
#include "usart.h"
#include "delay.h"

void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值  计数到5000为500ms
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
}

void TIM3_IRQHandler(void)   //TIM3中断
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源
        LED1=!LED1;
    }
}

void TIM3_PWM_Init(u16 arr,u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //使能定时器3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟

    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5

    //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形 GPIOB.5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO

    //初始化TIM3
    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    //初始化TIM3 Channel2 PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器

    TIM_Cmd(TIM3, ENABLE);  //使能TIM3
}

// 定义乐曲：《最炫民族风》
const tNote MyScore[294]=
{
    {L6,TT/4},{L3,TT/8},{L5,TT/8},{L6,TT/4},{L6,TT/8},{M1,TT/8},{M1,TT/4},{M2,TT/8},{M1,TT/8},{L6,TT/2},{M1,TT/4},{M1,TT/8},{L5,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/8},{M5,TT/8},
    {M5,TT/8},{M3,TT/8},{M2,TT/4},{M3,TT/2},{M6,TT/8},{M6,TT/8},{M6,TT/8},{M5,TT/8},{M3,TT/8},{M3,TT/4},{M1,TT/8},{L6,TT/8},{L6,TT/8},{L6,TT/8},{M3,TT/8},//苍茫的天涯是我的爱。。。最呀最摇
    {M2,TT/2},{M3,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},{L6,TT/4},{L5,TT/4},{L6,TT/2}, //摆，什么样的歌声才是最开怀
    {L6,TT/4},{L3,TT/8},{L5,TT/8},{L6,TT/4},{L6,TT/8},{M1,TT/8},{M1,TT/4},{M2,TT/8},{M1,TT/8},{L6,TT/2},{M1,TT/4},{M1,TT/8},{L5,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/8},{M5,TT/8},
    {M5,TT/8},{M3,TT/8},{M2,TT/4},{M3,TT/2},{M6,TT/8},{M6,TT/8},{M6,TT/8},{M5,TT/8},{M3,TT/8},{M3,TT/4},{M1,TT/8},{L6,TT/8},{L6,TT/8},{L6,TT/8},{M3,TT/8},//苍茫的天涯是我的爱。。。最呀最摇
    {M2,TT/2},{M3,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},{L6,TT/4},{L5,TT/4},{L6,TT/2}, //摆，什么样的歌声才是最开怀
    {M3,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M3,TT/8},{M5,TT/8},{M5,TT/8},{M6,TT/8},{H1,TT/8},{M6,TT/8},{M5,TT/4},{M6,TT/2},{L6,TT/4},{L6,TT/8},
    {L5,TT/8},{L6,TT/4},{M1,TT/4},{M2,TT/8},{M3,TT/16},{M2,TT/16},{M1,TT/8},{M2,TT/8},{M3,TT/2},{L6,TT/8},{M6,TT/8},{M6,TT/8},{M5,TT/8},{M2,TT/8},{M3,TT/16},{M2,TT/16},
    {M1,TT/8},{M2,TT/8},{M3,TT/2},//留下来
    {M1,TT/8},{L6,TT/8},{L6,TT/8},{M1,TT/8},{M2,TT/4},{L5,TT/8},{L5,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/2},{L6,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},
    {L5,TT/8},{L3,TT/8},{L6,TT/2},{L6,TT/4},{L6,TT/8},{L5,TT/8},{L6,TT/4},{M1,TT/4},{M2,TT/8},{M3,TT/16},{M2,TT/16},{M1,TT/8},{M2,TT/8},{M3,TT/2},{L6,TT/8},{M6,TT/8},
    {M6,TT/8},{M5,TT/8},{M2,TT/8},{M3,TT/16},{M2,TT/16},
    {M1,TT/8},{M2,TT/8},{M3,TT/2},//留下来
    {M1,TT/8},{L6,TT/8},{L6,TT/8},{M1,TT/8},{M2,TT/4},{L5,TT/8},{L5,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/4+TT/8},{M1,TT/8},{L6,TT/8},
    {M1,TT/8},{M2,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M3,TT/8},{M5,TT/8},{M6,TT/2},{M6,TT/2},{L6,TT/4},{L6,TT/8},{L5,TT/8},{L6,TT/4},
    {L6,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/16},{M2,TT/16},{M1,TT/8},{M2,TT/8},{M3,TT/2},
    {M6,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},{M1,TT/2},//登上天外云霄的舞台
    {L6,TT/4},{L6,TT/8},{L5,TT/8},{L6,TT/4},{M1,TT/4},{M2,TT/8},{M3,TT/16},{M2,TT/16},{M1,TT/8},{M2,TT/8},{M3,TT/2},{L6,TT/8},{M6,TT/8},{M6,TT/8},{M5,TT/8},{M2,TT/8},{M3,TT/16},{M2,TT/16},
    {M1,TT/8},{M2,TT/8},{M3,TT/2},//留下来
    {M1,TT/8},{L6,TT/8},{L6,TT/8},{M1,TT/8},{M2,TT/4},{L5,TT/8},{L5,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/2},{L6,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},
    {L5,TT/8},{L3,TT/8},{L6,TT/2},{L6,TT/4},{L6,TT/8},{L5,TT/8},{L6,TT/4},{M1,TT/4},{M2,TT/8},{M3,TT/16},{M2,TT/16},{M1,TT/8},{M2,TT/8},{M3,TT/2},{L6,TT/8},{M6,TT/8},
    {M6,TT/8},{M5,TT/8},{M2,TT/8},{M3,TT/16},{M2,TT/16},
    {M1,TT/8},{M2,TT/8},{M3,TT/2},//留下来
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
    
    //你爱我呀我爱你，蜜雪冰城甜蜜蜜
    {H3,TT/8},{H5,TT/8},{H5,TT/8},{H6,TT/16},
    {H5,TT/8},{H3,TT/8},{H1,TT/8},{H1,TT/16},{H2,TT/16},
    {H3,TT/8},{H3,TT/8},{H2,TT/8},{H1,TT/8},
    {H2,TT/2},
    //你爱我呀我爱你，蜜雪冰城甜蜜蜜
    {H3,TT/8},{H5,TT/8},{H5,TT/8},{H6,TT/16},
    {H5,TT/8},{H3,TT/8},{H1,TT/8},{H1,TT/16},{H2,TT/16},
    {H3,TT/8},{H3,TT/8},{H2,TT/8},{H2,TT/8},
    {H1,TT/2},
    //你爱我呀我爱你
    {H4,TT/4},{H4,TT/4},
    {H4,TT/8},{H6,TT/8},{H6,TT/4},
    {H5,TT/4},{H5,TT/8},{H3,TT/8},
    {H2,TT/2},
    //你爱我我爱你蜜雪冰城甜蜜蜜
    {H3,TT/8},{H5,TT/8},{H5,TT/8},{H6,TT/16},
    {H5,TT/8},{H3,TT/8},{H1,TT/8},{H1,TT/16},{H2,TT/16},
    {H3,TT/8},{H3,TT/8},{H2,TT/8},{H2,TT/8},
    {H1,TT/2},
    
    //梅开二度
    
    //你爱我呀我爱你，蜜雪冰城甜蜜蜜
    {H3,TT/8},{H5,TT/8},{H5,TT/8},{H6,TT/16},
    {H5,TT/8},{H3,TT/8},{H1,TT/8},{H1,TT/16},{H2,TT/16},
    {H3,TT/8},{H3,TT/8},{H2,TT/8},{H1,TT/8},
    {H2,TT/2},
    //你爱我呀我爱你，蜜雪冰城甜蜜蜜
    {H3,TT/8},{H5,TT/8},{H5,TT/8},{H6,TT/16},
    {H5,TT/8},{H3,TT/8},{H1,TT/8},{H1,TT/16},{H2,TT/16},
    {H3,TT/8},{H3,TT/8},{H2,TT/8},{H2,TT/8},
    {H1,TT/2},
    //你爱我呀我爱你
    {H4,TT/4},{H4,TT/4},
    {H4,TT/8},{H6,TT/8},{H6,TT/4},
    {H5,TT/4},{H5,TT/8},{H3,TT/8},
    {H2,TT/2},
    //你爱我我爱你蜜雪冰城甜蜜蜜
    {H3,TT/8},{H5,TT/8},{H5,TT/8},{H6,TT/16},
    {H5,TT/8},{H3,TT/8},{H1,TT/8},{H1,TT/16},{H2,TT/16},
    {H3,TT/8},{H3,TT/8},{H2,TT/8},{H2,TT/8},
    {H1,TT/2},
    
    //梅开三度
    
    //你爱我呀我爱你，蜜雪冰城甜蜜蜜
    {H3,TT/8},{H5,TT/8},{H5,TT/8},{H6,TT/16},
    {H5,TT/8},{H3,TT/8},{H1,TT/8},{H1,TT/16},{H2,TT/16},
    {H3,TT/8},{H3,TT/8},{H2,TT/8},{H1,TT/8},
    {H2,TT/2},
    //你爱我呀我爱你，蜜雪冰城甜蜜蜜
    {H3,TT/8},{H5,TT/8},{H5,TT/8},{H6,TT/16},
    {H5,TT/8},{H3,TT/8},{H1,TT/8},{H1,TT/16},{H2,TT/16},
    {H3,TT/8},{H3,TT/8},{H2,TT/8},{H2,TT/8},
    {H1,TT/2},
    //你爱我呀我爱你
    {H4,TT/4},{H4,TT/4},
    {H4,TT/8},{H6,TT/8},{H6,TT/4},
    {H5,TT/4},{H5,TT/8},{H3,TT/8},
    {H2,TT/2},
    //你爱我我爱你蜜雪冰城甜蜜蜜
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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);    //使能GPIO外设和AFIO复用功能模块时钟使能

    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);            //Timer3部分重映射  TIM3_CH2->PB5
    //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                       //TIM_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                 //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    TIM_TimeBaseStructure.TIM_Period = arr;                         //设置在下一个更新事件装入活动的自动重装载寄存器周期的值  80K
    TIM_TimeBaseStructure.TIM_Prescaler =psc;                       //设置用来作为TIMx时钟频率除数的预分频值  不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                    //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                 //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位


    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;               //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //比较输出使能
    TIM_OCInitStructure.TIM_Pulse = 0;                              //设置待装入捕获比较寄存器的脉冲值
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;        //输出极性:TIM输出比较极性低
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);                        //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);               //使能TIMx在CCR2上的预装载寄存器
    TIM_ARRPreloadConfig(TIM3, ENABLE);                             //使能TIMx在ARR上的预装载寄存器
    TIM_Cmd(TIM3, ENABLE);                                          //使能TIMx外设
}

// 蜂鸣器停止发声
void buzzerQuiet(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    TIM_Cmd(TIM3, DISABLE);        //停止TIM3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;   //PB.5 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);    //根据设定参数初始化GPIOB.5
    GPIO_ResetBits(GPIOB,GPIO_Pin_5);     //PB.5 输出低
}

//蜂鸣器发出指定频率的声音
//usFreq是发声频率，取值 (系统时钟/65536)+1 ～ 20000，单位：Hz
void buzzerSound(unsigned short usFreq)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    unsigned long  ulVal;
    if((usFreq<=8000000/65536UL)||(usFreq>20000))
    {
        buzzerQuiet();           // 蜂鸣器静音
    }
    else
    {
        GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);    //Timer3部分重映射  TIM3_CH2->PB5
        //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形 GPIOB.5
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;               //TIM_CH2
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //复用推挽输出
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);                  //初始化GPIO
        ulVal=8000000/usFreq;
        TIM3->ARR =ulVal ;              //设置自动重装载寄存器周期的值（音调）
        TIM_SetCompare2(TIM3,ulVal /2); //音量
        TIM_Cmd(TIM3, ENABLE);          //启动TIM3
    }
}

// 演奏乐曲
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
        buzzerQuiet();  // 蜂鸣器静音
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
        buzzerQuiet();  // 蜂鸣器静音
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
        buzzerQuiet();  // 蜂鸣器静音
    }
}
