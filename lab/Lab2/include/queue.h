#pragma once

#include <iostream>
#include "list.h"

namespace DATA_STRUCTURE {

template<class data_base>
class queue : public list<data_base> {
    public:
        queue() = default;
        ~queue() override = default;

        bool empty() override {
            // TODO
            return top_index == tail_index;
        }

        void pop() override {
            // TODO
            if (!empty())
            {
                top_index = (top_index + 1) % MAX_LINK_LIST_NUM;
            }
        }

        data_base top() override {
            // TODO
            if (empty())
            {
                std::cout << "QueueEmptyError!" << std::endl;
                return data_base{};
            }

            return list_data[top_index];
        }

        void push(data_base push_data) override {
            // TODO
            int next = (tail_index + 1) % MAX_LINK_LIST_NUM;

            if (next == top_index)
            {
                std::cout << "QueueFullError!" << std::endl;
                return;
            }

            list_data[tail_index] = push_data;
            tail_index = next;
        }

        void clear() override {
            // TODO
            top_index = 0;
            tail_index = 0;
        }
        
    private:
        int top_index = 0;
        int tail_index = 0;
        data_base list_data[MAX_LINK_LIST_NUM];
};

}
