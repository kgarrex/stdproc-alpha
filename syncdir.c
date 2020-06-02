#include <stdio.h>
#include <windows.h>

#include "core.h"

#define BUF_SIZE 256

#define FLAG_RECURSIVE   0x0001
#define FLAG_PROMPT_EACH 0x0002
#define FLAG_PROMPT_ONCE 0x0004
#define FLAG_VERBOSE     0x0008

enum
{
E_INVALID_DIR,
E_PARAM_COUNT
};

int foreach_file1(char *dir)
{
return 0;
}

#define is_full_path is_full_path1
BOOL is_full_path1(char *dir)
{
return 1;
}

int syncdir1(char *targetdir, char *sourcedir, int flags)
{
int i;
DWORD targetl, sourcel;
BOOL error, found = 0;
HANDLE hfind_target,
	   hfind_source;
WIN32_FIND_DATA
	targetfile = {0},
	sourcefile = {0};
char target[BUF_SIZE],
     source[BUF_SIZE];
SYSTEMTIME time;

targetl = lstrlen(targetdir);
sourcel = lstrlen(sourcedir);
CopyMemory(target, targetdir, targetl);
CopyMemory(source, sourcedir, sourcel);
target[targetl] = '\0';
source[sourcel] = '\0';
lstrcat(target, "\\*"); targetl++;
lstrcat(source, "\\*"); sourcel++;

if(!is_full_path(targetdir))
{
GetCurrentDirectory(BUF_SIZE, target);
}

if(!is_full_path(sourcedir))
{
GetCurrentDirectory(BUF_SIZE, source);
}

hfind_target = FindFirstFile(target, &targetfile);
hfind_source = FindFirstFile(source, &sourcefile);
if(hfind_target == INVALID_HANDLE_VALUE ||
   hfind_source == INVALID_HANDLE_VALUE)
{
error = GetLastError();
    if(ERROR_FILE_NOT_FOUND == error)
    {
    printf("Directory doesn't exists\n");
    return E_INVALID_DIR;
    }
}

i = 2;
while(i--) FindNextFile(hfind_target, &targetfile);
i = 2;
while(i--) FindNextFile(hfind_source, &sourcefile);

/* go through each file in source and compare against all files in target */
do {
source[sourcel] = '\0';
    do {
    source[sourcel] = '\0';
    target[targetl] = '\0';
        /* compare the file names */
        if(0 != lstrcmpA(sourcefile.cFileName,targetfile.cFileName))
	continue;

    found = 1;
        /* if both are directories, check for recursive flag */
        if((sourcefile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
        targetfile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
	    if(flags & FLAG_RECURSIVE) {
            lstrcatA(target, targetfile.cFileName);
            printf("Recursively calling sync_dir on '%s'\n", target);
            /* call sync_dir here */
            }
        continue;
        }
        /* if source is dir and target is file, delete file and copy dir */
        if((sourcefile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
        targetfile.dwFileAttributes & ~FILE_ATTRIBUTE_DIRECTORY))
        {
        lstrcatA(target, targetfile.cFileName);
        lstrcatA(source, sourcefile.cFileName);
        printf("Deleting file '%s' and Copying directory '%s'\n",
        target, source);
        /* delete file and copy dir here */
        continue;
        }
        /* if target is dir and source is file, delete dir and copy file */
  	if((targetfile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
        sourcefile.dwFileAttributes & ~FILE_ATTRIBUTE_DIRECTORY))
        {
        lstrcatA(target, targetfile.cFileName);
        lstrcatA(source, sourcefile.cFileName);
        printf("Deleting directory '%s' and Copying file '%s'\n",
        target, source);	
        continue;
        }

    /*compare file times, update if last write is less */
    /* LARGE_INTEGER bigint; */
    /*
    i = CMP_LARGEINT(((LARGE_INTEGER)targetfile.ftLastWriteTime),
                     ((LARGE_INTEGER)sourcefile.ftLastWriteTime));
    */
    i = CMP_FILETIME(targetfile.ftLastWriteTime,
        sourcefile.ftLastWriteTime);
        if(i < 0) { 
        /* FileTimeToSystemTime(&targetfile.ftLastWriteTime, &time); */
        lstrcatA(target, targetfile.cFileName);
        printf("Updating file '%s' in target\n", target);
        }
    break;

    } while(FindNextFile(hfind_target, &targetfile));
    /* if not found, copy file from source to target*/ 
    if(!found)
    {
    lstrcatA(source, sourcefile.cFileName);
        if(sourcefile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
        printf("Copying directory '%s' to target\n", source);	
        }
        else
        {
        printf("Copying file '%s' to target\n", source);
        }
    }
    else found = 0;

/* restart target directory search */
FindClose(hfind_target);
hfind_target = FindFirstFile(target, &targetfile);
i = 2;
    while(i--) FindNextFile(hfind_target, &targetfile);
} while(FindNextFile(hfind_source, &sourcefile));

FindClose(hfind_source);
hfind_source = FindFirstFile(source, &sourcefile);
i = 2;
while(i--) FindNextFile(hfind_source, &sourcefile);

/* go through each file in target looking for files to delete */
do {
target[targetl] = '\0';
    do {
        /* if files are different, continue */
        if(0 != lstrcmpA(targetfile.cFileName, sourcefile.cFileName))
        continue;
    found = 1;
    break;
    } while(FindNextFile(hfind_source, &sourcefile));
    /* if not found, delete file */
    if(!found)
    {
    lstrcatA(target, targetfile.cFileName);
        if(targetfile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
        printf("Deleting directory %s from target\n", target);	
        }
        else
        {
        printf("Deleting file %s from target\n", target);
        }
    }
    else found = 0;
FindClose(hfind_source);
hfind_source = FindFirstFile(source, &sourcefile);
i = 2;
    while(i--) FindNextFile(hfind_source, &sourcefile);
} while(FindNextFile(hfind_target, &targetfile));

return 1;
}

int main(int argc, char **argv)
{
/* syncdir [options] targetdir sourcedir
** compare the targetdir with the sourcedir.
** if files have the same name, check the timestamp and
** copy from sourcedir to targetdir ONLY if the timestamp
** is greater in the soure. Copy any new files from sourcedir
** to targetdir and delete any missing ones. This program should
** have a flag to work recursively through the directories.
*/


int flags = 0;

if(argc < 3) {
printf("Display usage\n");
return E_PARAM_COUNT;
}

flags |= FLAG_RECURSIVE;
syncdir1(*(argv+1), *(argv+2), flags);

printf("0x%x - 0x%x\n", 0x12345678, FLIP_ENDIANNESS(0x0056));
printf("0x%x - 0x%x\n", 0x1234, FLIP_BYTES(0x1234));

return 0;
}
