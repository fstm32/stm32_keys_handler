/*********************************************************************************************************
* 模块名称：ProcKeyOne.h
* 摘    要：ProcKeyOne模块，进行独立按键处理模块初始化，以及独立按键处理函数实现
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
#ifndef _PROC_KEY_ONE_H_
#define _PROC_KEY_ONE_H_

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "DataType.h"

/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              API函数声明
*********************************************************************************************************/
void  InitProcKeyOne(void);   //初始化ProcKeyOne模块

void  ProcKeyDownKey1(void);  //处理按键按下的事件，即按键按下的响应函数 
void  ProcKeyUpKey1(void);    //处理按键弹起的事件，即按键弹起的响应函数
void  ProcKeyDownKey2(void);  //处理按键按下的事件，即按键按下的响应函数 
void  ProcKeyUpKey2(void);    //处理按键弹起的事件，即按键弹起的响应函数
void  ProcKeyDownKey3(void);  //处理按键按下的事件，即按键按下的响应函数 
void  ProcKeyUpKey3(void);    //处理按键弹起的事件，即按键弹起的响应函数

// handler function for longing press a key.
void 	ProcKeyLongDownKey(u8 keyName);

#endif
