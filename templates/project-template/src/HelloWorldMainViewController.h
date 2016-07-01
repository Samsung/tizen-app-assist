#ifndef _HELLO_WORLD_MAIN_VIEW_CONTROLLER_H_
#define _HELLO_WORLD_MAIN_VIEW_CONTROLLER_H_

#include "WContentViewController.h"

class HelloWorldMainViewController: public app_assist::WContentViewController {
public:
	HelloWorldMainViewController();
protected:
	~HelloWorldMainViewController();

private:
	virtual Evas_Object* onCreateView(Evas_Object* parent, void* viewParam);
	virtual void onDestroy();
	virtual void onPushed(Elm_Object_Item* naviItem);

private:
	static void _moreBtnSelectCb(void* data, Evas_Object* obj, void* event_info);
};



#endif // _HELLO_WORLD_MAIN_VIEW_CONTROLLER_H_
