#if !defined(SYMBOL_HXX)
#define SYMBOL_HXX

#include <cstdint>
#include <deque>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include "wheel_parser/config.hxx"
#include "wheel_parser/allocator.hxx"

WHEEL_PARSER_AST_NAMESPACE
    using SymbolID = uint32_t;

    inline constexpr SymbolID INVALID_SYMBOL_ID = static_cast<SymbolID>(-1);

    struct SymbolHash {
        using is_transparent = void;

        [[nodiscard]] size_t operator()(std::string_view text) const noexcept {
            return std::hash<std::string_view>{}(text);
        }

        [[nodiscard]] size_t operator()(const std::string &text) const noexcept {
            return std::hash<std::string_view>{}(text);
        }
    };

    struct SymbolEqual {
        using is_transparent = void;

        [[nodiscard]] bool operator()(std::string_view lhs, std::string_view rhs) const noexcept {
            return lhs == rhs;
        }

        [[nodiscard]] bool operator()(const std::string &lhs, const std::string &rhs) const noexcept {
            return lhs == rhs;
        }

        [[nodiscard]] bool operator()(const std::string &lhs, std::string_view rhs) const noexcept {
            return lhs == rhs;
        }

        [[nodiscard]] bool operator()(std::string_view lhs, const std::string &rhs) const noexcept {
            return lhs == rhs;
        }
    };

    class StringInterner {
        private:
            std::deque<std::string> m_storage;
            std::unordered_map<std::string_view, SymbolID, SymbolHash, SymbolEqual> m_index;

        private:
            void rebuild_index() {
                m_index.clear();
                for (SymbolID id = 0; id < static_cast<SymbolID>(m_storage.size()); ++id) {
                    m_index.emplace(std::string_view(m_storage[id]), id);
                }
            }

        public:
            StringInterner()  = default;
            ~StringInterner() = default; 
            StringInterner(const StringInterner&) = delete;
            StringInterner& operator=(const StringInterner&) = delete;

            StringInterner(StringInterner&& other) noexcept :
                m_storage(std::move(other.m_storage)),
                m_index() {
                
                rebuild_index();
            }

            StringInterner &operator=(StringInterner&& other) noexcept {
                if (this != &other) {
                    m_storage = std::move(other.m_storage);
                    rebuild_index();
                }

                return *this;
            }

            [[nodiscard]] SymbolID intern(std::string_view text) {
                auto found = m_index.find(text);
                if (found != m_index.end()) {
                    return found->second;
                }

                const SymbolID symbol_id = static_cast<SymbolID>(m_storage.size());
                m_storage.emplace_back(text);
                m_index.emplace(std::string_view(m_storage.back()), symbol_id);

                return symbol_id;
            }

            [[nodiscard]] std::string_view resolve(SymbolID symbol_id) const noexcept {
                if (symbol_id >= m_storage.size()) {
                    return {};
                }

                return m_storage[symbol_id];
            }

            [[nodiscard]] size_t size() const noexcept {
                return m_storage.size();
            }
    };

WHEEL_PARSER_END_NAMESPACE

#endif // SYMBOL_HXX