#include <__internal/__assert.hxx>
#include <wheel_utils/logging.hxx>

#if defined (WHEEL_EXPERIMENT)
    using wheel_utils::is_pointer_value;
    using wheel_utils::not_null;
#endif

#if defined (WHEEL_EXPERIMENT)

    TEST(test_is_pointer_value)
        assert_eq(is_pointer_value<int>, false);
        assert_eq(is_pointer_value<int*>, true);
        assert_eq(is_pointer_value<void*>, true);
    DONE 

    #if defined (WHEEL_NO_FAIL_FAST)
        TEST(test_no_fail_fast)
            assert_eq(not_null<void*>(nullptr), false);
            
            int var     = 10;
            int *var_p  = &var;
            assert_eq(not_null<int*>(var_p), true);
        DONE 

    #else 
        NO_TEST(test_no_fail_fast)
    #endif

#else 
    NO_TEST(test_is_pointer_value)
    NO_TEST(test_no_fail_fast)
#endif 

TEST_MAIN
    RUN(test_is_pointer_value)
    RUN(test_no_fail_fast)
END_MAIN