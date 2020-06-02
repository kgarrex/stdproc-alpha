
#define COMSERVER_CREATE_REG_KEY() \
	error = RegCreateKeyExA( \
		HKEY_CURRENT_USER, \
		key, \
		0,  \
		0,  \
		REG_OPTION_NON_VOLATILE, \
		KEY_ALL_ACCESS,  \
		0, \
		&retkey, \
		&existed \
	); \
	if(error != ERROR_SUCCESS) { \
		printf("Error %d: Failed to create reg key\n", \
		GetLastError()); \
		return; \
	} \
	if(existed == 2) { \
		printf("Key '%s' already existed\n", key); \
	}

#define COMSERVER_CREATE_REG_VALUE(name, value) \
	RegSetValueEx(retkey, name, 0, REG_SZ, \
	value, strlen(value))

void RegisterCOMServer(struct COMServer *comserver) {
	int error, existed; HKEY retkey;
	char *pos, *key;
	struct _strbuf *sb = 0, *guid = 0;

	sb = string_new(sb, 256, "SOFTWARE\\Classes", 0);
	guid = string_new(0, 39, 0);
	pos = sb->pos;

	for(int i = 0; i < MAX_KEYS; i++) {
		switch(i) {
		case VERSION_INDEPENDENT_PROG_ID: {
			struct _strbuf* buf = 0;
			char* pos = 0;
			key = string_concat(sb, "\\", comserver->progid, 0);

			COMSERVER_CREATE_REG_KEY();
			COMSERVER_CREATE_REG_VALUE(0, comserver->name);
			pos = sb->pos;

			key = string_concat(sb, "\\CLSID", 0);
			COMSERVER_CREATE_REG_KEY();
			COMSERVER_CREATE_REG_VALUE(0, guid->head); 
			sb->pos = pos;

			buf = string_new(buf, 32, comserver->progid, ".", 0);
			key = string_concat(sb, "\\CurVer", 0);
			COMSERVER_CREATE_REG_KEY();
			COMSERVER_CREATE_REG_VALUE(0, _get_progid(comserver, buf));
			
		}
		sb->pos = pos;
		break;
	
		case PROG_ID: {
			char* pos = 0;
			//convert the verison number to string
			//and concat to viprogid
			string_concat(sb, "\\", comserver->progid, ".", 0);
			key = _get_progid(comserver, sb);
			COMSERVER_CREATE_REG_KEY();
			COMSERVER_CREATE_REG_VALUE(0, comserver->name);

			key = string_concat(sb, "\\CLSID", 0);
			COMSERVER_CREATE_REG_KEY();
			COMSERVER_CREATE_REG_VALUE(0, guid->head);
		}
		sb->pos = pos;
		break;

		case CONST_CLSID: {
			struct _strbuf* buf = 0;
			char* pos = 0;
			//Convert the GUID to a string	
			GUID_toString(comserver->guid, guid); 

			key = string_concat(sb, "\\CLSID\\", guid->head, 0);
			COMSERVER_CREATE_REG_KEY();
			COMSERVER_CREATE_REG_VALUE(0, comserver->name);
			pos = sb->pos;

			key = string_concat(sb, "\\InProcServer32", 0);
			COMSERVER_CREATE_REG_KEY();
			COMSERVER_CREATE_REG_VALUE(0, comserver->path);
			COMSERVER_CREATE_REG_VALUE("ThreadingModel", "Both");
			sb->pos = pos;

			buf = string_new(buf, 32, comserver->progid, ".", 0);	
			key = string_concat(sb, "\\ProgID", 0);
			COMSERVER_CREATE_REG_KEY();
			COMSERVER_CREATE_REG_VALUE(0, _get_progid(comserver, buf));
			sb->pos = pos;

			key = string_concat(sb, "\\VersionIndependentProgID", 0);
			COMSERVER_CREATE_REG_KEY();
			COMSERVER_CREATE_REG_VALUE(0, comserver->progid);
			sb->pos = pos;

			buf->pos = buf->head;
			key = string_concat(sb, "\\Version", 0);
			COMSERVER_CREATE_REG_KEY();
			COMSERVER_CREATE_REG_VALUE(0, _get_progid(comserver, buf));
		}
		sb->pos = pos;
		break;

		}
	}
}

#undef COMSERVER_CREATE_REG_KEY
#undef COMSERVER_CREATE_REG_VALUE
