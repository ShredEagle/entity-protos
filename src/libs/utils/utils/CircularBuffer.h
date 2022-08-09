#pragma once

#include <array>

#include <cassert>


namespace ad {

template <class T, std::size_t N>
struct CircularBuffer
{
    std::size_t size() const
    {
        return mSize;
    };

    bool empty() const
    {
        return mSize == 0;
    }

    bool full() const
    {
        return mSize == N;
    }

    template <class U>
    void push(U && aEntry)
    {
        if(full())
        {
            mTail = (mTail + 1) % N;
        }
        else
        {
            ++mSize;
        }

        mHead = mNext;
        mStore[mNext++] = std::forward<U>(aEntry);
        mNext %= N;
    }

    std::size_t previousWrite()
    {
        // TODO this could overflow for insanely large buffers
        return (mTail + mSize - 1) % N;
    }

    T & rewindBounded()
    {
        assert(!empty());
        // If tail is not the position before next write
        if( (mTail + 1) % N
            != mNext)
        {
            mNext = previousWrite();
            --mSize;
        }
        return mStore[previousWrite()];
    }

    T & advanceBounded()
    {
        assert(!empty());
        // If head is not the position before next write
        // (i.e. next did no reach the end of available elements)
        if( (mHead + 1) % N
            != mNext)
        {
            mNext = (mNext + 1) % N;
            ++mSize;
        }
        return mStore[previousWrite()];
    }

    std::array<T, N> mStore;
    std::size_t mNext{0}; // next write position. Is moved as the client rewinds and advance.
    std::size_t mHead{0}; // newest entry position
    std::size_t mTail{0}; // oldest entry position
    std::size_t mSize{0}; // TODO the size could probably be derived efficiently from mNext and mTail
};

} // namespace ad
