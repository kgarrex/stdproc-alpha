#include "network.h"

int NetworkAsyncHandler(struct async_packet *AsyncPacket);
#include "network_ioctl.c"

/* compile to suppress macro already defined warnings
 * from including ntstatus.h
 * cl /wd4005 network.c
 */

void* CreateSocketPool(struct SocketPool* SocketPool, int Count);

int NetworkAsyncHandler(struct async_packet *AsyncPacket) {
	//printf("Executing network async handler %d\n", AsyncPacket->Object.OperationType);
	char* string = 0;
	switch(AsyncPacket->object.op_type) {

	case SOCKET_CONNECT: {
		//EnterCriticalSection(&NetAsync->CriticalSection);
		switch(AsyncPacket->object.wsaovl_s.Internal) {
		case STATUS_NETWORK_UNREACHABLE:
			string = "The remote network is not reachable by the transport";	
			break;
		}
		printf(" Connect has completed: %s\n", string);
		//LeaveCriticalSection(&NetAsync->CriticalSection);
	} break;

	case SOCKET_IOCTL: {
		//printf(" IOCTL has completed\n");	
	} break;

	case SOCKET_RIO: {
		printf(" RIO has completed\n");
		//RIONotify
	} break;

	case SOCKET_INIT: {
		WSADATA WSAData = {0};
		DWORD Error = 0;
		Error = WSAStartup(0x00020002, &WSAData);
		if(Error) {
			printf("Error initializing winsock\n");	
			return 0;
		}
		printf("Winsock 2 was successfully initialized\n");
	} break;

	} //end switch

	return 0;
}

void* CreateSocketPool(struct SocketPool* SocketPool, int Count) {
	DWORD Error = 0;
	struct SocketObject* SocketObject = 0;

	PIP_ADAPTER_ADDRESSES AdapterAddresses = 0;

	long BufSize = 0;
	Error = GetAdaptersAddresses
		(AF_INET,
		0,
		0, //Reserved
		AdapterAddresses,
		&BufSize);
	if(Error != ERROR_SUCCESS) {
		if(Error = ERROR_BUFFER_OVERFLOW) {
			printf("AdaptersAddresses buffer needed %d\n", BufSize); 
			return 0;
		}
		printf("Error %d: Could not get adapter addresses\n", Error);	
		return 0;
	}

	SocketPool->Count = Count;
	SocketPool->Head = SocketPool->NextSocket = SocketObject = 
		alloc_mem(sizeof(SocketObject) * Count);
	while(Count--) {
		SocketObject->Socket = WSASocket (
			SocketPool->Family,
			SocketPool->Type,
			SocketPool->Protocol,
			0,
			0, //Reserved
			WSA_FLAG_OVERLAPPED | WSA_FLAG_REGISTERED_IO);
		if(SocketObject->Socket == INVALID_SOCKET) {
			printf("Error: Could not create socket\n");
			return 0;
		}
		if(Count == 0) SocketObject->Next = 0;
		else {
			SocketObject->Next = SocketObject + sizeof(struct SocketObject);
			SocketObject = SocketObject->Next;
		}
	}
	return SocketPool;
}

void* PopSocketObject(struct SocketPool* SocketPool) {
	struct SocketObject* SocketObject;
	if(!SocketPool->NextSocket) {
		printf("Error: No socket objects available\n");		
	}
	else {
		SocketObject = SocketPool->NextSocket;	
		SocketPool->NextSocket = SocketPool->NextSocket->Next;
	}
	return SocketObject;
}

int WinAPI_Network(struct NetworkClient* NetClient) {
	WSAPROTOCOL_INFO* ProtocolInfoBuf = 0;
	DWORD Error = 0, Success = 0, BytesReturned;

	DWORD BufLen = 0;
	Error = WSAEnumProtocols(0, 0, &BufLen);
	if(Error == SOCKET_ERROR && (WSAGetLastError() != WSAENOBUFS)) {
		printf("Error %d: getting protocol info\n", WSAGetLastError());
		return 0;
	}

	ProtocolInfoBuf = malloc(BufLen);
	Error = WSAEnumProtocols(0, ProtocolInfoBuf, &BufLen);
	if(Error == SOCKET_ERROR) {
		printf("Error %d: Could not enumerate protocols\n", WSAGetLastError());
		return 0;	
	}

	//WSAEnumNameSpaceProviders(&BytesReturned, &NameSpaceBuffer);
	
	struct SocketPool SocketPool = {0};
	SocketPool.Family = AF_INET;
	SocketPool.Type = SOCK_STREAM;
	SocketPool.Protocol = IPPROTO_TCP;
	CreateSocketPool(&SocketPool, 16);

	struct SocketObject* SocketObject = PopSocketObject(&SocketPool);

	struct async_packet *AsyncPacket = 0;
	AsyncPacket->handler.fn = NetworkAsyncHandler;
	AsyncPacket->handler.thread_mode = THREAD_MODE_IO;

	if(!CreateIoCompletionPort (
		(HANDLE)SocketObject->Socket,
		process.hiocp,
		(ULONG_PTR)AsyncPacket,
		0))
	{
		Error = GetLastError();
		printf("Error %d: Could not associate handle"
				"with completion port", Error);
		return 0;
	}

	char* Hostname = "www.google.com";
	char* Port = "80";

	struct WSAExProcs WSAExProcs = {0};
	GetWSAExProcs(SocketObject, &WSAExProcs);

	/*
	HANDLE NameResolutionHandle;
	// last 3 parameters are only supported with _UNICODE macro
	Error = GetAddrInfoEx (
		Hostname, //hostname
		Port, //servicename
		NS_ALL, //namespace
		//pointer namespace GUID
		//pointer addrinfoex structure
		//linked list of addrinfoex structs
		//timeout value in milliseconds
		//Overlapped pointer
		//completion routine
		&NameResolutionHandle); //Handle to the kernel object
	*/

	SOCKADDR_IN ClientAddress = {0};
	ClientAddress.sin_family = AF_INET;
	ClientAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	ClientAddress.sin_port = 0;

	Error = bind (
		SocketObject->Socket,
		(const SOCKADDR*)&ClientAddress,
		sizeof(ClientAddress));
	if(Error == SOCKET_ERROR) {
		printf("Error %d: Could not bind socket\n", WSAGetLastError());
		return 0;	
	}

	SOCKADDR_IN ServerAddress = {0};
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr("173.194.37.36");
	ServerAddress.sin_port = htons(80);

	AsyncPacket = pop_async_packet(NetworkAsyncHandler, THREAD_MODE_IO, SOCKET_CONNECT);
	Success = WSAExProcs.ConnectEx (
		SocketObject->Socket,
	 	(const SOCKADDR*)&ServerAddress,
	 	sizeof(ServerAddress),
	 	0, //send buffer
	 	0, //send buffer size
	 	&BytesReturned, //number of bytes sent
		(WSAOVERLAPPED*)AsyncPacket);
	if(!Success && WSAGetLastError() != WSA_IO_PENDING) {
		printf("Error %d: Could not connect on socket\n", WSAGetLastError());	
		return 0;
	}

	/*
	BufLen = BufLen / sizeof(WSAPROTOCOL_INFO);
	for(int i = 0; i < BufLen; i++) {
		printf(" %s\n", ProtocolInfoBuf[i].szProtocol);	
		printf(" Address Family: %d\n", ProtocolInfoBuf[i].iAddressFamily);
		printf(" Socket Type: %d\n", ProtocolInfoBuf[i].iSocketType);
		printf(" Protocol: %d\n", ProtocolInfoBuf[i].iProtocol);
		printf(" ByteOrder: %d\n", ProtocolInfoBuf[i].iNetworkByteOrder);
		printf(" ServiceFlags: 0x%x\n", ProtocolInfoBuf[i].dwServiceFlags1);
		printf("\n");
	}
	*/

	AsyncPacket = pop_async_packet(NetworkAsyncHandler, THREAD_MODE_IO, SOCKET_RIO);
	RIO_NOTIFICATION_COMPLETION Completion = {0};
	Completion.Type = RIO_IOCP_COMPLETION;
	Completion.Iocp.IocpHandle = process.hiocp;
	Completion.Iocp.CompletionKey = AsyncPacket;
	Completion.Iocp.Overlapped = AsyncPacket;

	RIO_BUF RIOBuffer;
	RIO_BUFFERID RIOBufferId = 0;
	RIO_CQ RIOCompletionQueue;
	RIO_RQ RIORequestQueue;
	char _RIOBuffer[4096];

	RIOBufferId = WSAExProcs.RIOTable.RIORegisterBuffer(_RIOBuffer, 4096);
	if(RIOBufferId == RIO_INVALID_BUFFERID) {
		Error = WSAGetLastError();
		printf("Error %d: RIORegisterBuffer\n", Error);
		return 0;	
	}
	RIOBuffer.BufferId = RIOBufferId;
	RIOBuffer.Offset = 0;
	RIOBuffer.Length = 4096;

	RIOCompletionQueue = WSAExProcs.RIOTable.RIOCreateCompletionQueue(4096, &Completion);
	if(RIOCompletionQueue == RIO_INVALID_CQ) {
		Error = WSAGetLastError();
		printf("Error %d: RIOCompletionQueue\n", Error);	
		return 0;
	}

	RIORequestQueue = WSAExProcs.RIOTable.RIOCreateRequestQueue(
		SocketObject->Socket,
		8, 1, 8, 1,
		RIOCompletionQueue,
		RIOCompletionQueue,
		AsyncPacket);
	if(RIORequestQueue == RIO_INVALID_RQ) {
		Error = WSAGetLastError();	
		printf("Error %d: RIOCreateRequestQueue\n", Error);
		return 0;
	}
	
	if(ERROR_SUCCESS != WSAExProcs.RIOTable.RIONotify(RIOCompletionQueue)) {
		Error = WSAGetLastError();
		printf("Error %d: RIONotify\n", Error);
		return 0;
	}
	return 1;
}

void InitializeThreadRIO(struct thread *Thread,
		RIO_EXTENSION_FUNCTION_TABLE* RIOTable)
{
	DWORD Error = 0;
	RIO_NOTIFICATION_COMPLETION Notification;

	Thread->async_packet = pop_async_packet(NetworkAsyncHandler, THREAD_MODE_IO, 0);
	Notification.Type = RIO_IOCP_COMPLETION;
	Notification.Iocp.IocpHandle = process.hiocp;
	Notification.Iocp.CompletionKey = Thread->async_packet; //Network Async Handler
	Notification.Iocp.Overlapped = Thread->async_packet; 

	Thread->RIOCompletionQueue =
		RIOTable->RIOCreateCompletionQueue(4096, &Notification); 
	if(Thread->RIOCompletionQueue == RIO_INVALID_CQ) {
		Error = WSAGetLastError();
		printf("Error %d: RIOCompletionQueue\n", Error);
		return; 	
	}

	//Thread->RIORequestQueue = RIOTable->RIOCreateRequestQueue (
}
