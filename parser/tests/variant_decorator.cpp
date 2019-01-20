/* Test file for variant_decorator.
 * @file
 * @date 2018-08-15
 * @author Anonymous
 */

#include <catch.hpp>
#include <string>
#include <variant_decorator.hpp>

namespace x3 = boost::spirit::x3;

TEST_CASE("variant_decorator::constructors")
{
    variant_decorator<float, int, bool> v{55};
    REQUIRE(boost::get<int>(v) == 55);

    SECTION("operator=")
    {
        v = true;
        CHECK(boost::get<bool>(v) == true);
    }

    SECTION("copy constructor")
    {
        //? Is it ``operator=`` or copy constructor?
        //! Stacktrace отладчика говорит, что здесь используется конструктор копирования
        auto v2 = v;
        CHECK(boost::get<int>(v2) == 55);
    }
}

TEST_CASE("variant_decorator::as")
{
    using namespace std::string_literals;

    variant_decorator<double, int, variant_decorator<std::string, bool>> v{45};
    CHECK(v.as<int>() == 45);

    //? Why shouldn't use just "text"?
    //! "text" это указатель на char (стандартный тип данных), поэтому "text" будет приведен к bool, так как приведение к стандартному типу более приоритетно
    v = variant_decorator<std::string, bool>{"text"s};
    auto& inner = v.as<variant_decorator<std::string, bool>>();
    CHECK(inner.as<std::string>() == "text");
}

TEST_CASE("variant_decorator::const")
{
    //? Why I need ``variant_decorator<bool>{true}`` instead of just ``true``? Will it compile? Why?
    //! true это блевская переменная (стандартный тип), поэтому онабудет приведена к int, так как приведение к стандартному типу более приоритетно
    variant_decorator<int, variant_decorator<bool>> v{variant_decorator<bool>{true}};
    //? How to avoid duplicates in ``as`` for const version?
    const auto& b = v.as<variant_decorator<bool>>();
    CHECK(b.as<bool>() == true);
}
