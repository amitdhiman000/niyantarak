/*
 * Log.h
 *
 *  Created on: 16-Oct-2016
 *      Author: amu
 */

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <cstdio>

#define PRINT(fmt,args...) printf("%s:%s(%d) " fmt "\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, ##args);
#define LOGD PRINT
#define LOGI PRINT
#define LOGW PRINT
#define LOGE PRINT

#define RET_IF(COND) { \
	if (COND) { \
		return; \
	} \
}

#define RETV_IF(COND,VAL) { \
	if (COND) { \
		return VAL; \
	} \
}

#define RET_MSG_IF(COND,FMT,ARGS...) { \
	if (COND) { \
		LOGI(FMT,##ARGS); \
		return; \
	} \
}

#define RETV_MSG_IF(COND,VAL,FMT,ARGS...) { \
	if (COND) { \
		LOGI(FMT,##ARGS); \
		return VAL; \
	} \
}

#endif /* __DEBUG_H__ */
