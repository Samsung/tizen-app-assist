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

#ifndef _APP_ASSIST_WWIDGET_H_
#define _APP_ASSIST_WWIDGET_H_

#include "WDefine.h"
#include "WWidgetInstance.h"

namespace app_assist {
/**
* @class	WWidget
* @brief	This class is the base class of a widget
*
* The %WWidget class is the base class of a widget. A widget class must be inherited from this class
*/

class WAPP_ASSIST_EXPORT WWidget {
public:
	/**
	 * This is the default constructor for this class.
	 */
	WWidget();

	/**
	 * This is the destructor for this class.
	 */
	virtual ~WWidget();

	/**
	 * Starts the widget.
	 *
	 * @param[in]	argc	The argument count
	 * @param[in]	argv	The argument vector
	 *
	 * @return	result
	 */
	int start(int argc, char* argv[]);

protected:
	/**
	 * Called when the widget is created
	 */
	virtual void onCreate() {};

	/**
	 * Called when the widget is terminated.
	 */
	virtual void onTerminate() {};

	/**
	 * Called when the widget instance is created to create WWidgetInstance instance.
	 *
	 * @return	An instance of WWidgetInstance. The instance should be newly allocated and it will be deallocated automatically.
	 */
	virtual WWidgetInstance* onCreateWidgetInstance() = 0;

private:
	WDECLARE_PRIVATE_IMPL(WWidget);
	WDISABLE_COPY_AND_ASSIGN(WWidget);
};

} // namespace

#endif // _APP_ASSIST_WWIDGET_H_
