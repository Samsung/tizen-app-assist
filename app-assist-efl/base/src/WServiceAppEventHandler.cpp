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

#include "WServiceAppEventHandler.h"
#include <service_app.h>
#include "WDebugInternal.h"
#include "WDefineInternal.h"

using namespace app_assist;

namespace app_assist {

class _WServiceAppEventHandlerImpl {
public:
	_WServiceAppEventHandlerImpl();
	~_WServiceAppEventHandlerImpl();

public:
	app_event_handler_h _handle;
	void* _userData;
	std::function<void (app_event_info_h, void*)> _eventHandlerFunc;

	static void _eventCb(app_event_info_h eventInfo, void* userData);
	void _removeEventHandler();
};
}

_WServiceAppEventHandlerImpl::_WServiceAppEventHandlerImpl()
	: _handle(nullptr), _userData(nullptr) {
}

_WServiceAppEventHandlerImpl::~_WServiceAppEventHandlerImpl() {
	_removeEventHandler();
}

void _WServiceAppEventHandlerImpl::_eventCb(app_event_info_h eventInfo, void* userData) {
	auto p = (_WServiceAppEventHandlerImpl*)userData;
	p->_eventHandlerFunc(eventInfo, p->_userData);
}

void _WServiceAppEventHandlerImpl::_removeEventHandler() {
	if (_handle) {
		service_app_remove_event_handler(_handle);
		_handle = nullptr;
		_userData = nullptr;

	}
}

WServiceAppEventHandler::WServiceAppEventHandler() {
	_pv = new _WServiceAppEventHandlerImpl();
}

WServiceAppEventHandler::~WServiceAppEventHandler() {
	delete _pv;
}


int WServiceAppEventHandler::addEventHandler(app_event_type_e eventType, std::function<void (app_event_info_h, void*)> handlerFunc, void* userData) {
	removeEventHandler();

	_pv->_eventHandlerFunc = handlerFunc;
	_pv->_userData = userData;

	return service_app_add_event_handler(&_pv->_handle, eventType, _pv->_eventCb, _pv);
}

void WServiceAppEventHandler::removeEventHandler() {
	_pv->_removeEventHandler();
}

app_event_handler_h WServiceAppEventHandler::getHandle() {
	return _pv->_handle;
}


