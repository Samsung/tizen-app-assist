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

#ifndef _APP_ASSIST_IWWINDOW_CONTROLLER_H_
#define _APP_ASSIST_IWWINDOW_CONTROLLER_H_

#include <Evas.h>
#include "WDefine.h"

namespace app_assist {

class WViewController;

/**
* @class	IWWindowController
* @brief	This class is the interface class for the elements related to the window.
*
* The %IWWindowController provides interfaces for evas object related to the window class instance.
*/
class WAPP_ASSIST_EXPORT IWWindowController {
public:
	/**
	 * Gets the class name.
	 *
	 * @return the class name
	 */
	virtual const char* getClassName() const = 0;

	/**
	 * Gets the evas object of window.
	 *
	 * @return evas object
	 */
	virtual Evas_Object* getEvasObject() const = 0;

	/**
	 * Gets the base layout evas object
	 *
	 * @return The pointer to the evas object of the layout
	 */
	virtual Evas_Object* getBaseLayoutEvasObject() const = 0;

	/**
	 * Gets the conformant evas object.
	 *
	 * @return The pointer to the evas object of the conformant
	 */
	virtual Evas_Object* getConformantEvasObject() const = 0;

	/**
	 * Gets the base view controller attached.
	 *
	 * @return The pointer to the base view controller
	 * @see attachBaseViewController()
	 */
	virtual WViewController* getBaseViewController() const = 0;

protected:
	/**
	 * This is the destructor for this class.
	 *
	 */
	virtual ~IWWindowController() {};
};

} // namespace

#endif /* _APP_ASSIST_IWWINDOW_CONTROLLER_H_ */
