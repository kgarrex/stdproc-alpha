#include "string\string.h"
#include "comserver.h"
#include "msocomaddin.h"

#define ADDIN_CREATE_REG_VALUE(name, value) \
	error = RegSetValueEx(retkey, name, 0, REG_SZ, \
	value, strlen(value)+1); \
	if(error != ERROR_SUCCESS) { \
		printf("Error %d: Failed to create value\n", \
			GetLastError()); \
		return; \
	}


void RegisterMSOCOMAddin(struct MSOCOMAddin* addin) {
	int error, existed; HKEY retkey;
	struct _strbuf* sb = 0;

	RegisterCOMServer((struct COMServer*)addin);

	sb = string_new(sb, 64,
	"SOFTWARE\\Microsoft\\Office\\Outlook\\Addins",
	0);

	string_concat(sb, "\\", addin->COMServer.progid, 0);

	error = RegCreateKeyEx(
		HKEY_CURRENT_USER,
		sb->head,
		0,
		0,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		0,
		&retkey,
		&existed
	);
	if(error != ERROR_SUCCESS) {
		printf("Error %d: Could not create key\n", error);
		return;
	}

	ADDIN_CREATE_REG_VALUE("Description", addin->description);
	ADDIN_CREATE_REG_VALUE("FriendlyName", addin->COMServer.name);

	error = RegSetValueEx(
		retkey, "LoadBehavior", 0, REG_DWORD,
		(char*)&addin->loadbehavior, 4 
	);
	if(error != ERROR_SUCCESS) {
		printf("Error %d: Failed to create value\n",
			GetLastError());
		return;
	}
}

void UnregisterMSOCOMAddin(struct MSOCOMAddin* addin) {
	int error = 0;
	struct _strbuf* sb = 0;

	UnregisterCOMServer((struct COMServer*)addin);

	sb = string_new(sb, 64,
	"SOFTWARE\\Microsoft\\Office\\Outlook\\Addins",
	0);

	string_concat(sb, "\\", addin->COMServer.progid, 0);

	error = RegDeleteTree(
		HKEY_CURRENT_USER,
		sb->head
	);
	if(error != ERROR_SUCCESS) {
		printf("Error %d: Could not delete key\n", error);
		return;
	}
}

#undef ADDIN_CREATE_REG_VALUE
