
struct IPEndpoint {
	union {
		union {
			char* string; struct { char oct1;
				char oct2;
				char oct3;
				char oct4;
			} addr;
		} IPv4;

		union {
			char* string;
			struct {
				short hex1;
				short hex2;
				short hex3;
				short hex4;
				short hex5;
				short hex6;
				short hex7;
				short hex8;	
			} addr; 
		} IPv6;
	} IPAddress;
	short Port;
	short Family;
};

struct SocketPool {
	void* Head;
	struct SocketObject* NextSocket;
	char Family;
	char Type;
	short Protocol;
	short Count;
};

struct SocketObject {
	struct SocketObject* Next;	
	SOCKET Socket;
};

struct NetworkClient {
	SOCKET Socket; // = INVALID_SOCKET;
	RIO_BUF RIOBuffer;
};

struct NetworkServer {
	SOCKET ListenSocket; // = INVALID_SOCKET;
	SOCKET AcceptSocket; // = INVALID_SOCKET;
};

struct WSAExProcs {
LPFN_CONNECTEX               ConnectEx;
LPFN_ACCEPTEX                AcceptEx;
LPFN_DISCONNECTEX            DisconnectEx;
LPFN_TRANSMITPACKETS         TransmitPackets;
LPFN_TRANSMITFILE            TransmitFile;
LPFN_GETACCEPTEXSOCKADDRS    GetAcceptExSockaddrs;
LPFN_WSARECVMSG              WSARecvMsg;
LPFN_WSASENDMSG              WSASendMsg;
RIO_EXTENSION_FUNCTION_TABLE RIOTable;
};

enum {
	SOCKET_CONNECT = 1,
	SOCKET_IOCTL,
	SOCKET_RIO,
	SOCKET_INIT,
};
