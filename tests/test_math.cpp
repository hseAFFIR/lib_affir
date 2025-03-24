#include <cxxtest/TestSuite.h>
#include "../src/math_utils.cpp"

class MathTest : public CxxTest::TestSuite {
public:
    void testAddition() {
        TS_ASSERT_EQUALS(add(2, 3), 5);
        TS_ASSERT_DIFFERS(add(0, 0), 1);
    }
};