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

#ifndef _APP_ASSIST_WVIEW_CONTROLLER_IMPL_H_
#define _APP_ASSIST_WVIEW_CONTROLLER_IMPL_H_

#include <memory>
#include <Evas.h>
#include "IWWindowController.h"

namespace app_assist {

class WViewController;

class _WViewControllerImpl {
public:
	_WViewControllerImpl();
	~_WViewControllerImpl();

private:
	Evas_Object* _obj;
	char* _name;
	std::weak_ptr<WViewController>* _popupMonitor;
	std::function<Evas_Object* (Evas_Object*, void*)>* _createViewDelegate;
	IWWindowController* _windowController;

	mutable std::shared_ptr<WViewController>* _selfPtr;

	static void _objDelCb(void* data, Evas* evas, Evas_Object* obj, void* event_info);
	static Evas_Object* getWindowBaseLayoutEvasObject(WViewController* view);
	static Evas_Object* getWindowEvasObject(WViewController* view);

	friend class WViewController;
	friend Evas_Object* wviewcontroller_get_window_baselayout_evasobj(WViewController* view);
	friend Evas_Object* wviewcontroller_get_window_evasobj(WViewController* view);
};

} // namespace

#endif /* _APP_ASSIST_WVIEW_CONTROLLER_IMPL_H_ */
