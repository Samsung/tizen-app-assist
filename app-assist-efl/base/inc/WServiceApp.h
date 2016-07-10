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

#ifndef _APP_ASSIST_WSERVICE_APP_H_
#define _APP_ASSIST_WSERVICE_APP_H_

#include <service_app.h>
#include "WDefine.h"

namespace app_assist {
/**
* @class	WServiceApp
* @brief	This class is the base class of an service application
*
*
* The %WServiceApp class is the base class of an service application. An service application class must be inherited from this class
* @code
*
* // main.cpp
* WAPP_ASSIST_EXPORT int main( int argc, char* argv[] )
* {
*   MyServiceApp serviceApp;
*   int ret = serviceApp.start( argc, argv );
*   return ret;
* }
* @endcode
*/
class WAPP_ASSIST_EXPORT WServiceApp {
public:
	/**
	 * This is the default constructor for this class.
	 *
	 */
	WServiceApp();

	/**
	 * This is the destructor for this class.
	 *
	 */
	virtual ~WServiceApp();

public:
	/**
	 * Starts the service application.
	 *
	 * @param[in]	argc	The argument count
	 * @param[in]	argv    The argument vector
	 *
	 * @return result
	 */
	int start(int argc, char* argv[]);

protected:
	/**
	 * Called when the service application started
	 *
	 * @param[in]	argc	The argument count
	 * @param[in]	argv	The argument vector
	 *
	 * @return result
	 */
	virtual int onStart(int argc, char* argv[]);

	/**
	 * Called when the service application created
	 *
	 * @return	true if the service application created successfully, else false
	 */
	virtual bool onCreate() { return true; };

	/**
	 * Called when the service application is terminated.
	 *
	 */
	virtual void onTerminate() {};

	/**
	 * Called when some request comes to the service application
	 *
	 * @param[in]	request	handle of request information
	 * @param[in]	firstLaunch	true, if it is called after creation
	 */
	virtual void onAppControl(app_control_h request, bool firstLaunch);

private:
	WDECLARE_PRIVATE_IMPL(WServiceApp);
	WDISABLE_COPY_AND_ASSIGN(WServiceApp);

};

} // namespace

#endif /* _APP_ASSIST_WSERVICE_APP_H_ */
