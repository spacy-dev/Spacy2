#ifndef SBO_COW_FOOABLE_HH
#define SBO_COW_FOOABLE_HH

#include <array>
#include <atomic>
#include <cassert>
#include <cstddef>
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>


#if defined(_MSC_VER) && _MSC_VER == 1800
#define noexcept
#define alignof __alignof
#endif


namespace SBOCOW {
    
    class Fooable
    {
    public:
        Fooable () = default;
    
        /// Create type-erased interface from implementation.
        template <typename T,
                  typename std::enable_if<
                      !std::is_same< Fooable, typename std::decay<T>::type >::value
                      >::type* = nullptr>
        Fooable (T&& impl)
        {
            handle_ = clone_impl(std::forward<T>(impl), buffer_);
        }
    
        Fooable (const Fooable& rhs) :
            handle_ (
                !rhs.handle_ || heap_allocated(rhs.handle_, rhs.buffer_) ?
                rhs.handle_ :
                handle_ptr(
                    char_ptr(&buffer_) + handle_offset(rhs.handle_, rhs.buffer_)
                )
            ),
            buffer_ (rhs.buffer_)
        {
            if (handle_)
                handle_->add_ref();
        }
    
        Fooable (Fooable&& rhs) noexcept
        {
            swap(rhs.handle_, rhs.buffer_);
        }
    
        /// Assign from specific implementation.
        template <typename T,
                  typename std::enable_if<
                      !std::is_same< Fooable, typename std::decay<T>::type >::value
                      >::type* = nullptr>
        Fooable& operator= (T&& impl)
        {
            reset();
            handle_ = clone_impl(std::forward<T>(impl), buffer_);
            return *this;
        }
    
        Fooable& operator= (const Fooable& rhs)
        {
            Fooable temp(rhs);
            swap(temp.handle_, temp.buffer_);
            if (handle_)
                handle_->add_ref();
            return *this;
        }
    
        Fooable& operator= (Fooable&& rhs) noexcept
        {
            Fooable temp(std::move(rhs));
            swap(temp.handle_, temp.buffer_);
            return *this;
        }
    
        ~Fooable ()
        {
            reset();
        }
    
        /**
         * @brief Checks if the type-erased interface holds an implementation.
         * @return true if an implementation is stored, else false
         */
        explicit operator bool() const
        {
            return handle_ != nullptr;
        }
    
        /**
         * @brief Conversion of the stored implementation to @code T*@endcode.
         * @return pointer to stored object if conversion to @code T*@endcode
         *         was successful, else nullptr
         */
        template <typename T>
        T* target()
        {
            assert(handle_);
            void* buffer_ptr = get_buffer_ptr<typename std::decay<T>::type>(const_cast<Buffer&>(buffer_));
            if(buffer_ptr)
            {
                Handle<T,false>* handle = dynamic_cast<Handle<T,false>*>(handle_);
                if(handle)
                    return &handle->value_;
            }
            else
            {
                Handle<T,true>* handle = dynamic_cast<Handle<T,true>*>(handle_);
                if(handle)
                    return &handle->value_;
            }
    
            return nullptr;
        }
    
        /**
         * @brief Conversion of the stored implementation to @code const T*@endcode.
         * @return pointer to stored object if conversion to @code const T*@endcode
         *         was successful, else nullptr
         */
        template <typename T>
        const T* target() const
        {
            assert(handle_);
            void* buffer_ptr = get_buffer_ptr<typename std::decay<T>::type>(const_cast<Buffer&>(buffer_));
            if(buffer_ptr)
            {
                Handle<T,false>* handle = dynamic_cast<Handle<T,false>*>(handle_);
                if(handle)
                    return &handle->value_;
            }
            else
            {
                Handle<T,true>* handle = dynamic_cast<Handle<T,true>*>(handle_);
                if(handle)
                    return &handle->value_;
            }
    
            return nullptr;
        }
    
        int foo ( ) const
        {
                assert(handle_);
                return read().foo( );
        }
        void set_value ( int value )
        {
                assert(handle_);
                write().set_value(value );
        }
    
    private:
        using Buffer = std::array<char, 24>;
    
        struct HandleBase
        {
            virtual ~HandleBase () {}
            virtual HandleBase* clone_into (Buffer & buf) const = 0;
            virtual bool unique () const = 0;
            virtual void add_ref () = 0;
            virtual void destroy () = 0;
    
            virtual int foo ( ) const = 0;
            virtual void set_value ( int value ) = 0;
        };
    
        template <typename T, bool HeapAllocated>
        struct Handle : HandleBase
        {
            template <typename U,
                      typename std::enable_if<
                          !std::is_same< T, typename std::decay<U>::type >::value
                                               >::type* = nullptr>
            explicit Handle( U&& value ) noexcept :
                value_( value ),
                ref_count_(1)
            {}
    
            template <typename U,
                      typename std::enable_if<
                          std::is_same< T, typename std::decay<U>::type >::value
                                               >::type* = nullptr>
            explicit Handle( U&& value ) noexcept ( std::is_rvalue_reference<U>::value &&
                                                    std::is_nothrow_move_constructible<typename std::decay<U>::type>::value ) :
                value_( std::forward<U>(value) ),
                ref_count_(1)
            {}
    
            virtual HandleBase * clone_into (Buffer & buf) const
            { return clone_impl(value_, buf); }
    
            virtual bool unique () const
            { return ref_count_ == 1u; }
    
            virtual void add_ref ()
            { ++ref_count_; }
    
            virtual void destroy ()
            {
                if (ref_count_ == 1u) {
                    if (HeapAllocated)
                        delete this;
                    else
                        this->~Handle();
                } else {
                    --ref_count_;
                }
            }
    
            virtual int foo ( ) const {
                return value_.foo( );
            }
            virtual void set_value ( int value ) {
                value_.set_value(value );
            }
    
            T value_;
            std::atomic_size_t ref_count_;
        };
    
        template <typename T, bool HeapAllocated>
        struct Handle<std::reference_wrapper<T>, HeapAllocated> : Handle<T&, HeapAllocated>
        {
            Handle (std::reference_wrapper<T> ref) :
                Handle<T&, HeapAllocated> (ref.get())
            {}
        };
    
        template <typename T>
        static HandleBase * clone_impl (T&& value, Buffer& buffer)
        {
            using PlainType = typename std::decay<T>::type;
    
            void* buffer_ptr = get_buffer_ptr<PlainType>(buffer);
            if (buffer_ptr) {
                new (buffer_ptr) Handle<PlainType, false>(std::forward<T>(value));
                return static_cast<HandleBase*>(buffer_ptr);
            }
    
            return new Handle<PlainType, true>(std::forward<T>(value));
        }
    
        static bool heap_allocated (const HandleBase* handle, const Buffer& buffer)
        {
            return handle < handle_ptr(char_ptr(&buffer)) ||
                    handle_ptr(char_ptr(&buffer) + sizeof(buffer)) <= handle;
        }
    
        void swap (HandleBase*& rhs_handle, Buffer& rhs_buffer)
        {
            const bool this_heap_allocated = heap_allocated(handle_, buffer_);
            const bool rhs_heap_allocated = heap_allocated(rhs_handle, rhs_buffer);
    
            if (this_heap_allocated && rhs_heap_allocated) {
                std::swap(handle_, rhs_handle);
            } else if (this_heap_allocated) {
                const std::ptrdiff_t offset = handle_offset(rhs_handle, rhs_buffer);
                rhs_handle = handle_;
                buffer_ = rhs_buffer;
                handle_ = handle_ptr(char_ptr(&buffer_) + offset);
            } else if (rhs_heap_allocated) {
                const std::ptrdiff_t offset = handle_offset(handle_, buffer_);
                handle_ = rhs_handle;
                rhs_buffer = buffer_;
                rhs_handle = handle_ptr(char_ptr(&rhs_buffer) + offset);
            } else {
                const std::ptrdiff_t this_offset =
                    handle_offset(handle_, buffer_);
                const std::ptrdiff_t rhs_offset =
                    handle_offset(rhs_handle, rhs_buffer);
                std::swap(buffer_, rhs_buffer);
                handle_ = handle_ptr(char_ptr(&buffer_) + this_offset);
                rhs_handle = handle_ptr(char_ptr(&rhs_buffer) + rhs_offset);
            }
        }
    
        void reset()
        {
            if (handle_)
                handle_->destroy();
        }
    
        const HandleBase& read () const
        {
            return *handle_;
        }
    
        HandleBase & write ()
        {
            if (!handle_->unique())
                handle_ = handle_->clone_into(buffer_);
            return *handle_;
        }
    
        template <class T>
        static void* get_buffer_ptr(Buffer& buffer)
        {
            using BufferHandle  = Handle<T, false>;
            void * buf_ptr = &buffer;
            std::size_t buf_size = sizeof(buffer);
            const bool memcopyable = std::is_trivially_destructible<typename std::remove_cv<T>::type>();
            return memcopyable ? std::align( alignof(BufferHandle),
                                             sizeof(BufferHandle),
                                             buf_ptr, buf_size ) : nullptr;
        }
    
        template <typename T>
        static unsigned char* char_ptr (T* ptr)
        {
            return static_cast<unsigned char*>(
                static_cast<void*>(
                    const_cast<typename std::remove_const<T>::type*>(ptr)
                )
            );
        }
    
        static HandleBase * handle_ptr (unsigned char * ptr)
        { return static_cast<HandleBase *>(static_cast<void *>(ptr)); }
    
        static std::ptrdiff_t handle_offset (const HandleBase * handle,
                                             const Buffer & buffer)
        {
            assert(handle);
            unsigned char * const char_handle = char_ptr(handle);
            unsigned char * const char_buffer = char_ptr(&buffer);
            return char_handle - char_buffer;
        }
    
        HandleBase* handle_ = nullptr;
        Buffer buffer_;
    };

}

#endif

