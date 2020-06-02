PUBLIC void *mem_alloc(uint32 size);
PUBLIC void mem_free(void *mem);

PRIVATE uint32
align_memory_pool(int page_size, int arena_size, int arena_count)
{
uint32 result = arena_size*arena_count;
    do
    {
        if(result > arena_size){
            if(result%arena_size == 0)
                return result/arena_size;
            else if(arena_size%result == 0)
                return 1;
        }
        return 0;
    } while (0);

}

#define GetPercentage(Total, Percentage, Number) \
    Number = (Total * Percentage) / 100; \
    if(((Total * Percentage) % 100) >= 0) Number++;

PUBLIC uint32 create_memory_pool
(uint16 granularity,
 uint16 scale,
 void   **pbase,
 void  *ratios,
 void *pool,
 uint32 num)
{
int i;
void *base = *pbase;
struct memory_pool *pool = 0;
struct memory_pool_arena *arena_addr = 0;
DWORD total_size = 0,
      page_count = 0,
      pool_index = 8,
      arena_size = 0,
      arena_count = 0;

    base = VirtualAlloc(0, granularity * scale, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

    if(!base){
        printf("Error %d: Could not get memory\n", GetLastError());    
        return 0;
    }

    for(i = 0; i < 8; i++){
        total_size += ratios[i];    
    }

    if(total_size != 16) {
        printf(" Error: Schema ratio must equal 16\n");
        return 0;    
    }

    arena_addr = base;
    arena_size = 0x1000;
    for(i = 0; i < 8; i++){
        process.mem[i].arena_size = arena_size; 
        if(ratios[i] <= 0){
            process.mem[i].next = 0;
            process.mem[i].arena_count = 0;
            continue;
        }
        InitializeCriticalSection(&process.mem[i].crit_sect);
        process.mem[i].arena_count = arena_count =
        (((ratios[i] * 0x1000) * scale) / arena_size);
        process.mem[i].next = arena_addr;
        arena_count--;
        while(arena_count--){
            arena_addr->next = ((void*)((int)arena_addr+arena_size));
            arena_addr = arena_addr->next;
        }
        arena_addr->next = 0;
        arena_addr = (void*)((int)arena_addr+arena_size);
        arena_size>>= 1;
    }
}

PUBLIC void *
mem_alloc(uint32 size)
{
#if 1
return malloc(size);
#else if 0
int arena_size = 0;
int pool_index = 8;
void *arena = 0;

while(pool_index--)
{
if(arena_size && size <= arena_size &&
size > process.mem[pool_index].arena_size)
	{
	if(!process.mem[pool_index].next) pool_index++;
	EnterCriticalSection(&process.mem[++pool_index].crit_sect);
	arena = process.mem[pool_index].next;
	process.mem[pool_index].next =
	((struct memory_pool_arena*)process.mem[pool_index].next)->next;
	LeaveCriticalSection(&process.mem[pool_index].crit_sect);
	break;
	}
arena_size = process.mem[pool_index].arena_size;
}
return arena;
#endif
}

PUBLIC void
mem_free(void *mem)
{
#if 1
free(mem);
#else if 0

#endif
}

/*
int main (int argc, char** argv)
{
    InitProcess();
    struct MemoryPool* MemoryPool = 0;
    //printf(" Size of OVERLAPPED_ENTRY %d\n", sizeof(OVERLAPPED_ENTRY));
    printf(" Size of WSANAMESPACE_INFO %d\n", sizeof(WSANAMESPACE_INFO));

    Sleep(1);
    return 0;
}
*/
