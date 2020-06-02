#define ROTATE_SHORT(x) \
	(short)x << 8 

//static inline stringbuf_t GUIDToString(GUID* Guid) {
char* GUID_toString(struct _GUID* guid, struct _strbuf* buf) {
	short tmp = 0;
	char str[10];
	string_concat(buf, "{", 0);
	string_concat
	(buf, integer_toString((guid->Data1 >> 16), buf->pos, 16), 0);

	string_concat
	(buf, integer_toString((short)guid->Data1, buf->pos, 16), "-", 0);

	string_concat
	(buf, integer_toString(guid->Data2, buf->pos, 16), "-", 0);

	string_concat
	(buf, integer_toString(guid->Data3, buf->pos, 16), "-", 0);

	string_concat
	(buf, integer_toString(guid->Data4[0], buf->pos, 16), 0);
	string_concat
	(buf, integer_toString(guid->Data4[1], buf->pos, 16), "-", 0);

	string_concat
	(buf, integer_toString(guid->Data4[2], buf->pos, 16), 0);
	string_concat
	(buf, integer_toString(guid->Data4[3], buf->pos, 16), 0);
	string_concat
	(buf, integer_toString(guid->Data4[4], buf->pos, 16), 0);
	string_concat
	(buf, integer_toString(guid->Data4[5], buf->pos, 16), 0);
	string_concat
	(buf, integer_toString(guid->Data4[6], buf->pos, 16), 0);
	string_concat
	(buf, integer_toString(guid->Data4[7], buf->pos, 16), 0);

	string_concat(buf, "}", 0);
	return buf->head;
}

char* _get_progid(struct COMServer* cs, struct _strbuf* buf) {
	char tmp[12];
	if(!cs->version) {
		//err: must set a version
		return 0;
	}
	string_concat(
		buf,
		integer_toString(cs->version >> 16, tmp, 10),
		0
	);
	if((short)cs->version) {
		string_concat(
			buf,
			".",
			integer_toString((short)cs->version, tmp, 10),
			0
		);
	}
	return buf->head;
}


