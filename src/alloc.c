#include <kernyx/kx.h>
#include <entry/macros.h>
#include <kernyx/alloc.h>

__visible bool INIT_ALLOC(ALLOC(*alloc), void *(*_alloc)(usize), void (*_free)(void *)) {
    if (!_alloc || !_free)
        return false;

    alloc->alloc = _alloc;
    alloc->free = _free;
    INIT_REFCOUNT(&alloc->refcount, 1U);

    return true;
}

__visible void *alloc_get_ref(ALLOC(*alloc)) {
    if (refcount_acquire(&alloc->refcount))
        return alloc;

    return NULL;
}