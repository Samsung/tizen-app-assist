#include "HelloWorldMainViewController.h"
#include <app.h>
#include "HelloWorldDebug.h"
#include "WMenuPopupController.h"
#include "WPopupController.h"
#include "IWWindowController.h"

#define EDJ_FILE "edje/project-template.edj"
#define GRP_MAIN "main"


using namespace app_assist;

HelloWorldMainViewController::HelloWorldMainViewController() {
}

HelloWorldMainViewController::~HelloWorldMainViewController() {

}

Evas_Object* HelloWorldMainViewController::onCreateView(Evas_Object* parent, void* viewParam) {
	setTitle("app-assist-efl");

	char edjPath[PATH_MAX] = {0,};

	char* resPath = app_get_resource_path();
	if (resPath) {
		snprintf(edjPath, sizeof(edjPath), "%s%s", resPath, EDJ_FILE);
		free(resPath);
	}
	Evas_Object* layout = elm_layout_add(parent);
	elm_layout_file_set(layout, edjPath, GRP_MAIN);
	elm_object_part_text_set(layout, "txt_title", "Hello World!");
	evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	return layout;
}

void HelloWorldMainViewController::onDestroy() {

}

void HelloWorldMainViewController::onPushed(Elm_Object_Item* naviItem) {
	//more Button
	setMoreButton(naviItem, _moreBtnSelectCb, this);
}

void HelloWorldMainViewController::_moreBtnSelectCb(void* data, Evas_Object* obj, void* event_info) {
	WHIT();
	HelloWorldMainViewController* p = (HelloWorldMainViewController*)data;

	WMenuPopupController* menu = new WMenuPopupController();
	menu->createMenu(p->getWindowController()->getEvasObject(), p->getNaviframeController()->getEvasObject());

	menu->appendItem("Show next view", [p]() {
		WContentViewController* vc = new WContentViewController();
		p->getNaviframeController()->push(vc);
	});
	menu->appendItem("Show pop-up", [p]() {
		WPopupController* popup = new WPopupController("Hello", "This is Pop-up!");
		p->attachPopup(popup);
	});

	menu->appendItem("Exit", [p]() {
		p->popOut();
	});
	menu->showMenu();

	p->attachPopup(menu);
}




