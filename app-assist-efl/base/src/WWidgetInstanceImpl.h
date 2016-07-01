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

#ifndef _APP_ASSIST_WWIDGET_INSTANCE_IMPL_H_
#define _APP_ASSIST_WWIDGET_INSTANCE_IMPL_H_

#include <widget_app.h>

namespace app_assist {

class WWidgetWindowController;
class WWidget;

class _WWidgetInstanceImpl {
public:
	static int onCreate(widget_context_h context, bundle* content, int w, int h, void* data);
	static int onDestroy(widget_context_h context, widget_app_destroy_type_e reason, bundle* content, void* data);
	static int onPause(widget_context_h context, void* data);
	static int onResume(widget_context_h context, void* data);
	static int onUpdate(widget_context_h context, bundle* content, int force, void* data);
	static int onResize(widget_context_h context, int w, int h, void* data);

private:
	_WWidgetInstanceImpl() : _window(nullptr), _widget(nullptr), _width(0), _height(0) {};
	~_WWidgetInstanceImpl() {};

private:
	static WWidgetInstance* _getWidgetInstance(widget_context_h context);

	WWidgetWindowController* _window;
	WWidget* _widget;
	int _width;
	int _height;

	friend class WWidgetInstance;
};

} // namespace

#endif // _APP_ASSIST_WWIDGET_INSTANCE_IMPL_H_
