#pragma once

#define MAX_SET_NUM 101

namespace DATA_STRUCTURE {

template<class data_base>
class unordered_set {
    public:
        unordered_set() = default;
        ~unordered_set() = default;

        // TODO
        int size() const
        {
            return top_index;
        }

        bool operator==(const unordered_set<data_base>& other) const
        {
            if (size() != other.size()) return false;

            for (int i = 0; i < size(); i++)
            {
                if (!other.find(list_data[i])) return false;
            }
            return true;
        }

        bool operator!=(const unordered_set<data_base>& other) const
        {
            return !(*this == other);
        }

        bool empty() {
            return top_index == 0;
        }
        void insert(data_base push_data) {
            list_data[top_index] = push_data;
            top_index++;
        }
        void clear() {
            top_index = 0;
        }
        bool find(data_base check_data) const {
            for (int i = 0; i < top_index; i++)
                if (list_data[i] == check_data)
                    return true;
            return false;
        }
    private:
        int top_index = 0;
        data_base list_data[MAX_SET_NUM];
};

}