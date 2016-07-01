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

#ifndef _APP_ASSIST_WAPP_H_
#define _APP_ASSIST_WAPP_H_

#include <app.h>
#include "WDefine.h"

namespace app_assist {

// forward declaration
class WWindowController;

/**
* @class	WApp
* @brief	This class is the base class of an application
*
*
* The %WApp class is the base class of an application. An application class must be inherited from this class
* @code
*
* // main.cpp
* WAPP_ASSIST_EXPORT int main( int argc, char* argv[] )
* {
*   MyApp app;
*   int ret = app.start( argc, argv );
*   return ret;
* }
* @endcode
*/
class WAPP_ASSIST_EXPORT WApp {
public:
	/**
	 * This is the default constructor for this class.
	 *
	 */
	WApp();

	/**
	 * This is the destructor for this class.
	 *
	 */
	virtual ~WApp();

public:
	/**
	 * Starts the application.
	 *
	 * @param[in]	argc	The argument count
	 * @param[in]	argv    The argument vector
	 *
	 * @return result
	 */
	int start(int argc, char* argv[]);

protected:
	/**
	 * Called when the application started
	 *
	 * @param[in]	argc	The argument count
	 * @param[in]	argv	The argument vector
	 *
	 * @return result
	 */
	virtual int onStart(int argc, char* argv[]);

	/**
	 * Called when the application created
	 *
	 * @return	true if the application created successfully, else false
	 */
	virtual bool onCreate() { return true; };

	/**
	 * Called when the application is terminated.
	 *
	 */
	virtual void onTerminate() {};

	/**
	 * Called when the application is paused.
	 *
	 */
	virtual void onPause() {};

	/**
	 * Called when the application is resumed.
	 *
	 */
	virtual void onResume() {};

	/**
	 * Called when some request comes to the application
	 *
	 * @param[in]	request	handle of request information
	 * @param[in]	firstLaunch	true, if it is called after creation
	 */
	virtual void onAppControl(app_control_h request, bool firstLaunch);

	/**
	 * Gets the window class instance of the application.
	 *
	 * @return	The pointer to the window
	 */
	WWindowController* getWindowController() const;

	/**
	 * Attaches a window to the application.
	 *
	 * @param[in]	win	A pointer to the window to attach
	 * @return	true if the window attached sucessfully, else false
	 */
	bool attachWindowController(WWindowController* win);

private:
	WDECLARE_PRIVATE_IMPL(WApp);
	WDISABLE_COPY_AND_ASSIGN(WApp);
};

} // namespace

#endif /* _APP_ASSIST_WAPP_H_ */
