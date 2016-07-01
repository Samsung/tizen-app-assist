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

#ifndef _SMPL_WATCH_FACE_VIEW_CONTROLLER_H_
#define _SMPL_WATCH_FACE_VIEW_CONTROLLER_H_

#include <WViewController.h>

class SmplWatchFaceViewController : public app_assist::WViewController {
public:
	SmplWatchFaceViewController(int width, int height);
	virtual ~SmplWatchFaceViewController();

	void update(int hour, int min, int sec);

private:
	Evas_Object* onCreateView(Evas_Object* parent, void* param);

	Evas_Object* _label;
	int _width;
	int _height;
};

#endif /* _SMPL_WATCHFACE_VIEW_CONTROLLER_H_ */
