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

#ifndef _SMPL_WATCH_APP_H_
#define _SMPL_WATCH_APP_H_

#include <WWatchApp.h>

class SmplWatchFaceViewController;

class SmplWatchApp : public app_assist::WWatchApp {
public:
	SmplWatchApp();
	virtual ~SmplWatchApp();

private:
	bool onCreate();	// override
	void onTerminate();	// override
	void onPause();	// override
	void onResume();	// override
	void onAppControl(app_control_h request, bool firstLaunch);	// override
	void onTimeTick(watch_time_h watchTime);	// override
	void onAmbientModeTimeTick(watch_time_h watchTime);	// override
	void onAmbientModeChanged(bool ambientMode);	// override

	SmplWatchFaceViewController* _watchFaceView;
};

#endif /* _SMPL_WATCH_APP_H_ */
