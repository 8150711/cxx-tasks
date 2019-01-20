/* CSV parser.
 * @file
 * @date 2018-08-15
 * @author Anonymous
 */

#ifndef __CSV_HPP__
#define __CSV_HPP__

#include <string>
#include <vector>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>

#include "parser.hpp"
#include "quoted_string.hpp"

/* An example of CSV:
 *      kind,of,header
 *      abc,with space,"quote"
 *      "comma , inside",132,  spaces  dot
 *      empty,,
 *
 * CSV (comma separated value) EBNF specification (http://www.rfc-editor.org/rfc/rfc4180.txt)
 *      string := [^,\n]+
 *      quoted_string := " [^"]* "
 *      cell := quated_string | string
 *      row := cell (, cell)* \n
 *      header := row
 *      csv := header row*
 */

namespace types
{
    namespace csv
    {
        //? Which kind of types should I use to describe the CSV type?
        //! Строку CSV можно представить в виде вектора из строк. Так как количество строк заранее не определено, то их тоже будем помещать в вектор. То есть сsv - vector<vector<string>>
        //{
        using csv = std::vector<std::vector<std::string>>;
        //}
    }
}

namespace parser
{
    namespace csv
    {
        //? Why I need a x3::no_skip here? Where is the original of the error?
        //! x3::no_skip превращает рассматриваемый текст в грамматический примитив (слово, число и т.д.). Это значит, что пробелы снаут составной частью рассматриваемого объекта, что нам и нужно https://www.boost.org/doc/libs/1_68_0/libs/spirit/doc/html/spirit/qi/reference/directive/lexeme.html
        //? Where is BOOST_SPIRIT_DEFINE? Is it necessary?
        //! BOOST_SPIRIT_DEFINE здесь не нужен. В документации сказано: "We have a rule and its definition, we tie the rule with a rule definition using the BOOST_SPIRIT_DEFINE macro". Так как нет ркурсивных зависимостей, то мы можем использовать оператор =
        namespace x3 = boost::spirit::x3;

        //{ csv grammar
        const auto string = x3::lexeme[+(~x3::char_(",\n"))];
        const auto cell = x3::rule<class row, std::string> {}
                 = parser::quoted_string | string;

        const auto row = x3::rule<class row, std::vector<std::string>> {}
                = cell % ',';

        const auto csv = x3::rule<class csv, types::csv::csv> {}
                = +(row);
        //}
    }
}

namespace literals
{
    namespace csv
    {
        //{ declare ``_csv`` literal
        types::csv::csv operator "" _csv(const char* str, size_t l)
        {
            return parser::load_from_string<types::csv::csv>(std::string(str, l), parser::csv::csv);
}
        //}
    }
}

#endif // __CSV_HPP__
