#pragma once

#include <forward_list>
#include <string>

using Value = std::string;
using Iterator = std::forward_list<std::string>::const_iterator;
using ZippedPair = std::pair<const Value&, const Value&>;
using SizeType = size_t;

class Zipped {
public:
    class ZippedIterator {
    public:
        ZippedIterator(const Iterator& iterator1, const Iterator& iterator2);
        ZippedIterator& operator++();
        ZippedPair operator*() const;
        ZippedPair operator*();
        bool operator!=(const ZippedIterator& other) const;
        bool operator!=(const ZippedIterator& other);

    private:
        Iterator iterator1_;
        Iterator iterator2_;
    };
    ZippedIterator begin() const;
    ZippedIterator end() const;
    ZippedIterator begin();
    ZippedIterator end();
    Zipped(Iterator& iterator1_begin, Iterator& iterator1_end, Iterator& iterator2_begin, Iterator& iterator2_end);

private:
    Iterator iterator1_begin_;
    Iterator iterator1_end_;
    Iterator iterator2_begin_;
    Iterator iterator2_end_;
};

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end);
