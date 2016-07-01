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

#ifndef _SMPL_WIDGET_H_
#define _SMPL_WIDGET_H_

#include "WWidget.h"

class SmplWidget : public app_assist::WWidget {
public:
	SmplWidget();
	~SmplWidget();
private:
	virtual void onCreate() override;
	virtual void onTerminate() override;
	virtual app_assist::WWidgetInstance* onCreateWidgetInstance() override;
};

#endif // _SMPL_WIDGET_H_
