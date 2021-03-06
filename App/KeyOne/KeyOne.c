/*********************************************************************************************************
* 模块名称：KeyOne.c
* 摘    要：KeyOne模块，进行独立按键初始化，以及按键扫描函数实现
* 当前版本：1.0.0
* 作    者：SZLY(COPYRIGHT 2018 - 2020 SZLY. All rights reserved.)
* 完成日期：2020年01月01日  
* 内    容：
* 注    意：                                                                  
**********************************************************************************************************
* 取代版本：
* 作    者：
* 完成日期：
* 修改内容：
* 修改文件：
*********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "KeyOne.h"
#include "stm32f10x_conf.h"

/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/
//KEY1为读取PC1引脚电平
#define KEY1    (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)) 
//KEY2为读取PC2引脚电平
#define KEY2    (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)) 
//KEY3为读取PA0引脚电平
#define KEY3    (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))  

/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/
//按键按下时的电压，0xFF表示按下为高电平，0x00表示按下为低电平
static  u8  s_arrKeyDownLevel[KEY_NAME_MAX];      //使用前要在InitKeyOne函数中进行初始化   

/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/
static  void  ConfigKeyOneGPIO(void); //配置按键的GPIO 

/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：ConfigKeyOneGPIO
* 函数功能：配置按键的GPIO 
* 输入参数：void 
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
static  void  ConfigKeyOneGPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  //GPIO_InitStructure用于存放GPIO的参数
  
  //使能RCC相关时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能GPIOA的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能GPIOC的时钟
  
  //配置PC1
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;           //设置引脚
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;        //设置输入类型
  GPIO_Init(GPIOC, &GPIO_InitStructure);                //根据参数初始化GPIO
  
  //配置PC2
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;           //设置引脚
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;        //设置输入类型
  GPIO_Init(GPIOC, &GPIO_InitStructure);                //根据参数初始化GPIO

  //配置PA0
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;           //设置引脚
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;        //设置输入类型
  GPIO_Init(GPIOA, &GPIO_InitStructure);                //根据参数初始化GPIO
}

/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：InitKeyOne
* 函数功能：初始化KeyOne模块
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
void InitKeyOne(void)
{
  ConfigKeyOneGPIO(); //配置按键的GPIO 
                                                                
  s_arrKeyDownLevel[KEY_NAME_KEY1] = KEY_DOWN_LEVEL_KEY1;  //按键KEY1按下时为低电平
  s_arrKeyDownLevel[KEY_NAME_KEY2] = KEY_DOWN_LEVEL_KEY2;  //按键KEY2按下时为低电平
  s_arrKeyDownLevel[KEY_NAME_KEY3] = KEY_DOWN_LEVEL_KEY3;  //按键KEY3按下时为低电平
}

/*********************************************************************************************************
* 函数名称：ScanKeyOne
* 函数功能：按键扫描，每10ms调用一次
* 输入参数：keyName-按键名，OnKeyOneUp-按键弹起响应函数的指针，OnKeyOneDown-按键按下响应函数的指针
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：如果s_arrKeyDownLevel[keyName] = 0xFF，对s_arrKeyDownLevel[keyName]直接取反得出的是256，而非0
*           正确的做法是(u8)(~s_arrKeyDownLevel[keyName])，这样得出的才是0。
*********************************************************************************************************/
void ScanKeyOne(u8 keyName, void(*OnKeyOneUp)(void), void(*OnKeyOneDown)(void), void(*OnKeyOneLongDown)(u8 keyName))
{
  static  u8  s_arrKeyVal[KEY_NAME_MAX];    //定义一个u8类型的数组，用于存放按键的数值
  static  u8  s_arrKeyFlag[KEY_NAME_MAX];   //定义一个u8类型的数组，用于存放按键的标志位
	// press time for each key.
	static u16 pressTime[KEY_NAME_MAX];
  // long press flag
	static u8 longPressFlag[KEY_NAME_MAX];
	
  s_arrKeyVal[keyName] = s_arrKeyVal[keyName] << 1;   //左移一位

  switch (keyName)
  {
    case KEY_NAME_KEY1:
      s_arrKeyVal[keyName] = s_arrKeyVal[keyName] | KEY1; //按下/弹起时，KEY1为0/1
      break;                                            
    case KEY_NAME_KEY2:                                 
      s_arrKeyVal[keyName] = s_arrKeyVal[keyName] | KEY2; //按下/弹起时，KEY2为0/1
      break;                                            
    case KEY_NAME_KEY3:                                 
      s_arrKeyVal[keyName] = s_arrKeyVal[keyName] | KEY3; //按下/弹起时，KEY3为0/1
      break;                                            
    default:
      break;
  }  
	// test if current action being a long press
	if(s_arrKeyVal[keyName] == s_arrKeyDownLevel[keyName] && pressTime[keyName] < 65535 && longPressFlag[keyName] == TRUE) {
		// accumulate pressing tiem for every key.
		pressTime[keyName] += 1;
		// pressed for one second, trigger long press event.
		if(pressTime[keyName] > 100) {
			(*OnKeyOneLongDown)(keyName);
			longPressFlag[keyName] = FALSE;
		}
	}
  
  //按键标志位的值为TRUE时，判断是否有按键有效按下
  if(s_arrKeyVal[keyName] == s_arrKeyDownLevel[keyName] && s_arrKeyFlag[keyName] == TRUE)
  {
    (*OnKeyOneDown)();                    //执行按键按下的响应函数
    s_arrKeyFlag[keyName] = FALSE;        //表示按键处于按下状态，按键标志位的值更改为FALSE
  }
  
  //按键标志位的值为FALSE时，判断是否有按键有效弹起
  else if(s_arrKeyVal[keyName] == (u8)(~s_arrKeyDownLevel[keyName]) && s_arrKeyFlag[keyName] == FALSE)
  {
    (*OnKeyOneUp)();                      //执行按键弹起的响应函数
    s_arrKeyFlag[keyName] = TRUE;         //表示按键处于弹起状态，按键标志位的值更改为TRUE
		
		// set press time to zero
		pressTime[keyName] = 0;
		// reset long press flag
		longPressFlag[keyName] = TRUE;
  }
}
