#include "comserver.h"

struct MSOCOMAddin {
	struct COMServer COMServer;
	char* description;
	int loadbehavior;
};

void RegisterMSOCOMAddin(struct MSOCOMAddin*);
void UnregisterMSOCOMAddin(struct MSOCOMAddin*);
