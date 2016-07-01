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

#include "WTimerImpl.h"
#include <Eina.h>
#include <Ecore.h>
#include "WDefine.h"
#include "WDebugInternal.h"
#include "WTimer.h"

using namespace app_assist;

_WTimerImpl::_WTimerImpl()
	: _timer(nullptr),
	_type(WTimer::Type::IDLER),
	_userData(nullptr),
	_selfPtr(nullptr),
	_ownerObj(nullptr) {
}

_WTimerImpl::~_WTimerImpl() {
	_deleteTimer();
	delete _selfPtr;
}

void _WTimerImpl::_deleteTimer() {
	if (_timer) {
		switch (_type) {
		case WTimer::Type::IDLER:
			ecore_idler_del((Ecore_Idler*)_timer);
			break;
		case WTimer::Type::ANIMATOR:
			ecore_animator_del((Ecore_Animator*)_timer);
			break;
		case WTimer::Type::TIMER:
			ecore_timer_del((Ecore_Timer*)_timer);
			break;
		case WTimer::Type::JOB:
			ecore_job_del((Ecore_Job*)_timer);
			break;
		default:
			break;
		}
		_timer = nullptr;
	}
}

void _WTimerImpl::_ownerObjDelCb(void* data, Evas* evas, Evas_Object* obj, void* event_info) {
	WHIT();

	auto timer = (WTimer*)data;
	timer->_pv->_ownerObj = nullptr;
	delete timer;
}

Eina_Bool _WTimerImpl::_timerCb(void* data) {
	auto timer = (WTimer*)data;
	std::weak_ptr<WTimer> timerWeakPtr(*(timer->_pv->_selfPtr));

	Eina_Bool ret = EINA_FALSE;
	if (timer->_pv->_timerFunc(timer->_pv->_userData))
		ret = EINA_TRUE;

	if (ret == EINA_FALSE && !timerWeakPtr.expired()) {
		timer->_pv->_timer = nullptr;
		delete timer;
	}

	return ret;
}

void _WTimerImpl::_jobCb(void* data) {
	auto timer = (WTimer*)data;
	std::weak_ptr<WTimer> timerWeakPtr(*(timer->_pv->_selfPtr));

	timer->_pv->_jobFunc(timer->_pv->_userData);
	if (!timerWeakPtr.expired()) {
		timer->_pv->_timer = nullptr;
		delete timer;
	}
}

Ecore_Idler* _WTimerImpl::getIdlerHandle(const WTimerWeakPtr& uiTimer) {
	if (auto p = uiTimer.lock()) {
		if (p->_pv->_type != WTimer::Type::IDLER) {
			WERROR("This is not Idler type timer!");
			return nullptr;
		}
		return (Ecore_Idler*)p->_pv->_timer;
	}
	return nullptr;
}

Ecore_Animator* _WTimerImpl::getAnimatorHandle(const WTimerWeakPtr& uiTimer) {
	if (auto p = uiTimer.lock()) {
		if (p->_pv->_type != WTimer::Type::ANIMATOR) {
			WERROR("This is not Animator type timer!");
			return nullptr;
		}
		return (Ecore_Animator*)p->_pv->_timer;
	}
	return nullptr;
}

Ecore_Timer* _WTimerImpl::getTimerHandle(const WTimerWeakPtr& uiTimer) {
	if (auto p = uiTimer.lock()) {
		if (p->_pv->_type != WTimer::Type::TIMER) {
			WERROR("This is not Timer type timer!");
			return nullptr;
		}
		return (Ecore_Timer*)p->_pv->_timer;
	}
	return nullptr;
}

Ecore_Job* _WTimerImpl::getJobHandle(const WTimerWeakPtr& uiTimer) {
	if (auto p = uiTimer.lock()) {
		if (p->_pv->_type != WTimer::Type::JOB) {
			WERROR("This is not Job type timer!");
			return nullptr;
		}
		return (Ecore_Job*)p->_pv->_timer;
	}
	return nullptr;
}
