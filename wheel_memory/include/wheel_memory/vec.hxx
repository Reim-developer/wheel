#if !defined (VEC_HXX)
#define VEC_HXX

#if defined(WHEEL_EXPERIMENT) && defined(WHEEL_SMALL_VEC)
#include <utility>
#include <memory>
#include "config.hxx"
#include "allocator.hxx"
#include <cstddef>
#include <type_traits>

WHEEL_MEMORY_NAMESPACE
    using wheel_memory::Arena;

    template <typename T>
    class SmallVec {
        private:
            Arena                  *m_arena;
            T                      *m_data;
            size_t                  m_size;
            size_t                  m_capacity;
            alignas(T) std::byte    m_stack_buffer[4096];

        private:
            inline void grow() noexcept {
                size_t new_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;
                T* new_data = m_arena->allocate_array<T>(new_capacity);
                
                for (size_t index = 0; index < m_size; ++index) {
                    ::new (&new_data[index]) T(std::move_if_noexcept(m_data[index]));

                    if constexpr (!std::is_trivially_destructible_v<T>) {
                        m_data[index].~T();
                    }
                }

                m_data              = new_data;
                m_capacity          = new_capacity;
            }

        public:
            static constexpr size_t STACK_CAPACITY = 4096 / sizeof(T);

            explicit SmallVec(Arena *arena) noexcept :
                m_arena(arena),
                m_data(reinterpret_cast<T*>(m_stack_buffer)),
                m_size(0),
                m_capacity(STACK_CAPACITY) {}

            explicit SmallVec(SmallVec&& other) noexcept :
                m_arena(std::exchange(other.m_arena, nullptr)),
                m_data(nullptr),
                m_size(std::exchange(other.m_size, 0)),
                m_capacity(0) {

                if (other.m_data == reinterpret_cast<T*>(other.m_stack_buffer)) {
                    m_data      = reinterpret_cast<T*>(m_stack_buffer);
                    m_capacity  = STACK_CAPACITY;

                    for (std::size_t index = 0; index < m_size; ++index) {
                        ::new(&m_data[index]) T(std::move(other.m_data[index]));

                        if constexpr (!std::is_trivially_destructible_v<T>) {
                            other.m_data[index].~T();
                        }
                    }

                    other.m_data     = reinterpret_cast<T*>(other.m_stack_buffer);
                    other.m_capacity = STACK_CAPACITY;
                } else {
                    m_data      = std::exchange(other.m_data, reinterpret_cast<T*>(other.m_stack_buffer));
                    m_capacity  = std::exchange(other.m_capacity, STACK_CAPACITY);
                }
            }

            SmallVec& operator=(SmallVec&& other) noexcept {
                if (this != &other) {
                    clear();

                    m_arena = std::exchange(other.m_arena, nullptr);
                    m_size  = std::exchange(other.m_size, 0);

                    if (other.m_data == reinterpret_cast<T*>(other.m_stack_buffer)) {
                        m_data     = reinterpret_cast<T*>(m_stack_buffer);
                        m_capacity = STACK_CAPACITY;

                        for (size_t index = 0; index < m_size; ++index) {
                            ::new(&m_data[index]) T(std::move(other.m_data[index]));

                            if constexpr (!std::is_trivially_destructible_v<T>) {
                                other.m_data[index].~T();
                            }
                        }

                        other.m_data       = reinterpret_cast<T*>(other.m_stack_buffer);
                        other.m_capacity   = STACK_CAPACITY;
                    } else {
                        m_data      = std::exchange(other.m_data, reinterpret_cast<T*>(other.m_stack_buffer));
                        m_capacity  = std::exchange(other.m_capacity, STACK_CAPACITY);
                    }
                }

                return *this;
            }

            ~SmallVec() {
                if constexpr (!std::is_trivially_destructible_v<T>) {
                    std::destroy_n(m_data, m_size);
                }
            }

            SmallVec(const SmallVec&)            = delete;
            SmallVec& operator=(const SmallVec&)& = delete;

            inline constexpr size_t size() const noexcept {
                return m_size;
            }

            inline constexpr T* data() const noexcept {
                return m_data;
            }

            inline constexpr void push_back(const T& value) noexcept {
                if (m_size >= m_capacity) {
                    grow();
                }

                ::new(&m_data[m_size]) T(value);
                ++m_size;
            }

            inline constexpr void push_back(T&& value) {
                if (m_size >= m_capacity) {
                    grow();
                }

                ::new (&m_data[m_size]) T(std::move(value));
                ++m_size;
            }

            inline constexpr void clear() noexcept {
                if constexpr (!std::is_trivially_destructible_v<T>) {
                    std::destroy_n(m_data, m_size);
                }

                m_size = 0;
            }

            inline constexpr std::size_t capacity() const noexcept {
                return m_capacity;
            }

            inline constexpr bool on_stack() noexcept {
                return m_data == reinterpret_cast<T*>(m_stack_buffer);
            }
    };

WHEEL_MEMORY_END_NAMESPACE

#endif 


#endif // VEC_HXX