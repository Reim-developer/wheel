#include <string_view>
#include <string>
#include <cstdint>
#include <cstring>
#include <vector>
#include <unordered_set>

#include <wheel_memory/allocator.hxx>
#include <__internal/__assert.hxx>

using wheel_memory::Arena;
using std::string_view;
using std::string;
using std::uint64_t;
using std::uintptr_t;
using std::memset;
using std::vector;
using std::size_t;
using std::unordered_set;
using std::to_string;

struct IdentifierNode {
    int         id;
    double      value;
    string_view name;

    IdentifierNode(int id, double value, string_view name) :
        id(id), value(value), name(name) {}
};

struct EmptyNode {
    int type_code;
    explicit EmptyNode(int code) : type_code(code) {}
};

struct alignas(64) CacheAlignedNode {
    uint64_t data[8];

    explicit CacheAlignedNode(uint64_t fill_value) {
        for (auto &element : data) {
            element = fill_value;
        }
    }
};

struct alignas(128) OverAlignedNode {
    char payload[128];

    explicit OverAlignedNode(char fill_character) {
        memset(payload, fill_character, sizeof(payload));
    }
};

TEST(test_basic_allocation)
    Arena  arena(1024);
    string sample_text = "sample_text";
    auto *node = arena.allocate<IdentifierNode>(
        42, 3.14, string_view(sample_text)
    );

    internal_assert(node != nullptr, "Allocation returned nullptr");
    internal_assert(node->id == 42, "id mismatch");
    internal_assert(node->name == "sample_text", "name mismatch");
    internal_assert(node->value == 3.14, "value mismatch");
DONE 

TEST(test_multiple_allocations)
    const int count = 100;
    Arena arena(1024 * 10);
    vector<IdentifierNode*> node_pointers;

    for (int index = 0; index < count; ++index) {
        static const char* variable_names[] = {
            "var_a", "var_b", "var_c", "temp"
        };

        auto *node = arena.allocate<IdentifierNode>(
            index, index * 1.5,
            string_view(variable_names[index % 4])
        );
        node_pointers.push_back(node);
    }

    internal_assert(
        static_cast<int>(node_pointers.size()) == count,
        "allocation count mismatch"
    );

    for (int index = 0; index < count; ++index) {
        internal_assert(
            node_pointers[index]->id == index,
            "node data corrupted at index: " << index
        );
    }

    for (size_t index = 1; index < node_pointers.size(); ++index) {
        auto *previous = reinterpret_cast<char*>(node_pointers[index - 1]);
        auto *current  = reinterpret_cast<char*>(node_pointers[index]);

        internal_assert(
            current >= previous + sizeof(IdentifierNode),
            "memory overlapped detected at index: " << index
        );
    }
    
DONE

TEST(test_mixed_types_allocation)
    Arena arena(2048);
    string sample_data = "mixed_type_test";

    auto *node1 = arena.allocate<IdentifierNode>(
        1, 1.0, 
        string_view(sample_data)
    );
    auto *node2 = arena.allocate<EmptyNode>(10);
    auto *node3 = arena.allocate<IdentifierNode>(
        2, 2.0, string_view(sample_data)
    );
    auto *node4 = arena.allocate<EmptyNode>(20);

    internal_assert(
        node1->id    == 1 && 
        node1->value == 1.0 &&
        node1->name  == "mixed_type_test",
        "node1 data corrupted"
    );
    internal_assert(
        node2->type_code == 10,
        "node2 data corrupted"
    );
    internal_assert(
        node3->id       == 2 &&
        node3->name     == "mixed_type_test" &&
        node3->value    == 2.0,
        "node3 data corrupted"
    );
    internal_assert(
        node4->type_code == 20,
        "node4 corrupted"
    );

    internal_assert(
        reinterpret_cast<uintptr_t>(node1) % alignof(IdentifierNode) == 0,
        "node1 misaligned"
    );
    internal_assert(
        reinterpret_cast<uintptr_t>(node2) % alignof(EmptyNode) == 0,
        "node2 misaligned"
    );
DONE 

TEST(test_reset_reuses_memory)
    Arena arena(1024);
    string sample_name = "reset_test";

    auto *node1 = arena.allocate<IdentifierNode>(
        1, 1.1, string_view(sample_name)
    );
    size_t offset_after_first = arena.used_size();
    internal_assert(
        offset_after_first > 0,
        "used_size should be > 0 after allocation"
    );
    arena.reset();
    internal_assert(arena.used_size() == 0, "arena did not reset to 0");

    auto *node2 = arena.allocate<IdentifierNode>(2, 2.2, string_view(sample_name));
    size_t offset_after_second = arena.used_size();

    internal_assert(
        offset_after_first == offset_after_second,
        "arena reuse failed (offset mismatch)"
    );
    internal_assert(
        reinterpret_cast<void*>(node1) == reinterpret_cast<void*>(node2),
        "arena reused wrong memory address"
    );
    internal_assert(node2->id    == 2, "new node data corrupted");
    internal_assert(node2->value == 2.2, "new node value corrupted");
DONE

TEST(test_reset_preservers_blocks)
    Arena arena(4096);

    for (int index = 0; index < 50; ++index) {
        auto _ = arena.allocate<EmptyNode>(index);
    }

    size_t blocks_before_reset = arena.block_count();
    size_t total_before_reset  = arena.total_allocated();

    internal_assert(blocks_before_reset == 1, "expected multipe blocks");
    arena.reset();

    internal_assert(
        arena.block_count() == blocks_before_reset, 
        "reset() should not destroy blocks"
    );
    internal_assert(
        arena.used_size() == 0,
        "used_size() should be 0 after reset"
    );

    for (int index = 0; index < 50; ++index) {
        auto _ = arena.allocate<EmptyNode>(index);
    }

    internal_assert(
        arena.block_count() == blocks_before_reset,
        "should reuse existing blocks after reset " <<
        "not create new ones"
    );
DONE 

TEST(test_fuzzing_alloction_patterns)
    Arena arena(2048);
    vector<void*> pointers;
    const int iterations = 1000;
    
    for (int index = 0; index < iterations; ++index) {
        int choice = index % 3;

        switch (choice) {
            case 0: {
                auto *ptr = arena.allocate<IdentifierNode>(index, index * 0.5, "fuzzing_test");
                pointers.push_back(ptr);
                internal_assert(ptr != nullptr, "allocation failed at iteration: " << index);

                break;
            }

            case 1: {
                auto *ptr = arena.allocate<EmptyNode>(index);
                pointers.push_back(ptr);
                internal_assert(ptr != nullptr, "allocation failed at iteration: " << index);

                break;
            }

            case 2: {
                auto *ptr = arena.allocate<int>(index);
                pointers.push_back(ptr);
                internal_assert(ptr != nullptr, "allocation failed at iteration: " << index);
            
                break;
            }
        }
    }

    unordered_set<void*> seen_address;

    for (size_t index = 0; index < pointers.size(); ++index) {
        if(!seen_address.insert(pointers[index]).second) {
            internal_assert(false,
                "duplicate allocation address detected at index: " <<
                to_string(index)
            );
        }
    }

    arena.reset();

    internal_assert(arena.used_size() == 0, "arena not properly reset");
    auto *fresh_node = arena.allocate<IdentifierNode>(999, 999.999, "fresh");

    internal_assert(fresh_node != nullptr, "post-reset allocation failed");
    internal_assert(
        fresh_node->id      == 999 &&
        fresh_node->name    == "fresh" &&
        fresh_node->value   == 999.999,
        "fresh allocation data corrupted"
    );
DONE 

TEST_MAIN
    RUN(test_basic_allocation)
    RUN(test_multiple_allocations)
    RUN(test_mixed_types_allocation)
    RUN(test_reset_reuses_memory)
    RUN(test_reset_preservers_blocks)
    RUN(test_fuzzing_alloction_patterns)
END_MAIN