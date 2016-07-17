#pragma once

#define __ALLOW_DEBUG_STAT_CHANGE		//允许代码修改调试状态

#define __ALLOW_DYMANIC_DEBUG			//允许动态修改调试状态

#ifndef __DEFAULT_DEBUG_STAT
#define __DEFAULT_DEBUG_STAT	true	//默认调试状态
#endif

//#define __WNO_OMP						//禁用OpenMP头

#define __ALLOW_LOWMEM_PREFER			//使用低内存优化配置

#ifndef MTYPE
#define MTYPE	float					//数学库浮点类型
#endif

/*连接*/
#define DEFAULT_DEBUG_STAT  __DEFAULT_DEBUG_STAT
