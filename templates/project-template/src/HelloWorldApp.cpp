#include "HelloWorldApp.h"
#include <runtime_info.h>
#include <tizen.h>
#include <system_settings.h>
#include "WWindowController.h"
#include "WNaviframeController.h"
#include "HelloWorldMainViewController.h"
#include "HelloWorldDebug.h"


#define PACKAGE_NAME "org.example.project-template"

using namespace app_assist;

HelloWorldApp::HelloWorldApp() {
}

HelloWorldApp::~HelloWorldApp() {
}

bool HelloWorldApp::onCreate() {
	_languageChangedHandler.addEventHandler(APP_EVENT_LANGUAGE_CHANGED, [](app_event_info_h eventInfo, void* userData) {
		char* locale = NULL;
		system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
		elm_language_set(locale);
		free(locale);
	}, this);

	attachWindowController(new WWindowController(PACKAGE_NAME, ELM_WIN_BASIC));

	WNaviframeController* navi = new WNaviframeController();

	// Add following statements, if you want to terminate application instead of going background
	/*
	navi->setOnLastItemPop([] (bool* popOut) {
		*popOut = true;
		elm_exit();
	});
	*/
	getWindowController()->attachBaseViewController(navi);
	return true;
}

void HelloWorldApp::onAppControl(app_control_h request, bool firstLaunch) {
	WENTER();

	if (firstLaunch) {
		WNaviframeController* frame = (WNaviframeController*)getWindowController()->getBaseViewController();
		frame->push(new HelloWorldMainViewController());
	}

	WLEAVE();
}

void HelloWorldApp::onTerminate() {
}

void HelloWorldApp::onPause() {
}

void HelloWorldApp::onResume() {
}

