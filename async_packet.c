/* proc types:
 * GUI APP
 * TERMINAL APP 
 * SERVICE
*/
struct procdef {
uint32 proc_type; /* bit field describing the type of process */
};

PRIVATE void *create_async_packet_pool
(
struct async_packet_pool *pool,
void *mem,
uint32 mem_size
)
/*
 *
*/
{
struct async_packet *ap;
uint16 count;

pool->size = mem_size;
pool->head = pool->next = mem;
count = (mem_size / sizeof(*ap)) - 1;
InitializeCriticalSection(&pool->crit_sect);
ap = pool->next;
    while(count--)
    {
    InitializeCriticalSection(&ap->crit_sect);
    ap->next = (struct async_packet*)((int)ap + sizeof(*ap));    
    ap = ap->next;
    }
InitializeCriticalSection(&ap->crit_sect);
ap->next = 0;
return 0;
}

#define pop_async_packet pop_async_packet1
#define push_async_packet push_async_packet1

//#if 1
PUBLIC struct async_packet *
pop_async_packet1(void *handler, uint32 mode, uint32 op_type)
{
struct async_packet *ap;
assert(process.async_packet_pool.next);

EnterCriticalSection(&process.async_packet_pool.crit_sect);
ap = process.async_packet_pool.next;
process.async_packet_pool.next = ((struct async_packet*)
  process.async_packet_pool.next)->next;
LeaveCriticalSection(&process.async_packet_pool.crit_sect);
ap->handler.fn = handler;
ap->handler.thread_mode = mode;
ap->object.op_type = op_type;
ap->object.ovl_s.hEvent = 0;
return ap;
}
//#else
#define pop_async_packet2(_ap, _Handler, _ThreadMode, _OpType) \
    if(!process.async_packet_pool.next) { \
        printf(" Error: No asynchronous objects available\n"); \
    } \
    else { \
        EnterCriticalSection(&process.async_packet_pool.crit_sect); \
            _ap = process.async_packet_pool.next; \
            process.async_packet_pool.next = \
            ((struct async_packet*)process.async_packet_pool.next)->next; \
        LeaveCriticalSection(&process.async_packet_pool.crit_sect); \
        _ap->handler.fn = _Handler; \
        _ap->handler.thread_mode = _ThreadMode; \
        _ap->object.op_type = _OpType; \
        _ap->object.ovl_s.hEvent = 0; \
    } \
//#endif

//#if 1
PUBLIC void
push_async_packet1(struct async_packet *ap)
{
EnterCriticalSection(&process.async_packet_pool.crit_sect);
ap->next = process.async_packet_pool.next;
process.async_packet_pool.next = ap;
LeaveCriticalSection(&process.async_packet_pool.crit_sect);
}
//#else
#define push_async_packet1(_ap) \
EnterCriticalSection(&process.async_packet_pool.crit_sect); \
_ap->next = process.async_packet_pool.next; \
process.async_packet_pool.next = _ap; \
LeaveCriticalSection(&process.async_packet_pool.crit_sect);
//#endif
