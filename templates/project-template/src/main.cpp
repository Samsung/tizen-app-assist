#include "HelloWorldDebug.h"
#include "HelloWorldApp.h"

WAPP_ASSIST_EXPORT int main(int argc, char* argv[]) {
	WHIT();

	HelloWorldApp app;
	int ret = app.start(argc, argv);

	WDEBUG("app return=%d", ret);
	return ret;
}
