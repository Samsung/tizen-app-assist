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

#ifndef _APP_ASSIST_WMENU_POPUP_CONTROLLER_H_
#define _APP_ASSIST_WMENU_POPUP_CONTROLLER_H_

#include <Evas.h>
#include <Elementary.h>
#include <functional>
#include "WViewController.h"

namespace app_assist {
/**
* @class	WMenuPopupController
* @brief	This class is the menu pop-up class to make easy to manage menus.
*
*
* The %WMenuPopupController is the menu pop-up class.
*/
class WAPP_ASSIST_EXPORT WMenuPopupController: public WViewController {
public:
	/**
	 * This is the constructor for this class.
	 *
	 */
	WMenuPopupController();

public:
	/**
	 * Creates a menu pop-up.
	 *
	 * @param[in]	win					parent window evas object
	 * @param[in]	resizableObj		resizable base evas object. According to this size, menu popup will be resized.
	 *
	 * @return true if it is succeeded
	 */
	bool createMenu(Evas_Object* win, Evas_Object* resizableObj);

	/**
	 * Appends menu items.
	 *
	 * @param[in]	label			label text of menu item
	 * @param[in]	selectItemCb	callback function object when the item is selected
	 *
	 * @return appended item. nullptr,if failed.
	 */
	Elm_Object_Item* appendItem(const char* label, std::function< void()> selectItemCb);

	/**
	 * Resize and show menu pop-up.
	 *
	 * @return true if it is succeeded
	 */
	bool showMenu();

protected:
	/**
	 * This is the destructor for this class.
	 *
	 */
	virtual ~WMenuPopupController();

private:
	virtual Evas_Object* onCreateView(Evas_Object* parent, void* param); //override;
	virtual void onDestroy(); //override;

private:
	WDECLARE_PRIVATE_IMPL(WMenuPopupController);
	WDISABLE_COPY_AND_ASSIGN(WMenuPopupController);
};

} // namespace

#endif /* _APP_ASSIST_WMENU_POPUP_CONTROLLER_H_ */
