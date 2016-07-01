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

#ifndef _SMPL_WIDGET_INSTANCE_H_
#define _SMPL_WIDGET_INSTANCE_H_

#include "WWidgetInstance.h"

class SmplWidgetInstance : public app_assist::WWidgetInstance {
public:
	SmplWidgetInstance();
	~SmplWidgetInstance();
private:
	virtual bool onCreate(widget_context_h context, bundle* content, int width, int height) override;
	virtual bool onDestroy(widget_context_h context, widget_app_destroy_type_e reason, bundle* content) override;
	virtual bool onPause(widget_context_h context) override;
	virtual bool onResume(widget_context_h context) override;
	virtual bool onUpdate(widget_context_h context, bundle* content, bool isForced) override;
	virtual bool onResize(widget_context_h context, int width, int height) override;
};

#endif // _SMPL_WIDGET_INSTANCE_H_
