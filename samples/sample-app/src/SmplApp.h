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

#ifndef _SMPL_APP_H_
#define _SMPL_APP_H_

#include "WApp.h"
#include "WAppEventHandler.h"

class SmplApp : public app_assist::WApp {
public:
	SmplApp();
	virtual ~SmplApp();

private:
	virtual bool onCreate() override;
	virtual void onTerminate() override;
	virtual void onAppControl(app_control_h request, bool firstLaunch) override;
	virtual void onPause() override;
	virtual void onResume() override;

private:
	app_assist::WAppEventHandler _regionFormatChangedHandler;
	app_assist::WAppEventHandler _languageChangedHandler;
};

#endif // _SMPL_APP_H_
