/* Custom manipulators.
 * @file
 * @date 2018-08-05
 * @author Anonymous
 */

#ifndef __IOMANIP_HPP__
#define __IOMANIP_HPP__

#include <iostream>

//{ endm manipulator
std::ostream& endm(std::ostream& os)
{
    return os << "[eol]" << std::endl;
}
//}

//{ squares manipulator
struct squares_instance {} squares;

class squares_wrapper
{
public:
    squares_wrapper(std::ostream &os) : os_(os) {}

    template<class Input>
    std::ostream & operator<<(Input const &in)
    {
        return os_ << '[' << in << ']';
    }
private:
    std::ostream &os_;
};

squares_wrapper operator<<(std::ostream &os, squares_instance)
{
    return squares_wrapper(os);
}
//}

//{ add manipulator
struct add_instance {} add;

template<typename OpType>
class add_wrapper_op2
{
public:
    add_wrapper_op2(std::ostream &os, OpType const &op1) :  os_(os), op1_(op1) {}

    std::ostream & operator<<(OpType const &op2)
    {
        return os_ << op1_ + op2;
    }
private:
    OpType const &op1_;
    std::ostream &os_;
};

class add_wrapper_op1
{
public:
    add_wrapper_op1(std::ostream &os) : os_(os) {}

    template<typename OpType>
    add_wrapper_op2<OpType> operator<<(OpType const &op1)
    {
        return add_wrapper_op2<OpType>(os_, op1);
    }
private:
    std::ostream &os_;
};

add_wrapper_op1 operator<<(std::ostream &os, add_instance)
{
    return add_wrapper_op1(os);
}
//}

#endif // __IOMANIP_HPP__
