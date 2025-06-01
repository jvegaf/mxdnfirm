#include <gtest/gtest.h>
// Individual test files should be run separately to avoid conflicts
// Each test defines its own mocks and class redefinitions

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
