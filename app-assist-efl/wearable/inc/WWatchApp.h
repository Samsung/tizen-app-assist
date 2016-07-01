/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd. All rights reserved.
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

#ifndef _APP_ASSIST_WWATCH_APP_H_
#define _APP_ASSIST_WWATCH_APP_H_

#include <watch_app.h>
#include "WDefine.h"

namespace app_assist {

// forward declaration
class WWatchWindowController;

/**
* @class	WWatchApp
* @brief	This class is the base class of an watch application
*
*
* The %WWatchApp class is the base class of an watch application. An watch application class must be inherited from this class
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
class WAPP_ASSIST_EXPORT WWatchApp {
public:
	/**
	 * This is the default constructor for this class.
	 */
	WWatchApp();

	/**
	 * This is the destructor for this class.
	 */
	virtual ~WWatchApp();

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
	 * Called when the application created.
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
	virtual void onAppControl(app_control_h request, bool firstLaunch) {};

	/**
	 * Called at each second. This callback is not called while the app is paused or the device is in ambient mode.
	 *
	 * @param[in]	watchTime	The watch time handle.
	 */
	virtual void onTimeTick(watch_time_h watchTime) {};

	/**
	 * Called at each minute when the device in the ambient mode.
	 *
	 * @param[in]	watchTime	The watch time handle.
	 */
	virtual void onAmbientModeTimeTick(watch_time_h watchTime) {};

	/**
	 * Called when the device enters or exits the ambient mode.
	 *
	 * @param[in]	ambientMode	If true the device enters the ambient mode, otherwise false
	 */
	virtual void onAmbientModeChanged(bool ambientMode) {};

	/**
	 * Gets the window class instance of the application.
	 *
	 * @return	The pointer to the window
	 */
	WWatchWindowController* getWindowController();

	/**
	 * Attaches a window to the application.
	 *
	 * @param[in]	win	A pointer to the window to attach
	 * @return	true if the window attached sucessfully, else false
	 */
	bool attachWindowController(WWatchWindowController* win);

	/**
	 * Gets the screen width
	 */
	int getScreenWidth();

	/**
	 * Gets the screen height
	 */
	int getScreenHeight();

private:
	WDECLARE_PRIVATE_IMPL(WWatchApp);
	WDISABLE_COPY_AND_ASSIGN(WWatchApp);
};

} // namespace

#endif /* _APP_ASSIST_WWATCH_APP_H_ */
