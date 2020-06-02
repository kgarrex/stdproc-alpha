
void GetWSAExProcs(
struct SocketObject* SocketObject,
struct WSAExProcs* procs)
{
DWORD NOT_USED = 0, error = 0;

GUID guids[9] =  {
WSAID_CONNECTEX,
WSAID_ACCEPTEX,
WSAID_DISCONNECTEX,
WSAID_GETACCEPTEXSOCKADDRS,
WSAID_TRANSMITFILE,
WSAID_TRANSMITPACKETS,
WSAID_WSARECVMSG,
WSAID_WSASENDMSG,
WSAID_MULTIPLE_RIO
};

GUID ConnectExGUID            = WSAID_CONNECTEX;
GUID AcceptExGUID             = WSAID_ACCEPTEX;
GUID DisconnectExGUID         = WSAID_DISCONNECTEX;
GUID GetAcceptExSockaddrsGUID = WSAID_GETACCEPTEXSOCKADDRS;
GUID TransmitFileGUID         = WSAID_TRANSMITFILE;
GUID TransmitPacketsGUID      = WSAID_TRANSMITPACKETS;
GUID WSARecvMsgGUID           = WSAID_WSARECVMSG;
GUID WSASendMsgGUID           = WSAID_WSASENDMSG;
GUID RIOTableGUID             = WSAID_MULTIPLE_RIO;

struct async_packet *async_packet;

#define GetWSAExProc(ProcName, ProcGuid, ProcFn, ProcString) \
	async_packet = pop_async_packet(NetworkAsyncHandler, THREAD_MODE_IO, SOCKET_IOCTL); \
	if(SOCKET_ERROR == WSAIoctl( \
		SocketObject->Socket, \
		SIO_GET_EXTENSION_FUNCTION_POINTER, \
		&ProcGuid, \
		sizeof(GUID), \
		(void*)&Procs->ProcName, \
		sizeof(ProcFn), \
		&NOT_USED, \
		(WSAOVERLAPPED*)async_packet, \
		0)) \
	{ \
		if((Error = WSAGetLastError()) != WSA_IO_PENDING) { \
			printf("Error %d: Could not get function pointer for %s\n", Error, ProcString); \
		} \
	}

/*
for(i = 0; i < 10; i++)
{
error = WSAIoctl
(SocketObject->Socket, SIO_GET_EXTENSION_FUNCTION_POINTER,
 &guids[i], BYTESIZE(GUID), &(procs+i), sizeof(stdcall_t),
 &NOT_USED, (WSAOVERLAPPED*)async_packet, 0); 
}
*/

GetWSAExProc(ConnectEx, ConnectExGUID, LPFN_CONNECTEX, "ConnectEx");
GetWSAExProc(AcceptEx, AcceptExGUID, LPFN_ACCEPTEX, "AcceptEx");
GetWSAExProc(DisconnectEx, DisconnectExGUID, LPFN_DISCONNECTEX, "DisconnectEx");
GetWSAExProc(TransmitPackets, TransmitPacketsGUID, LPFN_TRANSMITPACKETS, "TransmitPackets");
GetWSAExProc(TransmitFile, TransmitFileGUID, LPFN_TRANSMITFILE, "TransmitFile");
GetWSAExProc(GetAcceptExSockaddrs, GetAcceptExSockaddrsGUID, LPFN_GETACCEPTEXSOCKADDRS, "GetAcceptExSockaddrs");
GetWSAExProc(WSARecvMsg, WSARecvMsgGUID, LPFN_WSARECVMSG, "WSARecvMsg");
GetWSAExProc(WSASendMsg, WSASendMsgGUID, LPFN_WSASENDMSG, "WSASendMsg");

/* Get RIO Extension function pointers */
async_packet = pop_async_packet(NetworkAsyncHandler, THREAD_MODE_IO, SOCKET_IOCTL);
Procs->RIOTable.cbSize = sizeof(RIO_EXTENSION_FUNCTION_TABLE);
error = WSAIoctl
(SocketObject->Socket,
 SIO_GET_MULTIPLE_EXTENSION_FUNCTION_POINTER,
 &RIOTableGUID,
 sizeof(GUID),
 (void*)&Procs->RIOTable,
 sizeof(RIO_EXTENSION_FUNCTION_TABLE),
 &NOT_USED,
 (WSAOVERLAPPED*)async_packet,
 0);
    if(error = SOCKET_ERROR)
        if((Error = WSAGetLastError()) != WSA_IO_PENDING)
	printf("Error %d: Could not get function pointer for %s\n",
		Error, "RIOTable");
}
