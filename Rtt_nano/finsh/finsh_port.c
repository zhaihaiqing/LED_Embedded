/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */

#include <rthw.h>
#include <rtconfig.h>
#include "main.h"

#ifndef RT_USING_FINSH
#error Please uncomment the line <#include "finsh_config.h"> in the rtconfig.h 
#endif

#ifdef RT_USING_FINSH

RT_WEAK char rt_hw_console_getchar(void)
{
	/* Note: the initial value of ch must < 0 */
    int ch = -1;
	
	if(USART_GetITStatus(LOG_USART, USART_IT_RXNE) != RESET)
	{
		ch = USART_ReceiveData(LOG_USART);
	}
	else
	{
		if(USART_GetFlagStatus(LOG_USART, USART_FLAG_ORE) != RESET)
        {
			USART_ClearFlag(LOG_USART,USART_FLAG_ORE);
        }
        rt_thread_mdelay(5);
	}

    return ch;
}

#endif /* RT_USING_FINSH */

