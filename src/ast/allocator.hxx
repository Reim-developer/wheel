#ifndef ALLOCATOR_HXX
#define ALLOCATOR_HXX

#include <vector>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <stdexcept>
#include <new>

using std::vector;
using std::max_align_t;
using std::align;
using std::bad_alloc;
using std::forward;

namespace wheel::ast {
    class Arena {
        private:
            static size_t calculate_capacity(size_t requested) {
                const size_t min_align = sizeof(max_align_t);

                return (requested < min_align) ? min_align : requested;
            }
        
        public:
            explicit Arena (size_t capacity = 4096) : 
            m_buffer(calculate_capacity(capacity)), m_offset(0) {}

            Arena(const Arena&) = delete;
            Arena& operator     = (const Arena&) = delete;

        public:
            void reset() {
                m_offset = 0;
            }

            size_t used_size() const {
                return m_offset;
            }

            size_t total_size() const {
                return m_buffer.capacity();
            }

            template <typename T, typename ...Args>
            T *alloc(Args&& ...args) {
                size_t size         = sizeof(T);
                size_t aligned      = alignof(T);

                void *ptr           = static_cast<void*>(m_buffer.data() + m_offset);
                size_t space        = m_buffer.capacity() - m_offset;
                void *aligned_ptr   = align(aligned, size, ptr, space);

                if(aligned_ptr == nullptr || space < size) {
                    throw bad_alloc();
                }
                
                m_offset = static_cast<char*>(aligned_ptr) - m_buffer.data();

                T* object = static_cast<T*>(aligned_ptr);
                new (object) T(forward<Args>(args)...);

                m_offset += size;

                return object;
            }

        private:
            vector<char> m_buffer;
            size_t m_offset;
    };

} // namespace ast


#endif // ALLOCATOR_HXX