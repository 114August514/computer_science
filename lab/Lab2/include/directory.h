#pragma once

#include <iostream>

#define MAX_DIRECTORY_NUM 1001

namespace DATA_STRUCTURE
{
    template<typename Key, typename Value>
    class directory 
    {
        private:
            struct Pair
            {
                Key key{};
                Value value{};
            };

            int top_index = 0;
            Pair list_data[MAX_DIRECTORY_NUM]{};

        public: 
            directory() = default;
            ~directory() = default;

            int size() const
            {
                return top_index;
            }

            bool empty() const
            {
                return size() == 0;
            }

            Value *find(const Key &key)
            {
                for (int i = 0; i < size(); i++)
                {
                    if (list_data[i].key == key)
                        return &list_data[i].value;
                }
                return nullptr;
            }

            void set(const Key& key, const Value& value)
            {
                Value* found_value = find(key);

                if (found_value != nullptr)
                {
                    *found_value = value;
                    return;
                }

                // 键不存在，创建元素
                if (top_index < MAX_DIRECTORY_NUM)
                {
                    list_data[top_index].key = key;
                    list_data[top_index].value = value;
                    top_index++;
                }
                else 
                    std::cout << "DirectoryFullError!" << std::endl;
            }
            void clear()
            {
                top_index = 0;
            }

        };
}