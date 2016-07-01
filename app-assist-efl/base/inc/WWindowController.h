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

#ifndef _APP_ASSIST_WWINDOW_CONTROLLER_H_
#define _APP_ASSIST_WWINDOW_CONTROLLER_H_

#include <functional>
#include <Evas.h>
#include <Elementary.h>
#include "WDefine.h"
#include "IWWindowController.h"

namespace app_assist {

class WViewController;

/**
* @class	WWindowController
* @brief	This class represents window and base layout evas object.
*
* The %WWindowController manages window evas object and common base evas objects.
*/
class WAPP_ASSIST_EXPORT WWindowController : public IWWindowController {
public:
	WWindowController() = delete;
	/**
	 * Constructor
	 * Initializes this instance of WWindowController with the specified value.
	 *
	 * @param[in]	name	The window name
	 * @param[in]	winType The type of the window
	 */
	WWindowController(const char* name, Elm_Win_Type winType);

	/**
	 * Gets the class name of this class
	 *
	 * @return The class name
	 */
	virtual const char* getClassName() const; //override;

public:
	/**
	 * Creates the windows
	 * "onCreateWin" and "onCreateBaseLayout" will be called subsequently.
	 *
	 * @return true if the windows created sucessfully, else false
	 * @see onCreateWin()
	 * @see onCreateBaseLayout()
	 */
	bool create();

	/**
	 * Creates the windows but not be shown
	 * "onCreateWin" and "onCreateBaseLayout" will be called subsequently.
	 *
	 * @return true if the windows created sucessfully, else false
	 * @see create()
	 * @see onCreateWin()
	 * @see onCreateBaseLayout()
	 */
	bool precreate();

	/**
	 * Destroys the window class instance and delete window evas object.
	 * "onDestroy" will be called subsequently.
	 *
	 * @see onDestroy()
	 */
	void destroy();

	/**
	 * Gets the evas object of the window
	 *
	 * @return The pointer to the evas object of the window
	 * @see onCreateWin()
	 */
	virtual Evas_Object* getEvasObject() const; //override;

	/**
	 * Gets the base layout evas object
	 *
	 * @return The pointer to the evas object of the layout
	 * @see onCreateBaseLayout()
	 */
	virtual Evas_Object* getBaseLayoutEvasObject() const; //override;

	/**
	 * Gets the conformant evas object.
	 *
	 * @return The pointer to the evas object of the conformant
	 */
	virtual Evas_Object* getConformantEvasObject() const; //override;

	/**
	 * Gets the base UI object attached.
	 *
	 * @return The pointer to the base ui object
	 * @see attachBaseViewController()
	 */
	virtual WViewController* getBaseViewController() const; //override;

	/**
	 * Attaches base UI object, which can be WViewController instance.
	 *
	 * @param[in]	baseObj	WViewController instance to be attached
	 * @param[in]	creationParam	parameter to be passed as parameter of create method of the WViewController class
	 *
	 * @return true if the base ui object attached sucessfully, else false
	 * @see getBaseViewController()
	 */
	bool attachBaseViewController(WViewController* vc, void* creationParam);
	bool attachBaseViewController(WViewController* vc);

	/**
	 * Detaches the base UI object.
	 *
	 * @return the base UI object, which should be deleted explicitly later.
	 */
	WViewController* detachBaseViewController();

	/**
	 * Destroys the base UI object.
	 *
	 */
	void destroyBaseViewController();

	/**
	 * Gets the window name
	 *
	 * @return The name of the window
	 */
	const char* getName() const;

	/**
	 * Gets the type of the window
	 *
	 * @return The type of the window
	 */
	Elm_Win_Type getWinType() const;

	/**
	 * Sets a delegate function. It delegates onCreateWin handling to delegateFunc.
	 * This is simpler alternative to overriding the virtual method onCreateWin(), allowing direct use of WWindowController without sub-classing.
	 * If delegation is used, onCreateWin() will not be called.
	 *
	 * @param[in]   delegateFunc     The pointer to delegate function
	 * @see onCreateWin()
	 */
	void setOnCreateWin(std::function<Evas_Object* ()> delegateFunc);

	/**
	 * Sets a delegate function. It delegates onCreateBaseLayout handling to delegateFunc.
	 * This is simpler alternative to overriding the virtual method onCreateBaseLayout(), allowing direct use of WWindowController without sub-classing.
	 * If delegation is used, onCreateBaseLayout() will not be called.
	 *
	 * @param[in]   delegateFunc     The pointer to delegate function
	 * @see onCreateBaseLayout()
	 */
	void setOnCreateBaseLayout(std::function<Evas_Object* (Evas_Object* win, Evas_Object** outConformant)> delegateFunc);

	/**
	 * Sets a back key handler function. It is called when 'back' key is pressed.
	 * Normally it is used for single view controller application without naviframe.
	 *
	 * @param[in]   backHandlerFunc     The pointer to handler function
	 */
	void setOnBack(std::function<void()> backHandlerFunc);

protected:
	/**
	 * Called when create method is called to create window
	 * Override this method to create your own window evas object.
	 *
	 * @return The pointer to the evas object of the window created.
	 * @see create()
	 */
	virtual Evas_Object* onCreateWin();

	/**
	 * Called when create method is called to create base layout.
	 * Override this method to create your own base layout and conformant evas objects.
	 *
	 * @param[in]	win	A pointer to evas object of the window
	 * @param[out]	outConformant The pointer to the evas object of the conformant created.
	 *
	 * @return The pointer to the evas object of base layout created.
	 */
	virtual Evas_Object* onCreateBaseLayout(Evas_Object* win, Evas_Object** outConformant);

	/**
	 * Called when attachBaseViewController is called.
	 * Default behavior is to set the given base UI object to elm.swallow.content part of the default base layout.
	 * Override this method to add custom behaviors just before attachment, or if the default base layout is not used.
	 *
	 * @param[in]	vc	WViewController instance which is about to be attached
	 *
	 * @see attachBaseViewController()
	 */
	virtual void onAttachBaseViewController(WViewController* vc);

	/**
	 * Called when detachBaseViewController is called.
	 * Default behavior is to do nothing.
	 * Override this method to add custom behaviors just before detachment.
	 *
	 * @param[in]	vc	WViewController instance which is about to be detached
	 *
	 * @see detachBaseViewController()
	 */
	virtual void onDetachBaseViewController(WViewController* vc);

	/**
	 * Called when the window is destroyed
	 * Override this method to do something on destruction.
	 *
	 */
	virtual void onDestroy();

protected:
	/**
	 * This is the destructor for this class.
	 *
	 */
	virtual ~WWindowController();

private:
	WDECLARE_PRIVATE_IMPL(WWindowController);
	WDISABLE_COPY_AND_ASSIGN(WWindowController);
};

} // namespace

#endif /* _APP_ASSIST_WWINDOW_CONTROLLER_H_ */
