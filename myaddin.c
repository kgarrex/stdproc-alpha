#include <windows.h>

#include "myaddin.h"
#include "msocomaddin.h"

int main(int argc, char** argv) {

	struct MSOCOMAddin addin;
	addin.COMServer.progid = "My.Addin";
	addin.COMServer.name = "My Addin";
	addin.COMServer.version  = 0x00010000;
	addin.COMServer.guid =  &guid_name;
	addin.COMServer.path = "C:\\users\\kgarrett\\appdata\\myaddin.dll";
	addin.description = "This is my COM addin";
	addin.loadbehavior = 2;
	
	if(*(argv+1))
		UnregisterMSOCOMAddin(&addin);
	else
		RegisterMSOCOMAddin(&addin);

	return 0;
}
