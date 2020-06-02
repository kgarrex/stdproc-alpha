#include <mapi.h>
#include <mapiaux.h>
#include <mapidbg.h>
#include <mapidefs.h>
#include <mapicode.h>
#include <mapiguid.h>
#include <mapitags.h>
#include <mapix.h>

typedef HRESULT (_stdcall *pfnMAPIInitialize)(MAPIINIT_0*);
enum {
	MAPI_INIT,
};

int MAPIAsyncHandler(struct AsyncPacket* AsyncPacket) {
	switch(AsyncPacket->Object.OperationType) {
	case MAPI_INIT: {
		HRESULT Result;
		MAPIINIT_0 MapiInit = {0};

		HMODULE MAPI32Dll = LoadLibrary("C:\\windows\\system32\\mapi32.dll");
		pfnMAPIInitialize MAPIInitialize = (pfnMAPIInitialize)GetProcAddress(MAPI32Dll, "MAPIInitialize");
		MapiInit.ulVersion = 0;
		//if NT SERVICE set MAPI_INIT_SERVICE
		MapiInit.ulFlags = MAPI_MULTITHREAD_NOTIFICATIONS | MAPI_NO_COINIT;

		Result = MAPIInitialize(&MapiInit);
		if(SUCCEEDED(Result))
			printf("Initialized MAPI!\n");
		else
			printf("Failed to initialize MAPI\n");
	} break;

	} //end switch
	return 1;
}
