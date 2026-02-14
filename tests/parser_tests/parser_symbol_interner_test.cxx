#include <string>
#include <format>
#include <utility>
#include <wheel_parser/ast/symbol.hxx>
#include <__internal/__assert.hxx>

using std::string;
using std::format;
using wheel_parser::ast::StringInterner;
using wheel_parser::ast::INVALID_SYMBOL_ID;

TEST(test_symbol_interner_deduplicates_strings)
    StringInterner string_interner;
    auto id1 = string_interner.intern("alpha");
    auto id2 = string_interner.intern("alpha");
    auto id3 = string_interner.intern("beta");

    internal_assert(id1 == id2, "expected same symbol id for duplicate string");
    internal_assert(id1 != id3, "expected distinct ids for distinct strings");
    internal_assert(string_interner.resolve(id1) == "alpha", "expected alpha resolve");
    internal_assert(string_interner.resolve(id3) == "beta", "expected beta resolve");
DONE

TEST(test_symbol_interner_invalid_resolve_type)
    StringInterner interner;

    const auto root_id = interner.intern("root");
    internal_assert(interner.resolve(root_id) == "root", "expected root resolve");
    internal_assert(interner.resolve(INVALID_SYMBOL_ID).empty(), "invalid symbol id should resolve empty");
    internal_assert(interner.resolve(1000).empty(), "out-of-range symbol id should resolve empty");
DONE

TEST(test_symbol_interner_move_edge)
    StringInterner source;
    auto id1 = source.intern("alpha");
    auto id2 = source.intern("beta");
    
    StringInterner moved(std::move(source));
    internal_assert(moved.resolve(id1) == "alpha", "moved interner should preserve alpha");
    internal_assert(moved.resolve(id2) == "beta", "moved interner should preserve beta");
    internal_assert(moved.intern("alpha") == id1, "moved interner should preserve dedup index");
DONE 

TEST(test_symbol_interner_fuzz_many_identifier)
    StringInterner interner;

    for (int index = 0; index < 5000; ++index) {
        const string symbol = format("symbol_{}", index);
        auto id = interner.intern(symbol);

        internal_assert(interner.resolve(id) == symbol, "resoved symbol mismatch");
    }

    for (int index = 0; index < 5000; ++index) {
        const string symbol = std::format("symbol_{}", index);

        auto id1 = interner.intern(symbol);
        auto id2 = interner.intern(symbol);

        internal_assert(id1 == id2, "duplicate symbol must keep stable id");
    }

    internal_assert(interner.size() == 5000, "expected unique symbol count");
DONE 

TEST_MAIN
    RUN(test_symbol_interner_deduplicates_strings)
    RUN(test_symbol_interner_invalid_resolve_type)
    RUN(test_symbol_interner_move_edge)
    RUN(test_symbol_interner_fuzz_many_identifier)
END_MAIN