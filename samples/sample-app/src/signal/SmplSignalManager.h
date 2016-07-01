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

#ifndef _SMPL_SIGNAL_MANAGER_H_
#define _SMPL_SIGNAL_MANAGER_H_

#include "WDefine.h"
#include "SmplDebug.h"
#include "SmplAppStateSignal.h"

class SmplSignalManager {
public:
	static SmplSignalManager* getInstance();
	static void destroy();
public:
	SmplAppStateSignal& getAppStateSignal();
	app_assist::WSignal<void(int)>& getChangeSignal();

private:
	SmplSignalManager();
	~SmplSignalManager();
	WDISABLE_COPY_AND_ASSIGN(SmplSignalManager);
private:
	static SmplSignalManager* _instance;
	SmplAppStateSignal _appStateSignal;
	app_assist::WSignal<void(int)> _changeSignal;

};

#endif // _SMPL_SIGNAL_MANAGER_H_
