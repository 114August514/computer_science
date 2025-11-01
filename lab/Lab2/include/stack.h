#pragma once

#include <iostream>
#include "list.h"

namespace DATA_STRUCTURE {

template<class data_base>
class stack : public list<data_base> {
    public:
        stack() 
        {
            // head内元素是哑元，不允许访问。
            head = new Node();
        };

        ~stack() override 
        {
            clear();
            delete head;
        };

        bool empty() override {
            // TODO
            return head->next == nullptr;
        }

        void pop() override {
            // TODO
            if (empty()) return;
            Node* temp = head->next;
            head->next = temp->next;
            delete temp;
        }

        data_base top() override {
            // TODO
            if (empty())
            {
                std::cout << "StackEmptyError!" << std::endl;
                return data_base{};
            }
            return head->next->data;
        }

        void push(data_base push_data) override {
            // TODO
            Node* current = new Node(push_data, head->next);
            head->next = current;
        }

        void clear() override {
            // TODO
            Node* current = head->next;
            while (current != nullptr)
            {
                head->next = current->next;
                delete current;
                current = head->next;
            }
        }

    private:
        struct Node{
            data_base data{};
            Node* next = nullptr;
            
            Node() = default;
            Node(const data_base& d, Node* n) : data(d), next(n) {}
            ~Node() = default;
        };

        Node* head = nullptr;
        // int top_index = 0;
        // data_base list_data[MAX_LINK_LIST_NUM];
};

}
