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

#include "SmplWidgetInstance.h"

#include "WDebugBase.h"
#include "WWidgetWindowController.h"
#include "SmplWidgetMainViewController.h"

using namespace app_assist;

SmplWidgetInstance::SmplWidgetInstance() {
	WHIT();
}

SmplWidgetInstance::~SmplWidgetInstance() {
	WHIT();
}

bool SmplWidgetInstance::onCreate(widget_context_h context, bundle* content, int width, int height) {
	WHIT();

	attachWindowController(context, new WWidgetWindowController());
	getWindowController()->attachBaseViewController(new SmplWidgetMainViewController());

	return true;
}

bool SmplWidgetInstance::onDestroy(widget_context_h context, widget_app_destroy_type_e reason, bundle* content) {
	WHIT();

	return true;
}

bool SmplWidgetInstance::onPause(widget_context_h context) {
	WHIT();

	return true;
}

bool SmplWidgetInstance::onResume(widget_context_h context) {
	WHIT();

	return true;
}

bool SmplWidgetInstance::onUpdate(widget_context_h context, bundle* content, bool isForced) {
	WHIT();

	return true;
}

bool SmplWidgetInstance::onResize(widget_context_h context, int width, int height) {
	WHIT();

	return true;
}
