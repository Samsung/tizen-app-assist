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

#include "WWidget.h"

#include <widget_app.h>
#include "WDebugBase.h"
#include "WWidgetInstance.h"
#include "WWidgetImpl.h"
#include "WWidgetInstanceImpl.h"

using namespace app_assist;

widget_class_h _WWidgetImpl::_onCreate(void* data) {
	auto widget = static_cast<WWidget*>(data);

	widget_instance_lifecycle_callback_s ops = {0, };
	ops.create = _WWidgetInstanceImpl::onCreate;
	ops.destroy = _WWidgetInstanceImpl::onDestroy;
	ops.pause = _WWidgetInstanceImpl::onPause;
	ops.resume = _WWidgetInstanceImpl::onResume;
	ops.update = _WWidgetInstanceImpl::onUpdate;
	ops.resize = _WWidgetInstanceImpl::onResize;

	widget_class_h wclass = widget_app_class_create(ops, widget);

	widget->onCreate();

	return wclass;
}

void _WWidgetImpl::_onTerminate(void* data) {
	auto widget = static_cast<WWidget*>(data);
	widget->onTerminate();
}

WWidgetInstance* _WWidgetImpl::createWidgetInstance(WWidget* widget) {
	return widget->onCreateWidgetInstance();
}

WWidget::WWidget()
	: _pv(new _WWidgetImpl()) {
}

WWidget::~WWidget() {
	delete _pv;
}

int WWidget::start(int argc, char* argv[]) {
	widget_app_lifecycle_callback_s ops = {0,};
	ops.create = _WWidgetImpl::_onCreate;
	ops.terminate = _WWidgetImpl::_onTerminate;

	int ret = widget_app_main(argc, argv, &ops, this);
	if (ret != WIDGET_ERROR_NONE) {
		WDEBUG("widget_app_main fail (%d)", ret);
		return WIDGET_ERROR_OUT_OF_MEMORY;
	}

	return WIDGET_ERROR_NONE;
}
