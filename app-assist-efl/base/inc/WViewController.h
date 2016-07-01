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

#ifndef _APP_ASSIST_WVIEW_CONTROLLER_H_
#define _APP_ASSIST_WVIEW_CONTROLLER_H_

#include <functional>
#include <memory>
#include <Evas.h>
#include "WDefine.h"
#include "IWWindowController.h"

namespace app_assist {

class WViewController;

typedef std::weak_ptr<WViewController> WViewControllerWeakPtr;

/**
* @class	WViewController
* @brief	This class is the UI control class linked with one evas object.
*
*
* The %WViewController is the abstract class managing an evas object
*/
class WAPP_ASSIST_EXPORT WViewController {
public:
	/**
	 * This is the default constructor for this class.
	 *
	 */
	WViewController();

	/**
	 * Gets class name
	 *
	 * @return The class name
	 */
	virtual const char* getClassName() const;
public:

	/**
	 * Initializes and creates the control. onCreate will be called subsequently
	 *
	 * @param[in]	parent	parent evas object
	 * @param[in]	param	user data
	 *
	 * @return result
	 * @see onCreate()
	 */
	virtual bool create(Evas_Object* parent, void* param);

	/**
	 * Destroys the control. The linked evas object will be delete together.
	 * "onDestroy" will be called subsequently.
	 *
	 * @see onDestroy()
	 */
	virtual void destroy();

	/**
	 * Gets evas object of this class.
	 *
	 * @return The pointer to evas object
	 */
	Evas_Object* getEvasObject() const;

	/**
	 * Sets the name of the instance.
	 *
	 * @param[in]	name	blash
	 */
	void setName(const char* name);

	/**
	 * Gets the name of the instance.
	 *
	 * @return The control name
	 */
	const char* getName() const;

	/**
	 * Returns weak pointer of this instance to refer it safe.
	 *
	 * @return weak pointer of this class instance
	 */
	WViewControllerWeakPtr getWeakPtr() const;

	/**
	 * Attaches popup to the control.It means that it binds them to have same life-time.
	 * Internally create method of the pop-up will be called, if it is not yet created.
	 * If previous pop-up exists, it will be destroyed.
	 *
	 * @param[in]	popup pop-up instance pointer to be attached
	 *
	 * @see destroyPopup()
	 */
	void attachPopup(WViewController* popup);

	/**
	 * Destroys pop-up. Use this method when you want to destroy pop-up explicitly.
	 * Pop-up will be deleted when the control is deleted, although this method is not called.
	 *
	 * @see attachPopup()
	 */
	void destroyPopup();
public:
	/**
	 * Sets a delegate function. It delegates onCreate handling to delegateFunc.
	 * This is simpler alternative to overriding the virtual method onCreate(), allowing direct use of WContentViewController without sub-classing.
	 * If delegation is used, onCreateView() will not be called.
	 *
	 * @param[in]	delegateFunc	The pointer to delegate function
	 * @see onCreateView()
	 */
	void setOnCreateView(std::function<Evas_Object* (Evas_Object* parent, void* param)> delegateFunc);

protected:
	/**
	 * Override this method to initialize this WViewController class.
	 * It is called when create method is called.
	 *
	 * @param[in]	param	user parameter
	 *
	 * @return true if it is succeeded.
	 * @see create()
	 * @see onCreated()
	 */
	virtual bool onCreate(void* param) {return true;};
	/**
	 * Override this method to create evas object of this WViewController class.
	 *
	 * @param[in]	parent	the parent evas object
	 * @param[in]	param	user parameter
	 *
	 * @return The pointer to evas object created, which should be the most parent evas object among them created in this method.
	 * @see create()
	 * @see onCreated()
	 */
	virtual Evas_Object* onCreateView(Evas_Object* parent, void* param) {return nullptr;}

	/**
	 * Override this method to do something after evas object is created.
	 *
	 * @see create()
	 * @see onCreate()
	 */
	virtual void onCreated() {};

	/**
	 * Override this method to do something on this instance deletion.
	 *
	 * @see destroy()
	 */
	virtual void onDestroy() {};

	/**
	 * Set the part content with the WViewController object.
	 * The part is specified with a string part name.
	 * If the WViewController object is not yet created, it will be created internally at this method.
	 * If there is a previous WViewController object set as a part content, it will be destroyed.
	 *
	 * @param[in]	parentName	part name string. For some objects, it can be null pointer
	 * @param[in]	vc			WViewController object to be set as the part
	 *
	 * @return true, if it succeeded. If there is no part or this container view controller is not yet created, it will return false.
	 * @see getPartContent()
	 * @see unsetPartContent()
	 */
	bool setPartContent(const char* partName, WViewController* vc);

	/**
	 * Get the part content.
	 * It returns the WViewController object which was set as a part content.
	 * But it still the part content of this parent view controller object, unlike unsetPartContent method.
	 * So you should not destroy this returned pointer.
	 *
	 * @param[in]	parentName	part name string. For some objects, it can be null pointer
	 *
	 * @return WViewController object which was set as a part content. If it does not exist, it will return null pointer.
	 * @see setPartContent()
	 * @see unsetPartContent()
	 */
	WViewController* getPartContent(const char* partName) const;

	/**
	 * Unset the part content.
	 * It returns the WViewController object which was set as a part content.
	 * After getting this returned pointer, you have to destroy this explicitly.
	 *
	 * @param[in]	parentName	part name string. For some objects, it can be null pointer
	 *
	 * @return WViewController object which was set as a part content. If it does not exist, it will return null pointer.
	 * @see setPartContent()
	 */
	WViewController* unsetPartContent(const char* partName);

	/**
	 * Gets the pointer to the IWWindowController type class instance.
	 * Usually it will be normal window instance, but if the view created on UG module, it will be UG window instance.
	 * Through this returned pointer, you can get window related properties.
	 *
	 * @return The pointer to the IWWindowController type instance.
	 */
	IWWindowController* getWindowController() const;

protected:
	/**
	 * This is the destructor for this class.
	 *
	 */
	virtual ~WViewController();

private:
	WDECLARE_PRIVATE_IMPL(WViewController);
	WDISABLE_COPY_AND_ASSIGN(WViewController);
};

/**
 * Gets the class instance from evas object
 *
 * @param[in]	obj	evas object
 *
 * @return The pointer to WViewController class instance. It will be nullptr, if no instance is related.
 */
WAPP_ASSIST_EXPORT WViewController* wviewcontroller_get_instance_from_evasobj(Evas_Object* obj);

/**
 * Get the evas object of window baseLayout related to view.
 *
 * @param[in]	view	WViewController instance
 *
 * @return nullptr, if this failed to get window baseLayout.
 */
WAPP_ASSIST_EXPORT Evas_Object* wviewcontroller_get_window_baselayout_evasobj(WViewController* view);

/**
 * Get the evas object of window related to view.
 *
 * @param[in]	view	WViewController instance
 *
 * @return nullptr, if this failed to get window.
 */
WAPP_ASSIST_EXPORT Evas_Object* wviewcontroller_get_window_evasobj(WViewController* view);

} // namespace


#endif /* _APP_ASSIST_WVIEW_CONTROLLER_H_ */
