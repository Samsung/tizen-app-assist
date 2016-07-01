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

#ifndef _APP_ASSIST_WSERVICE_APP_EVENT_HANDLER_H_
#define _APP_ASSIST_WSERVICE_APP_EVENT_HANDLER_H_

#include <functional>
#include <service_app.h>
#include "WDefine.h"

namespace app_assist {
/**
* @class	WServiceAppEventHandler
* @brief	This class is
*
*/
class WServiceAppEventHandler {
public:
	/**
	 * This is the default constructor for this class.
	 *
	 */
	WServiceAppEventHandler();

	/**
	 * This is the destructor for this class.
	 *
	 */
	virtual ~WServiceAppEventHandler();

public:
	/**
	 * Adds service application event handler of event type.
	 *
	 * @param[in]	eventType	event type to be handled
	 * @param[in]	handlerFunc	The pointer to handler function
	 * @param[in]	userData	user data pointer to be used in handler function
	 *
	 * @return 0 on success, otherwise error
	 */
	int addEventHandler(app_event_type_e eventType, std::function<void (app_event_info_h eventInfo, void* userData)> handlerFunc, void* userData);

	/**
	 * Removes service application event handler
	 *
	 */
	void removeEventHandler();

protected:
	/**
	 * Returns the handle of added event handler
	 *
	 * @return the handle
	 *
	 * @see addEventHandler()
	 */
	app_event_handler_h getHandle();

private:
	WDECLARE_PRIVATE_IMPL(WServiceAppEventHandler);
	WDISABLE_COPY_AND_ASSIGN(WServiceAppEventHandler);

};

} // namespace

#endif /* _APP_ASSIST_WSERVICE_APP_EVENT_HANDLER_H_ */
