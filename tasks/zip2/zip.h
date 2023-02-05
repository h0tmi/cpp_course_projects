#pragma once

#include <iterator>

template <typename Sequence1, typename Sequence2>
class ZipIterator {
public:
    using Sequence1Reference = typename std::iterator_traits<Sequence1>::reference;
    using Sequence2Reference = typename std::iterator_traits<Sequence2>::reference;
    using ZippedPair = std::pair<const Sequence1Reference &, const Sequence2Reference &>;

    ZipIterator(const Sequence1 &iterator1, const Sequence2 &iterator2) : iterator1_(iterator1), iterator2_(iterator2) {
    }
    ZipIterator &operator++() {
        ++iterator1_;
        ++iterator2_;
        return *this;
    }
    ZippedPair operator*() const {
        return ZippedPair(*iterator1_, *iterator2_);
    }
    ZippedPair operator*() {
        return ZippedPair(*iterator1_, *iterator2_);
    }
    bool operator!=(const ZipIterator &other) const {
        return iterator1_ != other.iterator1_ && iterator2_ != other.iterator2_;
    }
    bool operator!=(const ZipIterator &other) {
        return iterator1_ != other.iterator1_ && iterator2_ != other.iterator2_;
    }

private:
    Sequence1 iterator1_;
    Sequence2 iterator2_;
};

template <typename Sequence1, typename Sequence2>
class Zipped {
public:
    ZipIterator<Sequence1, Sequence2> begin() const {  // NOLINT
        return ZipIterator(iterator1_begin_, iterator2_begin_);
    }
    ZipIterator<Sequence1, Sequence2> end() const {  // NOLINT
        return ZipIterator(iterator1_end_, iterator2_end_);
    }
    ZipIterator<Sequence1, Sequence2> begin() {  // NOLINT
        return ZipIterator(iterator1_begin_, iterator2_begin_);
    }
    ZipIterator<Sequence1, Sequence2> end() {  // NOLINT
        return ZipIterator(iterator1_end_, iterator2_end_);
    }
    Zipped(const Sequence1 &iterator1_begin, const Sequence1 &iterator1_end, const Sequence2 &iterator2_begin,
           const Sequence2 &iterator2_end)
        : iterator1_begin_(iterator1_begin),
          iterator1_end_(iterator1_end),
          iterator2_begin_(iterator2_begin),
          iterator2_end_(iterator2_end) {
    }

private:
    Sequence1 iterator1_begin_;
    Sequence1 iterator1_end_;
    Sequence2 iterator2_begin_;
    Sequence2 iterator2_end_;
};

template <typename Sequence1, typename Sequence2>
auto Zip(const Sequence1 &sequence1, const Sequence2 &sequence2) {
    return Zipped(std::begin(sequence1), std::end(sequence1), std::begin(sequence2), std::end(sequence2));
}