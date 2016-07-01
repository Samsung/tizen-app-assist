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

#ifndef _SMPL_APP_STATE_SIGNAL_H_
#define _SMPL_APP_STATE_SIGNAL_H_

#include "WSignal.h"

class SmplAppStateSignal : public app_assist::WSignal<bool(const char*)> {
public:
	SmplAppStateSignal();
	virtual ~SmplAppStateSignal();
private:
	virtual void onEmit(const char* msg) override;
};

#endif // _SMPL_APP_STATE_SIGNAL_H_
