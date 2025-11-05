# pragma once

#include <iostream>
#include <math.h>

#include "queue.h"
#include "unordered_set.h"
#include "directory.h"

namespace DATA_STRUCTURE {

struct Point
{
    int x, y;

    bool operator==(const Point& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const
    {
        return x != other.x || y != other.y;
    }
};

struct VisitSet
{
    Point pos{};
    unordered_set<int> get_gold{};

    bool operator==(const VisitSet& other) const
    {
        return pos == other.pos && get_gold == other.get_gold;
    }

    bool operator!=(const VisitSet& other) const
    {
        return !(*this == other);
    }
};

class walk {
    public:
        walk() = default;
        walk(int _steps, int _gold_num, Point* _gold, int* _gold_value) {
            steps = _steps;
            gold_num = _gold_num;
            gold = _gold;
            gold_value = _gold_value;
        }
        ~walk() = default;

        void print_para();
        int get_value();
    private:
        int gold_num = 0;
        Point* gold{};
        int* gold_value{};
        int steps = 0;

        struct element {
            int steps;
            int value;
            Point pos;
            unordered_set<int> get_gold;
        };
        queue<struct element> q;

        int compute_distance(int i, const Point& pos);
};

}