/*
 * Copyright (c) 2014-2016 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef _APP_ASSIST_WTIMER_IMPL_H_
#define _APP_ASSIST_WTIMER_IMPL_H_

#include <Ecore.h>
#include "WTimer.h"

namespace app_assist {

class _WTimerImpl {
public:
	_WTimerImpl();
	~_WTimerImpl();

private:
	void* _timer;
	WTimer::Type _type;

	void* _userData;
	std::shared_ptr<WTimer>* _selfPtr;
	Evas_Object* _ownerObj;

	std::function<bool (void*)> _timerFunc;
	std::function<void (void*)> _jobFunc;

	void _deleteTimer();

	static void _ownerObjDelCb(void* data, Evas* evas, Evas_Object* obj, void* event_info);
	static Eina_Bool _timerCb(void* data);
	static void _jobCb(void* data);

	static Ecore_Idler* getIdlerHandle(const WTimerWeakPtr& uiTimer);
	static Ecore_Animator* getAnimatorHandle(const WTimerWeakPtr& uiTimer);
	static Ecore_Timer* getTimerHandle(const WTimerWeakPtr& uiTimer);
	static Ecore_Job* getJobHandle(const WTimerWeakPtr& uiTimer);

	friend class WTimer;
	friend Ecore_Idler* wuitimer_get_idler_handle(const WTimerWeakPtr& uiTimer);
	friend Ecore_Animator* wuitimer_get_animator_handle(const WTimerWeakPtr& uiTimer);
	friend Ecore_Timer* wuitimer_get_timer_handle(const WTimerWeakPtr& uiTimer);
	friend Ecore_Job* wuitimer_get_job_handle(const WTimerWeakPtr& uiTimer);
};

} // namespace

#endif /* _APP_ASSIST_WTIMER_IMPL_H_ */
