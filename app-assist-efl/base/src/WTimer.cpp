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

#include "WTimer.h"
#include "WDebugInternal.h"
#include "WDefineInternal.h"
#include "WTimerImpl.h"

using namespace app_assist;

WTimer::WTimer() {
	WHIT();
	_pv = new _WTimerImpl();
}

WTimer::~WTimer() {
	WHIT();
	if (_pv->_ownerObj) {
		evas_object_event_callback_del_full(_pv->_ownerObj, EVAS_CALLBACK_DEL, _WTimerImpl::_ownerObjDelCb, this);
	}
	delete _pv;
}

WTimer::Type WTimer::getType() const {
	return _pv->_type;
}

void WTimer::destroy(WTimerWeakPtr& timer) {
	if (auto p = timer.lock()) {
		delete p.get();
	}
	timer.reset();
}

WTimerWeakPtr WTimer::addIdler(std::function<bool (void*)> timerFunc, void* userData,  Evas_Object* ownerObj) {
	WASSERT_EX(ownerObj != nullptr, "ownerObj is null.");

	WTimerWeakPtr timerPtr = WTimer::addIdler(timerFunc, userData);
	std::shared_ptr<WTimer> timer = timerPtr.lock();
	timer->_pv->_ownerObj = ownerObj;

	evas_object_event_callback_add(ownerObj, EVAS_CALLBACK_DEL, _WTimerImpl::_ownerObjDelCb, timer.get());

	timer.reset();

	return timerPtr;
}

WTimerWeakPtr WTimer::addIdler(std::function<bool (void*)> timerFunc, void* userData) {
	WTimer* timer = new WTimer();
	timer->_pv->_selfPtr = new std::shared_ptr<WTimer>(timer, [](WTimer * p) {});
	timer->_pv->_type = WTimer::Type::IDLER;
	timer->_pv->_timerFunc = timerFunc;
	timer->_pv->_userData = userData;
	Ecore_Idler* ecoreIdler = ecore_idler_add(_WTimerImpl::_timerCb, timer);
	if (ecoreIdler == nullptr) {
		WERROR("failed to add ecore idler!");
		delete timer;
		return std::weak_ptr<WTimer>();
	}
	timer->_pv->_timer = ecoreIdler;

	return std::weak_ptr<WTimer>(*timer->_pv->_selfPtr);
}

WTimerWeakPtr WTimer::addAnimator(std::function<bool (void*)> timerFunc, void* userData, Evas_Object* ownerObj) {
	WASSERT_EX(ownerObj != nullptr, "ownerObj is null.");

	WTimerWeakPtr timerPtr = WTimer::addAnimator(timerFunc, userData);
	std::shared_ptr<WTimer> timer = timerPtr.lock();
	timer->_pv->_ownerObj = ownerObj;

	evas_object_event_callback_add(ownerObj, EVAS_CALLBACK_DEL, _WTimerImpl::_ownerObjDelCb, timer.get());

	timer.reset();

	return timerPtr;
}

WTimerWeakPtr WTimer::addAnimator(std::function<bool (void*)> timerFunc, void* userData) {
	WTimer* timer = new WTimer();
	timer->_pv->_selfPtr = new std::shared_ptr<WTimer>(timer, [](WTimer * p) {});
	timer->_pv->_type = WTimer::Type::ANIMATOR;
	timer->_pv->_timerFunc = timerFunc;
	timer->_pv->_userData = userData;
	Ecore_Animator* ecoreAnimator = ecore_animator_add(_WTimerImpl::_timerCb, timer);
	if (ecoreAnimator == nullptr) {
		WERROR("failed to add ecore animator!");
		delete timer;
		return std::weak_ptr<WTimer>();
	}
	timer->_pv->_timer = ecoreAnimator;

	return std::weak_ptr<WTimer>(*timer->_pv->_selfPtr);
}

WTimerWeakPtr WTimer::addTimer(double interval, std::function<bool (void*)> timerFunc, void* userData, Evas_Object* ownerObj) {
	WASSERT_EX(ownerObj != nullptr, "ownerObj is null.");

	WTimerWeakPtr timerPtr = WTimer::addTimer(interval, timerFunc, userData);
	std::shared_ptr<WTimer> timer = timerPtr.lock();
	timer->_pv->_ownerObj = ownerObj;

	evas_object_event_callback_add(ownerObj, EVAS_CALLBACK_DEL, _WTimerImpl::_ownerObjDelCb, timer.get());

	timer.reset();

	return timerPtr;
}

WTimerWeakPtr WTimer::addTimer(double interval, std::function<bool (void*)> timerFunc, void* userData) {
	WTimer* timer = new WTimer();
	timer->_pv->_selfPtr = new std::shared_ptr<WTimer>(timer, [](WTimer * p) {});
	timer->_pv->_type = WTimer::Type::TIMER;
	timer->_pv->_timerFunc = timerFunc;
	timer->_pv->_userData = userData;
	Ecore_Timer* ecoreTimer = ecore_timer_add(interval, _WTimerImpl::_timerCb, timer);
	if (ecoreTimer == nullptr) {
		WERROR("failed to add ecore timer!");
		delete timer;
		return std::weak_ptr<WTimer>();
	}
	timer->_pv->_timer = ecoreTimer;

	return std::weak_ptr<WTimer>(*timer->_pv->_selfPtr);
}

WTimerWeakPtr WTimer::addJob(std::function<void (void*)> timerFunc, void* userData, Evas_Object* ownerObj) {
	WASSERT_EX(ownerObj != nullptr, "ownerObj is null.");

	WTimerWeakPtr timerPtr = WTimer::addJob(timerFunc, userData);
	std::shared_ptr<WTimer> timer = timerPtr.lock();
	timer->_pv->_ownerObj = ownerObj;

	evas_object_event_callback_add(ownerObj, EVAS_CALLBACK_DEL, _WTimerImpl::_ownerObjDelCb, timer.get());

	timer.reset();

	return timerPtr;
}

WTimerWeakPtr WTimer::addJob(std::function<void (void*)> timerFunc, void* userData) {
	WTimer* timer = new WTimer();
	timer->_pv->_selfPtr = new std::shared_ptr<WTimer>(timer, [](WTimer * p) {});
	timer->_pv->_type = WTimer::Type::JOB;
	timer->_pv->_jobFunc = timerFunc;
	timer->_pv->_userData = userData;
	Ecore_Job* ecoreJob = ecore_job_add(_WTimerImpl::_jobCb, timer);
	if (ecoreJob == nullptr) {
		WERROR("failed to add ecore job!");
		delete timer;
		return std::weak_ptr<WTimer>();
	}
	timer->_pv->_timer = ecoreJob;

	return std::weak_ptr<WTimer>(*timer->_pv->_selfPtr);
}

Ecore_Idler* app_assist::wuitimer_get_idler_handle(const WTimerWeakPtr& uiTimer) {
	return _WTimerImpl::getIdlerHandle(uiTimer);
}

Ecore_Animator* app_assist::wuitimer_get_animator_handle(const WTimerWeakPtr& uiTimer) {
	return _WTimerImpl::getAnimatorHandle(uiTimer);
}

Ecore_Timer* app_assist::wuitimer_get_timer_handle(const WTimerWeakPtr& uiTimer) {
	return _WTimerImpl::getTimerHandle(uiTimer);
}

Ecore_Job* app_assist::wuitimer_get_job_handle(const WTimerWeakPtr& uiTimer) {
	return _WTimerImpl::getJobHandle(uiTimer);
}

