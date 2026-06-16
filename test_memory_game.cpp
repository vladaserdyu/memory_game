#include <gtest/gtest.h>
#include "gameboard.h"
#include <vector>
#include <algorithm>

TEST(MinMovesTest, StandardSizes) {
    EXPECT_EQ(calcMinMoves(2), 2);
    EXPECT_EQ(calcMinMoves(4), 8);
    EXPECT_EQ(calcMinMoves(6), 18);
    EXPECT_EQ(calcMinMoves(8), 32);
}

TEST(MinMovesTest, InvalidSizes) {
    EXPECT_EQ(calcMinMoves(0), 0);
    EXPECT_EQ(calcMinMoves(1), 0);
}

TEST(MaxMovesTest, StandardSizes) {
    EXPECT_EQ(calcMaxMoves(2), 4);
    EXPECT_EQ(calcMaxMoves(4), 16);
    EXPECT_EQ(calcMaxMoves(6), 36);
    EXPECT_EQ(calcMaxMoves(8), 64);
}

TEST(MaxMovesTest, InvalidSizes) {
    EXPECT_EQ(calcMaxMoves(0), 0);
    EXPECT_EQ(calcMaxMoves(1), 1);
}

TEST(IsMatchTest, EqualValues) {
    EXPECT_TRUE(isMatch(5, 5));
    EXPECT_TRUE(isMatch(0, 0));
    EXPECT_TRUE(isMatch(99, 99));
}

TEST(IsMatchTest, DifferentValues) {
    EXPECT_FALSE(isMatch(1, 2));
    EXPECT_FALSE(isMatch(5, 3));
}

TEST(GenerateCardValuesTest, ValidSize4x4) {
    auto values = generateCardValues(4);
    EXPECT_EQ(values.size(), 16);

    std::sort(values.begin(), values.end());
    for (size_t i = 0; i < values.size(); i += 2) {
        EXPECT_EQ(values[i], values[i + 1]);
        if (i + 2 < values.size()) {
            EXPECT_NE(values[i], values[i + 2]);
        }
    }
}

TEST(GenerateCardValuesTest, InvalidSizes) {
    auto values = generateCardValues(0);
    EXPECT_EQ(values.size(), 0);

    values = generateCardValues(1);
    EXPECT_EQ(values.size(), 0);
}

void validateGridSize(int size) {
    if (size < 2 || size > 8) {
        throw std::runtime_error("Некорректный размер поля");
    }
    if (size % 2 != 0) {
        throw std::runtime_error("Размер поля должен быть чётным");
    }
}

void validateInput(int size) {
    if (size < 2 || size > 8) {
        throw std::runtime_error("Размер поля должен быть от 2 до 8");
    }
    if (size % 2 != 0) {
        throw std::runtime_error("Размер поля должен быть чётным");
    }
}

void loadImageWithCheck(const QString& path) {
    if (path.isEmpty()) {
        throw std::runtime_error("Не удалось загрузить картинку");
    }
}

TEST(ValidateGridSizeTest, ValidSizes) {
    EXPECT_NO_THROW(validateGridSize(2));
    EXPECT_NO_THROW(validateGridSize(4));
    EXPECT_NO_THROW(validateGridSize(6));
    EXPECT_NO_THROW(validateGridSize(8));
}

TEST(ValidateGridSizeTest, InvalidSizes) {
    EXPECT_THROW(validateGridSize(0), std::runtime_error);
    EXPECT_THROW(validateGridSize(1), std::runtime_error);
    EXPECT_THROW(validateGridSize(9), std::runtime_error);
    EXPECT_THROW(validateGridSize(-5), std::runtime_error);
    EXPECT_THROW(validateGridSize(3), std::runtime_error);
    EXPECT_THROW(validateGridSize(5), std::runtime_error);
    EXPECT_THROW(validateGridSize(7), std::runtime_error);
}

TEST(ValidateInputTest, ValidSizes) {
    EXPECT_NO_THROW(validateInput(2));
    EXPECT_NO_THROW(validateInput(4));
    EXPECT_NO_THROW(validateInput(6));
    EXPECT_NO_THROW(validateInput(8));
}

TEST(ValidateInputTest, InvalidSizes) {
    EXPECT_THROW(validateInput(0), std::runtime_error);
    EXPECT_THROW(validateInput(1), std::runtime_error);
    EXPECT_THROW(validateInput(9), std::runtime_error);
    EXPECT_THROW(validateInput(10), std::runtime_error);
    EXPECT_THROW(validateInput(3), std::runtime_error);
    EXPECT_THROW(validateInput(5), std::runtime_error);
    EXPECT_THROW(validateInput(7), std::runtime_error);
}

TEST(LoadImageTest, ValidPath) {
    EXPECT_NO_THROW(loadImageWithCheck("images/1.jpg"));
    EXPECT_NO_THROW(loadImageWithCheck("images/2.jpg"));
}

TEST(LoadImageTest, InvalidPath) {
    EXPECT_THROW(loadImageWithCheck(""), std::runtime_error);
}

TEST(CombinationTest, AllPairsMatch) {
    auto values = generateCardValues(4);
    for (size_t i = 0; i < values.size(); i += 2) {
        EXPECT_TRUE(isMatch(values[i], values[i + 1]));
    }
}

TEST(CombinationTest, DifferentPairsDontMatch) {
    auto values = generateCardValues(4);
    std::sort(values.begin(), values.end());
    for (size_t i = 0; i < values.size(); i += 2) {
        if (i + 2 < values.size()) {
            EXPECT_FALSE(isMatch(values[i], values[i + 2]));
        }
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
