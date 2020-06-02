enum {
	COM_INIT,
};

int COMAsyncHandler(struct AsyncPacket* AsyncPacket) {
	switch(AsyncPacket->Object.OperationType) {
	case COM_INIT: {
		//Initialize the COM library
		HRESULT COM = CoInitializeEx(0, COINIT_MULTITHREADED);
		if(COM == S_OK) printf("The COM library was successfully initialized\n");
	} break;

	}
	return 1;
}
