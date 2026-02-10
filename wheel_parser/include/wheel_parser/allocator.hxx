#if !defined(ALLOCATOR_HXX)
#define ALLOCATOR_HXX

#include "config.hxx"
#include <vector>
#include <cstddef>
#include <utility>
#include <memory>
#include <deque>
#include <concepts>
#include <bit>

WHEEL_PARSER_NAMESPACE
    class Arena {
        private:
            struct Block {
                std::vector<char> storage;
                size_t offset = 0;

                explicit Block(std::size_t capacity) : storage(capacity), offset(0) {}

                template<typename T>
                T* try_allocate() {
                    constexpr std::size_t object_alignment = alignof(T);
                    constexpr std::size_t object_size      = sizeof(T);

                    char* base_pointer = storage.data() + offset;
                    std::size_t remaining_space = storage.size() - offset;
                    void *candidate = static_cast<void*>(base_pointer);

                    if(!std::align(object_alignment, object_size, candidate, remaining_space)) {
                        return nullptr;
                    }

                    std::size_t padding = static_cast<std::size_t>(
                        static_cast<char*>(candidate) - base_pointer
                    );
                    offset += padding + object_size;

                    return static_cast<T*>(candidate);
                }

                void reset_offset() {
                    offset = 0;
                }

                [[nodiscard]] std::size_t used() const {
                    return offset;
                }

                [[nodiscard]] std::size_t capacity() const {
                    return storage.size();
                }
            };

        private:
            std::deque<Block> m_blocks;
            Block* m_current_block;
            std::size_t m_reset_watermark = 0;
            std::size_t m_total_allocated;

        private:
            void append_block(std::size_t  capacity) {
                m_blocks.emplace_back(capacity);
                m_current_block = &m_blocks.back();
                m_total_allocated += capacity;
            }

        public:
            static constexpr std::size_t DEFAULT_BLOCK_CAPACITY = 4096;

            explicit Arena(std::size_t initial_capacity = DEFAULT_BLOCK_CAPACITY) :
                m_current_block(nullptr),
                m_total_allocated(0) {

                std::size_t actual_capacity = (initial_capacity < DEFAULT_BLOCK_CAPACITY) 
                    ? DEFAULT_BLOCK_CAPACITY : initial_capacity;

                append_block(actual_capacity);
            }
            ~Arena()            = default;
            Arena(const Arena&) = delete;
            Arena& operator = (const Arena&) = delete;

            Arena(Arena &&other) noexcept : 
                m_blocks(std::move(other.m_blocks)),
                m_current_block(other.m_current_block),
                m_reset_watermark(other.m_reset_watermark),
                m_total_allocated(other.m_total_allocated) {
            
                other.m_current_block   = nullptr;
                other.m_reset_watermark = 0;
                other.m_total_allocated = 0;
            }

            Arena &operator=(Arena &&other) noexcept {
                if(this != &other) {
                    m_blocks                = std::move(other.m_blocks);
                    m_current_block         = other.m_current_block;
                    m_reset_watermark       = other.m_reset_watermark;
                    m_total_allocated       = other.m_total_allocated;

                    other.m_current_block   = nullptr;
                    other.m_reset_watermark = 0;
                    other.m_total_allocated = 0;
                }

                return *this;
            }

            template<typename T, typename ...Args>
            requires std::constructible_from<T, Args ...>
            [[nodiscard]] T* allocate(Args&& ...arguments) {
                if(T* pointer = m_current_block->template try_allocate<T>()) {
                    [[likely]]
                    return ::new(pointer) T(std::forward<Args>(arguments)...);
                }

                return allocate_slow_path<T>(std::forward<Args>(arguments)...);
            }

            [[nodiscard]] std::size_t total_allocated() const {
                return m_total_allocated;
            }

            [[nodiscard]] std::size_t used_size() const {
                std::size_t total = 0;
                for (const auto &block: m_blocks) {
                    total += block.used();
                }

                return total;
            }

            [[nodiscard]] std::size_t block_count() const {
                return m_blocks.size();
            }

            void reset(){
                m_reset_watermark = m_blocks.size();
                for (auto &block : m_blocks) {
                    block.reset_offset();
                }

                m_current_block = &m_blocks.front();
            }

        private:
            auto current_block_iterator() {
                for (auto iterator = m_blocks.begin(); iterator != m_blocks.end(); ++iterator) {
                    if(&(*iterator) == m_current_block) {
                        return iterator;
                    }      
                }

                return m_blocks.end();
            }

            template<typename T>
            void grow_for() {
                constexpr std::size_t minimum_needed = alignof(T) + sizeof(T);
                std::size_t next_capacity = m_current_block->capacity() * 2;

                if(next_capacity < minimum_needed) {
                    next_capacity = std::bit_ceil(minimum_needed);
                }

                append_block(next_capacity);
            }

            template<typename T, typename ... Args>
            WHEEL_NO_INLINE T *allocate_slow_path(Args&& ...arguments) {
                auto current_iterator = current_block_iterator();
                auto next_iterator    = std::next(current_iterator);

                if (next_iterator != m_blocks.end()) {
                    if(T* pointer = next_iterator->template try_allocate<T>()) {
                        m_current_block = &(*next_iterator);

                        return ::new(pointer) T(std::forward<Args>(arguments)...);
                    }
                }

                grow_for<T>();
                T* pointer = m_current_block->template try_allocate<T>();
                return ::new (pointer) T(std::forward<Args>(arguments)...);
            }
    };
WHEEL_PARSER_END_NAMESPACE

#endif // ALLOCATOR_HXX