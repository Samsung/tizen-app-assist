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

#include "WWidgetInstance.h"

#include "WDebugBase.h"
#include "WWidget.h"
#include "WWidgetWindowController.h"
#include "WWidgetImpl.h"
#include "WWidgetInstanceImpl.h"

using namespace app_assist;

int _WWidgetInstanceImpl::onCreate(widget_context_h context, bundle* content, int w, int h, void* data) {
	auto widget = static_cast<WWidget*>(data);
	WWidgetInstance* widgetInstance = _WWidgetImpl::createWidgetInstance(widget);
	if (widgetInstance == nullptr) {
		WDEBUG("WidgetInstance creation fail");
		return WIDGET_ERROR_FAULT;
	}

	widget_app_context_set_tag(context, (void*)widgetInstance);
	widgetInstance->_pv->_width = w;
	widgetInstance->_pv->_height = h;
	widgetInstance->_pv->_widget = widget;

	bool ret =  widgetInstance->onCreate(context, content, w, h);
	if (ret) {
		return WIDGET_ERROR_NONE;
	}
	else {
		return WIDGET_ERROR_FAULT;
	}
}

int _WWidgetInstanceImpl::onDestroy(widget_context_h context, widget_app_destroy_type_e reason, bundle* content, void* data) {
	auto widgetInstance = _getWidgetInstance(context);

	if (widgetInstance->_pv->_window)
		widgetInstance->_pv->_window->destroy();

	bool ret = widgetInstance->onDestroy(context, reason, content);
	delete widgetInstance;

	if (ret)
		return WIDGET_ERROR_NONE;
	else
		return WIDGET_ERROR_FAULT;
}

int _WWidgetInstanceImpl::onPause(widget_context_h context, void* data) {
	auto widgetInstance = _getWidgetInstance(context);

	bool ret = widgetInstance->onPause(context);
	if (ret)
		return WIDGET_ERROR_NONE;
	else
		return WIDGET_ERROR_FAULT;
}

int _WWidgetInstanceImpl::onResume(widget_context_h context, void* data) {
	auto widgetInstance = _getWidgetInstance(context);

	bool ret = widgetInstance->onResume(context);
	if (ret)
		return WIDGET_ERROR_NONE;
	else
		return WIDGET_ERROR_FAULT;
}

int _WWidgetInstanceImpl::onUpdate(widget_context_h context, bundle* content, int force, void* data) {
	auto widgetInstance = _getWidgetInstance(context);

	bool isForced = true;
	if (force == 0) isForced = false;
	bool ret = widgetInstance->onUpdate(context, content, isForced);
	if (ret)
		return WIDGET_ERROR_NONE;
	else
		return WIDGET_ERROR_FAULT;
}

int _WWidgetInstanceImpl::onResize(widget_context_h context, int w, int h, void* data) {
	auto widgetInstance = _getWidgetInstance(context);

	bool ret = widgetInstance->onResize(context, w, h);
	if (ret) {
		widgetInstance->_pv->_width = w;
		widgetInstance->_pv->_height = h;
		return WIDGET_ERROR_NONE;
	}
	else {
		return WIDGET_ERROR_FAULT;
	}
}

WWidgetInstance* _WWidgetInstanceImpl::_getWidgetInstance(widget_context_h context) {
	WWidgetInstance* widgetInstance = nullptr;
	widget_app_context_get_tag(context, (void**)(&widgetInstance));

	return widgetInstance;
}

// WWidgetInstance

WWidgetInstance::WWidgetInstance()
	: _pv(new _WWidgetInstanceImpl()) {
}

WWidgetInstance::~WWidgetInstance() {
	delete _pv;
}

WWidgetWindowController* WWidgetInstance::getWindowController() const {
	return _pv->_window;
}

WWidget* WWidgetInstance::getWidget() const {
	return _pv->_widget;
}

int WWidgetInstance::getWidth() const {
	return _pv->_width;
}

int WWidgetInstance::getHeight() const {
	return _pv->_height;
}

bool WWidgetInstance::attachWindowController(widget_context_h context, WWidgetWindowController* win) {
	_pv->_window = win;
	return win->create(context, getWidth(), getHeight());
}
