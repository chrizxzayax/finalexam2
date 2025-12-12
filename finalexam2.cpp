// comsc 210 | finalexam2 | Christian Molina

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <random>
#include <chrono>
#include <algorithm>

struct Customer {
    std::string name;
    std::string order;
};

std::mt19937 make_rng() {
    // Seed with high-resolution clock for variability
    return std::mt19937(static_cast<unsigned>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()));
}

bool coin_flip(std::mt19937 &rng) {
    std::uniform_int_distribution<int> d(0, 1);
    return d(rng) == 1;
}

std::string random_pick(std::mt19937 &rng, const std::vector<std::string> &v) {
    std::uniform_int_distribution<size_t> d(0, v.size() - 1);
    return v[d(rng)];
}

// -------------------- Milestone 1 START --------------------

struct Node {
    Customer cust;
    Node* next;
    Node(const Customer &c) : cust(c), next(nullptr) {}
};

class LinkedListQueue {
public:
    LinkedListQueue() : head(nullptr), tail(nullptr), cnt(0) {}
    ~LinkedListQueue() {
        while (!empty()) pop_front();
    }

    void push_back(const Customer &c) {
        Node* n = new Node(c);
        if (!tail) {
            head = tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
        ++cnt;
    }

    // Remove and return the front customer; caller should check empty() first.
    Customer pop_front() {
        if (!head) return Customer{"", ""};
        Node* tmp = head;
        Customer c = head->cust;
        head = head->next;
        if (!head) tail = nullptr;
        delete tmp;
        --cnt;
        return c;
    }

    bool empty() const { return head == nullptr; }
    size_t size() const { return cnt; }

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

private:
    Node* head;
    Node* tail;
    size_t cnt;
};


// need to add a data set for names and orders
static const std::vector<std::string> NAMES = {
    "Alex", "Sam", "Jordan", "Taylor", "Casey", "Riley", "Morgan",
    "Jamie", "Avery", "Parker", "Quinn", "Robin", "Drew", "Cameron", "Sky"
};

// Orders specific to each vendor
static const std::vector<std::string> COFFEE_ORDERS = {
    "Latte", "Espresso", "Cappuccino", "Americano", "Mocha", "Macchiato"
};
static const std::vector<std::string> MUFFIN_ORDERS = {
    "Blueberry", "Chocolate Chip", "Bran", "Banana Nut", "Lemon", "Pumpkin"
};
static const std::vector<std::string> BRACELET_COLORS = {
    "Red", "Blue", "Green", "Purple", "Yellow", "Black"
};
static const std::vector<std::string> SMOOTHIE_FLAVORS = {
    "Strawberry", "Mango", "Banana", "Mixed Berry", "Peach", "Pineapple"
};
Customer make_customer_for_vendor(std::mt19937 &rng, const std::vector<std::string> &orderList) {
    Customer c;
    c.name = random_pick(rng, NAMES);
    c.order = random_pick(rng, orderList);
    return c;
}


int main() {
    auto rng = make_rng();
    LinkedListQueue coffeeQueue;
    // Initialize with 3 customers
    for (int i = 0; i < 3; ++i) {
        coffeeQueue.push_back(make_customer_for_vendor(rng, COFFEE_ORDERS));
    }
        std::cout << "  Queue now: [";
    coffeeQueue.print_queue();
    std::cout << "]\n";

    return 0;
}
