/* Copyright 2016 Amit Dhiman
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* Dated : 24-02-2017
* Author : Amit Dhiman <amitdhiman000@gmail.com>
* Description : Debug macros
*/
#ifndef __DEBUG_AMU__
#define __DEBUG_AMU__

#include <cstdio>

#define FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define PRINT(fmt,args...) printf("%s:%s(%d) " fmt "\n", FILENAME, __PRETTY_FUNCTION__, __LINE__, ##args);
#define APP_LOGV PRINT
#define APP_LOGD PRINT
#define APP_LOGI PRINT
#define APP_LOGW PRINT
#define APP_LOGE PRINT

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
		APP_LOGI(FMT,##ARGS); \
		return; \
	} \
}

#define RETV_MSG_IF(COND,VAL,FMT,ARGS...) { \
	if (COND) { \
		APP_LOGI(FMT,##ARGS); \
		return VAL; \
	} \
}

#endif /* __DEBUG_AMU__ */
