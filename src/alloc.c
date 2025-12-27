#include <kernyx/kx.h>
#include <entry/macros.h>
#include <kernyx/alloc.h>

__visible bool INIT_ALLOC(alloc_t *alloc, void *(*_alloc)(usize), void (*_free)(void *)) {
    if (!_alloc || !_free)
        return false;

    alloc->alloc = _alloc;
    alloc->free = _free;
    INIT_REFCOUNT(&alloc->refcount, 1U);

    return true;
}

__visible alloc_t *alloc_get_ref(alloc_t *alloc) {
    if (refcount_acquire(&alloc->refcount))
        return alloc;

    return NULL;
}