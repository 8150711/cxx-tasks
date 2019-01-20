/* Image iterator.
 * @file
 * @date 2018-08-05
 * @author Anonymous
 */

#ifndef __ITERATOR_HPP__
#define __ITERATOR_HPP__

#include <algorithm>
#include <type_traits>
#include <vector>
#include <utility>

#include <boost/iterator/iterator_adaptor.hpp>

//{ image iterator
template<class Iterator>
class image_iterator: public boost::iterator_adaptor<image_iterator<Iterator>, Iterator>
{
public:
    explicit image_iterator(Iterator it, size_t width, size_t stride):
        image_iterator::iterator_adaptor_(it), 
        width_(width), 
        stride_(stride)
        {}

private:
    size_t width_;
    size_t stride_;
    size_t ind_ = 0;

    friend class boost::iterator_core_access;

    void increment()
    {
        if (ind_ % stride_ == 0)
        {
            ind_ += stride_ - width_ + 1;
            this->base_reference() += stride_ - width_ + 1;
        }
        else
        {
            ind_++;
            this->base_reference()++;
        }
    }

    void decrement()
    {
        if (width_- ind_ % (int)stride_ == 0)
        {
            ind_ += width_- ind_ % (int)stride_ + 1;
            this->base_reference() -= width_- ind_ % (int)stride_ + 1;
        }
        else
        {
            ind_--;
            this->base_reference()--;
        }
    }


    void advance( typename image_iterator::difference_type n )
    {
        int length = n;
        int shift = length > 0 ? (ind_ % (int)stride_): -(width_- ind_ % (int)stride_);

        length += (shift + length) / (int)width_ * (stride_ - width_);

        this->base_reference() += length;
        ind_ += length;
    }

    typename image_iterator::difference_type distance_to( image_iterator const & dst ) const
    {
        int n = dst.base() - this->base();
        int rest = (n > 0) ? ind_ : dst.ind_ % stride_;
        int s = abs(n) + rest;
        int res = abs(n)/n * (s/(int)stride_ * (int)width_ + s % (int)stride_  - rest);

        return res;
    }
};
//}

template<class Container = std::vector<uint8_t>>
class image
{
public:
    image(size_t width, size_t height, size_t stride):
        data(stride * height),
        width_(width),
        stride_(stride)
    {}

    auto begin()
    {
        return image_iterator(data.begin(), width_, stride_);
    }

    auto end()
    {
        return image_iterator(data.end(), width_, stride_);
    }

    auto begin() const
    {
        return image_iterator(data.begin(), width_, stride_);
    }

    auto end() const
    {
        return image_iterator(data.end(), width_, stride_);
    }

private:
    Container data; // image data including strides. Access by (x, y): data[y * stride_ + x]
    size_t width_;
    size_t stride_;
};

#endif // __ITERATOR_HPP__
