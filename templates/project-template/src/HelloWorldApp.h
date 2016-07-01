#ifndef _HELLO_WORLD_APP_H_
#define _HELLO_WORLD_APP_H_

#include "WApp.h"
#include "WAppEventHandler.h"

class HelloWorldApp : public app_assist::WApp {
public:
	HelloWorldApp();
	~HelloWorldApp();
private:
	virtual bool onCreate();
	virtual void onTerminate();
	virtual void onAppControl(app_control_h request, bool firstLaunch);
	virtual void onPause();
	virtual void onResume();
private:
	app_assist::WAppEventHandler _languageChangedHandler;
};

#endif // _HELLO_WORLD_APP_H_
