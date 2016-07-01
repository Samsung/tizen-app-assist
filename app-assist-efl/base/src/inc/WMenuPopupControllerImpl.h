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

#ifndef _APP_ASSIST_WMENU_POPUP_CONTROLLER_IMPL_H_
#define _APP_ASSIST_WMENU_POPUP_CONTROLLER_IMPL_H_

#include <vector>
#include <functional>
#include <Evas.h>

namespace app_assist {

class _WMenuPopupControllerImpl {
public:
	_WMenuPopupControllerImpl();
	~_WMenuPopupControllerImpl();

public:
	static void destroyLastMenuPopup();

private:
	static Evas_Object* _lastMenuPopup;

	Evas_Object* _win;
	Evas_Object* _resizableBaseObj;
	Evas_Object* _popup;
	std::vector<std::function< void()>> _vSelectItemCb;

	void _init(Evas_Object* win, Evas_Object* naviframe);
	void _popupMove();
	Evas_Object* _create();

	static void _popupSelectCb(void* data, Evas_Object* obj, void* event_info);
	static void _popupResizeCb(void* data, Evas* e, Evas_Object* obj, void* event_info);
	static void _popupWinRotateCb(void* data, Evas_Object* obj, void* event_info);
	static void _popupDismissedCb(void* data, Evas_Object* obj, void* event_info);

	friend class WMenuPopupController;
};

} // namespace

#endif /* _APP_ASSIST_WMENU_POPUP_CONTROLLER_IMPL_H_ */
