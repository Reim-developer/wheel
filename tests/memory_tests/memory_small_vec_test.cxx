#include <__internal/__assert.hxx>
#include <wheel_memory/vec.hxx>
#include <wheel_memory/allocator.hxx>
#include <wheel_memory/config.hxx>

USE_WHEEL_SMALL_VEC

#if HAS_USE_SMALL_VEC
    #include <cstddef>
    using wheel_memory::Arena;
#endif 

#if defined (WHEEL_EXPERIMENT) && defined (WHEEL_SMALL_VEC)
    struct TestObject {
        static int alive_count;
        static int move_count;
        static int copy_count;

        int value;

        TestObject(int value = 0) : value(value) {
            ++alive_count;
        }

        ~TestObject() {
            --alive_count;
        }

        TestObject(const TestObject &other) : value(other.value) {
            ++copy_count;
            ++alive_count;
        }

        TestObject(TestObject &&other) noexcept : value(other.value) {
            ++move_count;
            ++alive_count;
            other.value = - 1;
        }

        TestObject& operator=(const TestObject& other) {
            value = other.value;
            ++copy_count;

            return *this;
        }

        TestObject& operator=(TestObject&& other) noexcept {
            value = other.value;
            other.value = -1;
            ++move_count;

            return *this;
        }

        static void reset() {
            TestObject::alive_count = 0;
            TestObject::copy_count  = 0;
            TestObject::move_count  = 0;
        }
    };

    int TestObject::alive_count = 0;
    int TestObject::move_count  = 0;
    int TestObject::copy_count  = 0;

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

    TEST(test_complex_types)
        TestObject::reset();

        Arena arena;
        SmallVec<TestObject> vec(&arena);

        TestObject object(100);

        vec.push_back(object);
        assert_eq(TestObject::copy_count, 1);
        assert_eq(TestObject::move_count, 0);
        assert_eq(TestObject::alive_count, 2);

        assert_eq(vec.data()[0].value, 100);

        vec.clear();
        assert_eq(TestObject::alive_count, 1);
    DONE 

    TEST(test_move_constructor)
        Arena arena;

        {
            SmallVec<int> vec1(&arena);
            for (int index = 0; index < 10; ++index) {
                vec1.push_back(index);
            }

            assert_eq(vec1.on_stack(), true);
            SmallVec<int> vec2(std::move(vec1));

            assert_eq(vec1.size(), 0);
            assert_eq(vec2.size(), 10);
            assert_eq(vec2.on_stack(), true);

            for (int index = 0; index < 10; ++index) {
                assert_eq(vec2.data()[index], index);
            }
        };

        {
            SmallVec<int> vec1(&arena);
            
            for (std::size_t index = 0; index < SmallVec<int>::STACK_CAPACITY + 10; ++index) {
                vec1.push_back(index);
            }

            assert_eq(vec1.on_stack(), false);
            SmallVec<int> vec2(std::move(vec1));

            assert_eq(vec1.size(), 0);
            assert_eq(vec1.on_stack(), true);

            assert_eq(vec2.size(), SmallVec<int>::STACK_CAPACITY + 10);
            assert_eq(vec2.on_stack(), false);

            for (std::size_t index = 0; index < SmallVec<int>::STACK_CAPACITY + 10; ++index) {
                assert_eq(vec2.data()[index], static_cast<int>(index));
            }
        };

        {
            TestObject::reset();
            SmallVec<TestObject> vec(&arena);

            vec.push_back(TestObject(42));
            vec.push_back(TestObject(32));

            SmallVec<TestObject> vec2(std::move(vec));

            int alive_count = TestObject::alive_count;
            assert_eq(TestObject::alive_count, alive_count);

            assert_eq(vec.size(), 0);
            assert_eq(vec2.size(), 2);
        }
    DONE 

    TEST(test_self_assigment)
        Arena arena;
        SmallVec<int> vec(&arena);

        for (int index = 0; index < 10; ++index) {
            vec.push_back(index);
        }

        vec = std::move(vec);
        assert_eq(vec.size(), 10);

        for (int index = 0; index < 10; ++index) {
            assert_eq(vec.data()[index], index);
        }
    DONE 

    TEST(test_vec_of_vecs)
        Arena arena;

        SmallVec<SmallVec<int>> vec(&arena);

        vec.push_back(SmallVec<int>(&arena));
        vec.data()[0].push_back(1);
    
        assert_eq(vec.data()[0].data()[0], 1);
    DONE 
#else 
    NO_TEST(test_basic_operations)
    NO_TEST(test_stack_to_heap_transition)
    NO_TEST(test_complex_types)
    NO_TEST(test_move_constructor)
    NO_TEST(test_self_assigment)
    NO_TEST(test_vec_of_vecs)
#endif 

TEST_MAIN
    RUN(test_basic_operations)
    RUN(test_stack_to_heap_transition)
    RUN(test_complex_types)
    RUN(test_move_constructor)
    RUN(test_self_assigment)
    RUN(test_vec_of_vecs)
END_MAIN
