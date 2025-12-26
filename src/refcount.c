#include <kernyx/kx.h>
#include <kernyx/refcount.h>

__visible void INIT_REFCOUNT(REFCOUNT(*refcount), u32 n) {
    __atomic_store_n(&refcount->refs, n, __ATOMIC_RELAXED);
}

__visible bool refcount_acquire(REFCOUNT(*refcount)) {
    u32 old = __atomic_load_n(&refcount->refs, __ATOMIC_RELAXED);

    do {
        if (!old)
            return false;
    } while (!__atomic_compare_exchange_n(&refcount->refs, &old, old + 1U, false, __ATOMIC_RELEASE, __ATOMIC_RELAXED));

    return true;
}

__visible bool refcount_release(REFCOUNT(*refcount)) {
    u32 old = __atomic_fetch_sub(&refcount->refs, 1U, __ATOMIC_RELEASE);

    if (!old)
        __builtin_trap();

    if (old == 1U) {
        __atomic_thread_fence(__ATOMIC_ACQUIRE);
        return true;
    }

    return false;
}