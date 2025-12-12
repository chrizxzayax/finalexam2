// comsc 210 | finalexam2 | Christian Molina

#include <iostream>
#include <string>
#include <random>
#include <chrono>

struct Customer {
    std::string name;
    std::string order;
};

std::mt19937 make_rng() {
    // Seed with high-resolution clock for variability
    return std::mt19937(static_cast<unsigned>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()));
}

// -------------------- Milestone 1 START --------------------

struct Node {
    Customer cust;
    Node* next;
    Node(const Customer &c) : cust(c), next(nullptr) {}
};

class LinkedListQueue {
public:
    LinkedListQueue() : head(nullptr), tail(nullptr){}

    void push_back(const Customer &c) {
        Node* n = new Node(c);
        if (!tail) {
            head = tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
    }

    bool empty() const { return head == nullptr; }

    // Print queue contents in order head -> tail
    void print_queue() const {
        Node* cur = head;
        std::cout << "[";
        bool first = true;
        while (cur) {
            if (!first) std::cout << " | ";
            std::cout << cur->cust.name << "(" << cur->cust.order << ")";
            first = false;
            cur = cur->next;
        }
        std::cout << "]";
    }

private:// private members that are used in the class
    Node* head;
    Node* tail;
};