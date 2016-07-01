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

#ifndef _APP_ASSIST_WWIDGET_WINDOW_CONTROLLER_H_
#define _APP_ASSIST_WWIDGET_WINDOW_CONTROLLER_H_

#include <widget_app.h>
#include "WDefine.h"
#include "IWWindowController.h"
#include "WViewController.h"

namespace app_assist {

/**
* @class	WWidgetWindowController
* @brief	This class represents window and base layout evas object.
*
* The %WWidgetWindowController manages window evas object and common base evas objects.
*/
class WAPP_ASSIST_EXPORT WWidgetWindowController : public IWWindowController {
public:
	/**
	 * This is the default constructor for this class.
	 */
	WWidgetWindowController();

	/**
	 * Creates the windows
	 *
	 * @param[in]	context	The context of widget instance
	 * @param[in]	width	The pixel value for widget width
	 * @param[in]	height	The pixel value for widget height
	 *
	 * @return	true if the windows created sucessfully, else false
	 */
	bool create(widget_context_h context, int width, int height);

	/**
	 * Destroys the window class instance and delete window evas object.
	 * "onDestroy" will be called subsequently.
	 *
	 * @see onDestroy()
	 */
	void destroy();

	/**
	 * Attaches base UI object, which can be WViewController instance.
	 *
	 * @param[in]	baseObj	WViewController instance to be attached
	 * @param[in]	creationParam	parameter to be passed as parameter of create method of the WViewController class
	 *
	 * @return	true if the base ui object attached sucessfully, else false
	 * @see getBaseViewController()
	 */
	bool attachBaseViewController(WViewController* vc, void* creationParam);
	bool attachBaseViewController(WViewController* vc);

	/**
	 * Detaches the base UI object.
	 *
	 * @return	The base UI object, which should be deleted explicitly later.
	 */
	WViewController* detachBaseViewController();

	/**
	 * Gets the class name.
	 *
	 * @return	The class name
	 */
	virtual const char* getClassName() const; //override;

	/**
	 * Gets the evas object of window.
	 *
	 * @return	The Evas object
	 */
	virtual Evas_Object* getEvasObject() const; //override;

	/**
	 * Gets the base layout evas object
	 *
	 * @return	The pointer to the evas object of the layout
	 */
	virtual Evas_Object* getBaseLayoutEvasObject() const; //override;

	/**
	 * Gets the conformant evas object.
	 *
	 * @return	The pointer to the evas object of the conformant
	 */
	virtual Evas_Object* getConformantEvasObject() const; //override;

	/**
	 * Gets the base view controller attached.
	 *
	 * @return	The pointer to the base view controller
	 * @see attachBaseViewController()
	 */
	virtual WViewController* getBaseViewController() const; //override;

protected:
	/**
	 * This is the destructor for this class.
	 *
	 */
	virtual ~WWidgetWindowController();

	/**
	 * Called when create method is called to create window
	 * Override this method to create your own style window evas object.
	 *
	 * @param[in]	context	The context of widget instance
	 * @param[in]	width	The pixel value for widget width
	 * @param[in]	height	The pixel value for widget height
	 *
	 * @return The pointer to the evas object of the window created.
	 * @see create()
	 */
	virtual Evas_Object* onCreateWin(widget_context_h context, int width, int height);

	/**
	 * Called when the window is destroyed
	 * Override this method to do something on destruction.
	 *
	 */
	virtual void onDestroy() {};

	/**
	 * Called when create method is called to create base layout.
	 * Override this method to create your own base layout and conformant evas objects.
	 *
	 * @param[in]	win	A pointer to evas object of the window
	 * @param[out]	outConformant The pointer to the evas object of the conformant created.
	 *
	 * @return	The pointer to the evas object of base layout created.
	 */
	virtual Evas_Object* onCreateBaseLayout(Evas_Object* win, Evas_Object** outConformant) { return nullptr; };

	/**
	 * Called when attachBaseViewController is called.
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
	virtual void onDetachBaseViewController(WViewController* vc) {};

private:
	WDECLARE_PRIVATE_IMPL(WWidgetWindowController);
	WDISABLE_COPY_AND_ASSIGN(WWidgetWindowController);
};
} // namespace

#endif // _APP_ASSIST_WWIDGET_WINDOW_CONTROLLER_H_
