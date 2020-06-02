#ifndef COMSERVER_H
#define COMSERVER_H

#include <windows.h>
#include <stdio.h>

typedef union
{
char *a;
wchar_t *w;
} string_t*;

struct mso_office_addin
{
struct com_server;
};

struct com_server
{
GUID* guid;
uint32 version;
string_t progid;
string_t name;
string_t path;
};

void register_com_server(struct com_server *);
void unregister_com_server(struct com_server *);

#endif
