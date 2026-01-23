#include <cassert>
#include <string>
#include <utility>
#include <iostream>
#include <random>
#include <ast/allocator.hxx>

using wheel::ast::Arena;
using std::string;
using std::string_view;
using std::move;
using std::cout;
using std::vector;

struct IdentifierNode {
    int id;
    double value;
    string_view name;

    IdentifierNode(int id, double value, string_view name) : 
        id(id), value(value), name(move(name)) {}
};

struct EmptyNode {
    int type_code;
    explicit  EmptyNode(int code) : type_code(code) {}
};

void test_string_view_ast() {
    cout << "[TEST] 'test_string_view_ast'\n";

    string sample_text   = "sample_text";
    int    sample_int    = rand();
    double sample_double = static_cast<double>(rand());
    Arena arena(1024);

    auto *node = arena.alloc<IdentifierNode>(
        sample_int, sample_double, string_view(sample_text)
    );

    assert(node->name  == sample_text && "<string_view> value is not allowed");
    assert(node->id    == sample_int && "<integer> value is not allowed");
    assert(node->value == sample_double && "<double> value is not allowed");

    cout << "<node->name>: " << node->name << "\n";
    cout << "<node->id>: " << node->id << "\n";
    cout << "<node->value>: " << node->value << "\n";
}

void test_allocation_array() {
    cout << "[TEST] 'test_allocation_array'\n";
    const int number = 100;
    Arena arena(1024 * 10);

    vector<IdentifierNode*> node_pointers;

    for(int index = 0;index < number; ++index) {
        static const char* var_names[] = {"var_a", "var_b", "var_c", "temp"};

        auto *node = arena.alloc<IdentifierNode>(
            index, index * 1.5, string_view(var_names[index % 4])
        );

        node_pointers.push_back(node);
    }

    assert(node_pointers.size() == number && "Allocation count mismatch");

    for (int index = 0; index < number; ++index) {
        assert(node_pointers[index]->id == index && "Note data corrupted");
    }

    for(size_t index = 1; index < node_pointers.size(); ++index) {
        auto *prev    = node_pointers[index - 1];
        auto *current = node_pointers[index];
        
        assert(reinterpret_cast<char*>(current) > reinterpret_cast<char*>(prev) && "Memory overlap detected");
    }

    cout << "[TEST] allocated and verified " << number << " nodes successfully\n";
}

void test_reset_and_pause() {
    cout << "[TEST] 'test_reset_and_pause'\n";

    Arena arena(1024);
    string sample_var_name = "sample_var_name";

    auto *node1 = arena.alloc<IdentifierNode>(1, 1.1, string_view(sample_var_name));
    size_t offset_after_first = arena.used_size();

    assert(offset_after_first > 0);
    arena.reset();
    assert(arena.used_size() == 0 && "Arena did not reset to 0");

    auto *node2 = arena.alloc<IdentifierNode>(2, 2.2, string_view(sample_var_name));
    size_t offset_after_second = arena.used_size();

    auto address1 = reinterpret_cast<void*>(node1);
    auto address2 = reinterpret_cast<void*>(node2);
    assert(offset_after_first == offset_after_second && "Arena reuse failed (offset mismatch)");
    assert(address1           == address2 && "Arena reused wrong memory address");
    assert(node2->id          == 2 && "New <node2> data <id> corrupted by <node1>");
    assert(node2->value       == 2.2 && "New <node2> data <value> corrupted by <node1>");

    cout << "<offset_after_first>: " << offset_after_first << "\n";
    cout << "<offset_after_second>: " << offset_after_second << "\n";
    cout << "<address1>: " << address1 << "\n";
    cout << "<address2>: " << address2 << "\n";
    cout << "<node2->id>: " << node2->id << "\n";
    cout << "<node2->value>: " << node2->value << "\n";
}

void test_mixed_types_alloc() {
    cout << "[TEST] 'test_mixed_types_alloc'\n";

    Arena arena(2048);
    string sample_data = "this is sample text data";

    auto *node1 = arena.alloc<IdentifierNode>(1, 1.0, string_view(sample_data));
    auto *node2 = arena.alloc<EmptyNode>(10);
    auto *node3 = arena.alloc<IdentifierNode>(2, 2.0, string_view(sample_data));
    auto *node4 = arena.alloc<EmptyNode>(20);

    assert(node1->id == 1 && node2->type_code == 10);
    assert(node3->id == 2 && node4->type_code == 20);
    assert(node1->value == 1.0 && node3->value == 2.0);

    cout << "<node1->id>: " << node1->id << "\n";
    cout << "<node1->value>: " << node1->value << "\n";
    cout << "<node1->name>: " << node1->name << "\n";

    cout << "<node3->id>: " << node3->id << "\n";
    cout << "<node3->value>: " << node3->value << "\n";
    cout << "<node3->name>: " << node3->name << "\n";
    
    cout << "<node2->type_code>: " << node2->type_code << "\n";
    cout << "<node4->type_code>: " << node4->type_code << "\n"; 
}

int main() {
    test_string_view_ast();
    test_allocation_array();
    test_reset_and_pause();
    test_mixed_types_alloc();
}