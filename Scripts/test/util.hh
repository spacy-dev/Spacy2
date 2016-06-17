
#define CHECK_HEAP_ALLOC(expression, expected_allocations) \
    reset_heap_allocations(); \
    expression; \
    { \
      auto n_heap_allocations = heap_allocations(); \
      EXPECT_EQ( n_heap_allocations, expected_allocations ); \
    }


inline std::size_t& heap_allocations ()
{
    static std::size_t allocations_ = 0;
    return allocations_;
}

inline void reset_heap_allocations ()
{
    heap_allocations() = 0;
}

inline void* operator new (std::size_t size)
{
    ++heap_allocations();
    return malloc(size);
}

inline void* operator new[] (std::size_t size)
{
    ++heap_allocations();
    return malloc(size);
}

inline void operator delete (void * ptr)
{
    free(ptr);
}

inline void operator delete[] (void * ptr)
{
    free(ptr);
}
