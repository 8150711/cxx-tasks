/* quated_string parser.
 * @file
 * @date 2018-08-18
 * @author Anonymous
 */

#ifndef __QUOTED_STRING_HPP__
#define __QUOTED_STRING_HPP__

#include <boost/spirit/home/x3.hpp>

namespace parser
{
    namespace x3 = boost::spirit::x3;

    //{ should be only one line
    const auto quoted_string = x3::lexeme['"' >> *(('\\' >> x3::char_) | (~x3::char_('"'))) >> '"'];
    //}
}

#endif // __QUOTED_STRING_HPP__
