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

#ifndef _APP_ASSIST_WTIMER_H_
#define _APP_ASSIST_WTIMER_H_

#include <Ecore.h>
#include <Evas.h>
#include <functional>
#include "WDefine.h"

namespace app_assist {

class WTimer;

typedef std::weak_ptr<WTimer> WTimerWeakPtr;

/**
* @class	WTimer
* @brief	This class wraps ecore timer module.
*
*
* The %WTimer is the class managing ecore timer
*/
class WAPP_ASSIST_EXPORT WTimer {
public:
	enum class Type {
		IDLER,		///< Can be called multiple times with in one rendering cycle, based on system load.
		ANIMATOR, 	///< Called just once per rendering cycle.
		TIMER,		///< Called just once per rendering cycle, if the "set time" is elapsed before next vsync.
		JOB			///< Called just once per registration prior to timer, idler and animator.
	};

	/**
	 * Returns type of timer.
	 *
	 * @return timer type
	 * @see enum class Type
	 */
	WTimer::Type getType() const;

	/**
	 * Deletes this instance.
	 * The created timer will be deleted together.
	 *
	 * @param[in,out]	timer	the reference type of weak pointer of WTimer instance. timer variable will be reset.
	 *
	 */
	static void destroy(WTimerWeakPtr& timer);

	/**
	 * Creates a IDLER type timer. It uses ecore_idler internally.
	 * It can be called multiple times with in one rendering cycle, based on system load.
	 *
	 * @param[in]	timerFunc	std::function object of worker function
	 * @param[in]	userData	user data
	 * @param[in]	ownerObj	the owner Evas_Object
	 * 							When this Evas Object is deleted, the WTimer will be deleted together, if it is still alive.
	 * 							nullptr is not allowed.
	 *
	 * @return The weak pointer of WTimer instance created. The validity of the pointer can be checked with the lock() method.
	 *         If it is failed or invalid when lock() method is called with this weak pointer will return empty shared_ptr.
	 */
	static WTimerWeakPtr addIdler(std::function<bool (void* userData)> timerFunc, void* userData, Evas_Object* ownerObj);

	/**
	 * Creates a IDLER type timer. It uses ecore_idler internally.
	 * It can be called multiple times with in one rendering cycle, based on system load.
	 *
	 * @param[in]	timerFunc	std::function object of worker function
	 * @param[in]	userData	user data
	 *
	 * @return The weak pointer of WTimer instance created. The validity of the pointer can be checked with the lock() method.
	 *         If it is failed or invalid when lock() method is called with this weak pointer will return empty shared_ptr.
	 *         The returned value should be released by WTimer::destroy().
	 */
	static WTimerWeakPtr addIdler(std::function<bool (void* userData)> timerFunc, void* userData);

	/**
	 * Creates a ANIMATOR type timer. It uses ecore_animator internally.
	 * It will be called just once per rendering cycle.
	 *
	 * @param[in]	timerFunc	std::function object of worker function
	 * @param[in]	userData	user data
	 * @param[in]	ownerObj	the owner Evas_Object
	 * 							When this Evas Object is deleted, the WTimer will be deleted together, if it is still alive.
	 * 							nullptr is not allowed.
	 *
	 * @return The weak pointer of WTimer instance created. The validity of the pointer can be checked with the lock() method.
	 *         If it is failed or invalid when lock() method is called with this weak pointer will return empty shared_ptr.
	 */
	static WTimerWeakPtr addAnimator(std::function<bool (void* userData)> timerFunc, void* userData,  Evas_Object* ownerObj);

	/**
	 * Creates a ANIMATOR type timer. It uses ecore_animator internally.
	 * It will be called just once per rendering cycle.
	 *
	 * @param[in]	timerFunc	std::function object of worker function
	 * @param[in]	userData	user data
	 *
	 * @return The weak pointer of WTimer instance created. The validity of the pointer can be checked with the lock() method.
	 *         If it is failed or invalid when lock() method is called with this weak pointer will return empty shared_ptr.
	 *         The returned value should be released by WTimer::destroy().
	 */
	static WTimerWeakPtr addAnimator(std::function<bool (void* userData)> timerFunc, void* userData);

	/**
	 * Creates a timer. It uses ecore_timer internally.
	 * It will be called just once per rendering cycle, if the "set time" is elapsed before next vsync.
	 *
	 * @param[in]	interval	The interval in seconds.
	 * @param[in]	timerFunc	std::function object of worker function
	 * @param[in]	userData	user data
	 * @param[in]	ownerObj	the owner Evas_Object
	 * 							When this Evas Object is deleted, the WTimer will be deleted together, if it is still alive.
	 * 							nullptr is not allowed.
	 *
	 * @return The weak pointer of WTimer instance created. The validity of the pointer can be checked with the lock() method.
	 *         If it is failed or invalid when lock() method is called with this weak pointer will return empty shared_ptr.
	 */
	static WTimerWeakPtr addTimer(double interval, std::function<bool (void* userData)> timerFunc, void* userData,  Evas_Object* ownerObj);

	/**
	 * Creates a timer. It uses ecore_timer internally.
	 * It will be called just once per rendering cycle, if the "set time" is elapsed before next vsync.
	 *
	 * @param[in]	interval	The interval in seconds.
	 * @param[in]	timerFunc	std::function object of worker function
	 * @param[in]	userData	user data
	 *
	 * @return The weak pointer of WTimer instance created. The validity of the pointer can be checked with the lock() method.
	 *         If it is failed or invalid when lock() method is called with this weak pointer will return empty shared_ptr.
	 *         The returned value should be released by WTimer::destroy().
	 */
	static WTimerWeakPtr addTimer(double interval, std::function<bool (void* userData)> timerFunc, void* userData);

	/**
	 * Creates a JOB type timer. It uses ecore_job internally.
	 * It will be called just once per registration prior to timer, idler and animator.
	 *
	 * @param[in]	timerFunc	std::function object of worker function
	 * @param[in]	userData	user data
	 * @param[in]	ownerObj	the owner Evas_Object
	 * 							When this Evas Object is deleted, the WTimer will be deleted together, if it is still alive.
	 * 							nullptr is not allowed.
	 *
	 * @return The weak pointer of WTimer instance created. The validity of the pointer can be checked with the lock() method.
	 *         If it is failed or invalid when lock() method is called with this weak pointer will return empty shared_ptr.
	 */
	static WTimerWeakPtr addJob(std::function<void (void* userData)> timerFunc, void* userData,  Evas_Object* ownerObj);

	/**
	 * Creates a JOB type timer. It uses ecore_job internally.
	 * It will be called just once per registration prior to timer, idler and animator.
	 *
	 * @param[in]	timerFunc	std::function object of worker function
	 * @param[in]	userData	user data
	 *
	 * @return The weak pointer of WTimer instance created. The validity of the pointer can be checked with the lock() method.
	 *         If it is failed or invalid when lock() method is called with this weak pointer will return empty shared_ptr.
	 *         The returned value should be released by WTimer::destroy().
	 */
	static WTimerWeakPtr addJob(std::function<void (void* userData)> timerFunc, void* userData);

private:
	WTimer();
	~WTimer();

private:
	WDECLARE_PRIVATE_IMPL(WTimer);
	WDISABLE_COPY_AND_ASSIGN(WTimer);
};

/**
 * Gets Ecore_Idler handle pointer.
 * Do not call ecore_idler_del with this returned pointer, because it is internal property of this WTimer instance.
 * To delete the Ecore_Idler, call destroy method of this WTimer class.
 *
 * @param[in]	uiTimer	the reference of WTimer weak pointer
 *
 * @return Ecore_Idler handle pointer
 * @see addIder()
 */
WAPP_ASSIST_EXPORT Ecore_Idler* wuitimer_get_idler_handle(const WTimerWeakPtr& uiTimer);

/**
 * Gets Ecore_Animator handle pointer.
 * Do not call ecore_animator_del with this returned pointer, because it is internal property of this WTimer instance.
 * To delete the Ecore_Animator, call destroy method of this WTimer class.
 *
 * @param[in]	uiTimer	the reference of WTimer weak pointer
 *
 * @return Ecore_Animator handle pointer
 * @see addAnimator()
 */
WAPP_ASSIST_EXPORT Ecore_Animator* wuitimer_get_animator_handle(const WTimerWeakPtr& uiTimer);

/**
 * Gets Ecore_Timer handle pointer.
 * Do not call ecore_timer_del with this returned pointer, because it is internal property of this WTimer instance.
 * To delete the Ecore_Timer, call destroy method of this WTimer class.
 *
 * @param[in]	uiTimer	the reference of WTimer weak pointer
 *
 * @return Ecore_Timer handle pointer
 * @see addTimer()
 */
WAPP_ASSIST_EXPORT Ecore_Timer* wuitimer_get_timer_handle(const WTimerWeakPtr& uiTimer);

/**
 * Gets Ecore_Job handle pointer.
 * Do not call ecore_job_del with this returned pointer, because it is internal property of this WTimer instance.
 * To delete the Ecore_Job, call destroy method of this WTimer class.
 *
 * @param[in]	uiTimer	the reference of WTimer weak pointer
 *
 * @return Ecore_Job handle pointer
 * @see addJob()
 */
WAPP_ASSIST_EXPORT Ecore_Job* wuitimer_get_job_handle(const WTimerWeakPtr& uiTimer);

} // namespace

#endif /* _APP_ASSIST_WTIMER_H_ */
