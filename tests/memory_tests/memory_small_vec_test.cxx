#include <__internal/__assert.hxx>
#include <cstddef>
#include <wheel_memory/vec.hxx>
#include <wheel_memory/allocator.hxx>
#include <wheel_memory/config.hxx>

USE_WHEEL_SMALL_VEC

#if HAS_USE_SMALL_VEC
    using wheel_memory::Arena;
#endif 

#if defined (WHEEL_EXPERIMENT) && defined (WHEEL_SMALL_VEC)
    TEST(test_basic_operations)
        Arena arena;
        SmallVec<int> vec(&arena);
        
        assert_eq(vec.size(), 0);
        assert_eq(vec.capacity(), SmallVec<int>::STACK_CAPACITY);

        for (int index = 0; index < 25; ++index) {
            vec.push_back(index);
        }

        assert_eq(vec.size(), 25);
        for (int index = 0; index < 25; ++index) {
            assert_eq(vec.data()[index], index);
        }

        vec.clear();
        assert_eq(vec.size(), 0);
    DONE 


    TEST(test_stack_to_heap_transition)
        Arena arena;
        SmallVec<int> vec(&arena);

        for (std::size_t index = 0; index < SmallVec<int>::STACK_CAPACITY; ++index) {
            vec.push_back(static_cast<int>(index));
            assert_eq(vec.on_stack(), true);
        }

        vec.push_back(42);
        assert_eq(vec.on_stack(), false);

        for (std::size_t index = 0; index < SmallVec<int>::STACK_CAPACITY; ++index) {
            assert_eq(vec.data()[index], static_cast<int>(index));
        }
        assert_eq(vec.data()[SmallVec<int>::STACK_CAPACITY], 42);
        vec.clear();

        assert_eq(vec.size(), 0);
    DONE
#else 
    NO_TEST(test_basic_operations)
    NO_TEST(test_stack_to_heap_transition)
#endif 

TEST_MAIN 
    RUN(test_basic_operations)
    RUN(test_stack_to_heap_transition)
END_MAIN