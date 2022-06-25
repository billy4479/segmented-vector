#pragma once

#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <vector>

#ifndef NDEBUG
#include <fmt/core.h>
#include <sstream>
#endif

template <typename Type> class SegmentedVector {
  public:
    SegmentedVector() { add_segment(); }
    SegmentedVector(std::initializer_list<Type> objs) : SegmentedVector() {
        for (auto &obj : objs)
            push_back(obj);
    }
    ~SegmentedVector() {
        for (Segment &segment : m_segments)
            delete[] segment.data;

        m_segments.clear();
    }

  public:
    [[nodiscard]] Type *get(size_t index) {
        if (index >= size())
            return nullptr;

        size_t segment_capacity_accumulator = 0;
        for (Segment &segment : m_segments) {
            segment_capacity_accumulator += segment.capacity;
            if (segment_capacity_accumulator > index) {
                // We are in the right segment
                return &segment.data[index - (segment_capacity_accumulator -
                                              segment.capacity)];
            }
        }

        // Unreachable
        assert(false && "Unreachable");
        return nullptr;
    }

    void push_back(const Type &obj) {
        if (size() >= capacity())
            add_segment();
        Segment &s = m_segments.back();
        auto result = s.push_back(obj);
        assert(result != nullptr);
    }

  public:
    size_t size() const {
        size_t s = 0;
        for (const Segment &segment : m_segments)
            s += segment.size;

        return s;
    }
    size_t capacity() const {
        size_t c = 0;
        for (const Segment &segment : m_segments)
            c += segment.capacity;

        return c;
    }

    void print_me() {
#ifndef NDEBUG
        for (size_t i = 0; i < size(); i++)
            fmt::print("{}\n", *get(i));

        for (const auto &segment : m_segments) {
            std::stringstream ss;
            for (size_t i = 0; i < segment.size; i++)
                ss << segment.data[i] << ",";

            fmt::print("{{ data: {{ ptr: {}, data: [{}] }}, size: {}, "
                       "capacity: {} }}\n",
                       static_cast<void *>(segment.data), ss.str(),
                       segment.size, segment.capacity);
        }
#endif
    }

  private:
    struct Segment {
        Type *data{nullptr};
        size_t size;
        size_t capacity;

        Type *push_back(const Type &obj) {
            if (size >= capacity)
                return nullptr;

            data[size] = obj;
            return &data[size++];
        }
    };

    // TODO: create something custom
    std::vector<Segment> m_segments;
    size_t next_segment_capacity = 2;
    static constexpr int segment_capacity_multiplier = 2;

  private:
    void add_segment() {
        Segment segment = {
            .data = new Type[next_segment_capacity](),
            .size = 0,
            .capacity = next_segment_capacity,
        };
        next_segment_capacity *= segment_capacity_multiplier;
        m_segments.push_back(segment);
    }
};