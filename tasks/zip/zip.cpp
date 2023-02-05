#include "zip.h"
Zipped::Zipped(Iterator& iterator1_begin, Iterator& iterator1_end, Iterator& iterator2_begin, Iterator& iterator2_end)
    : iterator1_begin_(iterator1_begin),
      iterator1_end_(iterator1_end),
      iterator2_begin_(iterator2_begin),
      iterator2_end_(iterator2_end) {
}
Zipped::ZippedIterator::ZippedIterator(const Iterator& iterator1, const Iterator& iterator2)
    : iterator1_(iterator1), iterator2_(iterator2) {
}
Zipped::ZippedIterator& Zipped::ZippedIterator::operator++() {
    ++iterator1_;
    ++iterator2_;
    return *this;
}
ZippedPair Zipped::ZippedIterator::operator*() const {
    return ZippedPair(*iterator1_, *iterator2_);
}
ZippedPair Zipped::ZippedIterator::operator*() {
    return ZippedPair(*iterator1_, *iterator2_);
}
bool Zipped::ZippedIterator::operator!=(const Zipped::ZippedIterator& other) const {
    return iterator1_ != other.iterator1_ && iterator2_ != other.iterator2_;
}
bool Zipped::ZippedIterator::operator!=(const Zipped::ZippedIterator& other) {
    return iterator1_ != other.iterator1_ && iterator2_ != other.iterator2_;
}
Zipped::ZippedIterator Zipped::begin() const {
    return Zipped::ZippedIterator(iterator1_begin_, iterator2_begin_);
}
Zipped::ZippedIterator Zipped::end() const {
    return Zipped::ZippedIterator(iterator1_end_, iterator2_end_);
}
Zipped::ZippedIterator Zipped::begin() {
    return Zipped::ZippedIterator(iterator1_begin_, iterator2_begin_);
}
Zipped::ZippedIterator Zipped::end() {
    return Zipped::ZippedIterator(iterator1_end_, iterator2_end_);
}
Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end) {
    return Zipped(a_begin, a_end, b_begin, b_end);
}
