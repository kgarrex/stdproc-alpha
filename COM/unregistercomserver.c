#if 0 
#define COMSERVER_DELETE_REG_KEY() \
	error = RegDeleteKeyEx( \
		HKEY_CURRENT_USER, \
		key, \
		0, \
		0 \
	); \
	if( error != ERROR_SUCCESS) { \
		printf("Error %d: Could not delete key\n", error); \
		return; \
	} 
#else
#define COMSERVER_DELETE_REG_KEY() \
	error = RegDeleteTree( \
		HKEY_CURRENT_USER, \
		key \
	); \
	if(error != ERROR_SUCCESS) { \
		printf("Error %d: Could not delete key\n", error); \
		return; \
	}
#endif

void UnregisterCOMServer(struct COMServer *comserver) {
	int error; char *pos, *key;
	struct _strbuf *sb = 0, *guid = 0;

	sb = string_new(sb, 256, "SOFTWARE\\Classes", 0);
	guid = string_new(0, 39, 0);
	pos = sb->pos;

	for(int i = 0; i < MAX_KEYS; i++) {
		switch(i) {
		case VERSION_INDEPENDENT_PROG_ID: {
			key = string_concat(sb, "\\", comserver->progid, 0);
			COMSERVER_DELETE_REG_KEY();
		}
		sb->pos = pos;
		break;
	
		case PROG_ID: {
			//convert the verison number to string
			//and concat to viprogid
			string_concat(sb, "\\", comserver->progid, ".", 0);
			key = _get_progid(comserver, sb);
			COMSERVER_DELETE_REG_KEY();
		}
		sb->pos = pos;
		break;

		case CONST_CLSID: {
			//Convert the GUID to a string	
			GUID_toString(comserver->guid, guid); 
			key = string_concat(sb, "\\CLSID\\", guid->head, 0);
			COMSERVER_DELETE_REG_KEY();
		}
		sb->pos = pos;
		break;

		}
	}
}
#undef COMSERVER_DELETE_REG_KEY
