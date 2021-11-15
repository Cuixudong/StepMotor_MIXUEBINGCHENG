#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"

int main(void)
{
    delay_init();           //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(115200);      //���ڳ�ʼ��Ϊ115200
    LED_Init();             //LED�˿ڳ�ʼ��
    TIM3_PWM_Init(899,0);   //����Ƶ��PWMƵ��=72000000/900=80Khz

    musicPlay();
    while(1)
    {
    }
}
