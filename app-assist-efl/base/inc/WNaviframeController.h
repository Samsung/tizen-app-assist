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

#ifndef _APP_ASSIST_WNAVI_FRAME_CONTROLLER_H_
#define _APP_ASSIST_WNAVI_FRAME_CONTROLLER_H_

#include <functional>
#include <Evas.h>
#include <Elementary.h>
#include "WViewController.h"

namespace app_assist {

// forward declaration
class WContentViewController;

/**
* @class	WNaviframeController
* @brief	This class is the controller class of naviframe elementary.
*
*
* The %WNaviframeController class is to control naviframe elementary.
*/
class WAPP_ASSIST_EXPORT WNaviframeController : public WViewController {
public:
	/**
	 * This is the default constructor for this class.
	 *
	 */
	WNaviframeController();

	/**
	 * Gets the class name.
	 *
	 * @return The class name
	 */
	virtual const char* getClassName() const; //override;

public:
	/**
	 * Initializes and creates the evas object.
	 * "onCreate" will be called subsequently.
	 *
	 * @param[in]	parent	the parent evas object
	 * @param[in]	param	user data
	 *
	 * @return	true if the creation is succeeded, else false
	 * @see onCreate()
	 */
	bool create(Evas_Object* parent, void* param);	//final;

	/**
	 * Destroy the instance. "onDestroy" will be called subsequently.
	 * The naviframe elementary will be deleted together.
	 *
	 * @see onDestroy()
	 */
	void destroy(); //final;

	/**
	 * Pushes the view controller instance into naviframe. "onPushToNaviframe" will be called subsequently.
	 * If the view controller is not yet called "create", it will be called internally.
	 *
	 * @param[in]	vc	the pointer to the view controller instance to be pushed
	 * @param[in]	vcName	the view controller instance name to be set when it is pushed.
	 * @param[in]	vcParam	the user data to be passed as parameter of "create" method of the view controller.
	 *
	 * @return	true if succeed, else false
	 * @see onPushToNaviframe
	 */
	bool push(WContentViewController* vc, const char* vcName, void* vcCreationParam);
	bool push(WContentViewController* vc, const char* vcName);
	bool push(WContentViewController* vc);

	/**
	 * Destroy all views stacked on naviframe.
	 *
	 */
	void destroyAllViewControllers();

	/**
	 * Check if the view of view controller is on top of naviframe.
	 *
	 * @param[in]	vc	the pointer to the view controller instance to be checked
	 *
	 * @return	true if it is top, else false
	 */
	bool isTopContentViewController(WContentViewController* vc) const;
	/**
	 * Get the view controller on top of naviframe.
	 *
	 * @return	WContentViewController pointer related with naviframe item
	 */
	WContentViewController* getTopContentViewController() const;

public:
	/**
	 * Sets a delegate function. It delegates onLastItemPop handling to delegateFunc.
	 * This is simpler alternative to overriding the virtual method onLastItemPop(), allowing direct use of WNaviframeController without sub-classing.
	 * If delegation is used, onLastItemPop() will not be called.
	 *
	 * @param[in]	delegateFunc	The pointer to delegate function
	 * @see onLastItemPop()
	 */
	void setOnLastItemPop(std::function<void(bool* popOut)> delegateFunc);

protected:
	/**
	 * Override this method to create your own naviframe elementary.
	 *
	 * @param[in]	parent	The pointer to the parent evas object
	 * @param[in]	param	user data
	 * @return The pointer to naviframe evas object created
	 */
	virtual Evas_Object* onCreateView(Evas_Object* parent, void* param);	//override;

	/**
	 * Override this method to do something on this instance deletion.
	 *
	 * @see destroy()
	 */
	virtual void onDestroy(); //override;

	/**
	 * Override this method to do something before pushing the view to the naviframe.
	 * In the overridden method, WNaviframeController::onPush should be called to do the base operation.
	 *
	 * @param[in]	vc	The pointer to the view controller instance to be pushed
	 * @param[in]	viewParam	user data to be passed from "push". It is passed to the "create" method of the WContentViewController class
	 *
	 * @return result
	 * @see push
	 */
	virtual bool onPush(WContentViewController* vc, void* viewParam);

	/**
	 * Override this method to do something before popping the view from the naviframe.
	 * If you override this method, call WNaviframeController::onItemPop at the bottom of your method to do the base operations.
	 *
	 * @param[in]	vc	The pointer to the view controller instance to be popped
	 * @param[in]	item	The pointer to the navi item
	 *
	 * @return	true the view will be popped out, false the view will not be popped.
	 * @see onLastItemPop()
	 */
	virtual bool onItemPop(WContentViewController* vc, Elm_Object_Item* item);

	/**
	 * Override this method to change how the application should work when last view is popped from naviframe.
	 * If you don't override it, the application will go background by default.
	 * Even though onLastItemPop is called, "onItemPop" will be called before.
	 *
	 * @param[out]	popOut	Set true, if you will pop out the last view and end the application. If not, set false before return
	 * @see onItemPop()
	 */
	virtual void onLastItemPop(bool* popOut);

protected:
	/**
	 * This is the destructor for this class.
	 *
	 */
	virtual ~WNaviframeController();

private:
	WDECLARE_PRIVATE_IMPL(WNaviframeController);
	WDISABLE_COPY_AND_ASSIGN(WNaviframeController);
};

} // namespace

#endif /* _APP_ASSIST_WNAVI_FRAME_CONTROLLER_H_ */
