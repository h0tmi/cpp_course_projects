#include <catch.hpp>
#include <palindrome.h>

TEST_CASE("IsPalindrome") {

    REQUIRE(IsPalindrome(""));
    REQUIRE(IsPalindrome("ab ba"));
    REQUIRE(IsPalindrome("ab b   a    "));
    REQUIRE(IsPalindrome(" a b b   a     "));
    REQUIRE(IsPalindrome("i"));
    REQUIRE(IsPalindrome("              i"));
    REQUIRE(IsPalindrome("i          "));
    REQUIRE(IsPalindrome("ii"));
    REQUIRE(IsPalindrome("ii"));
    REQUIRE(IsPalindrome("ada"));
    REQUIRE(IsPalindrome("i   "));
    REQUIRE(!IsPalindrome("i   j"));
    REQUIRE(!IsPalindrome("    i    i   j   "));
    REQUIRE(!IsPalindrome("a da  a"));
    REQUIRE(IsPalindrome("aa da  a"));
    REQUIRE(IsPalindrome("abba"));
    REQUIRE(IsPalindrome("a roza upala na lapu azora"));

    REQUIRE(!IsPalindrome("qwerty"));
}
