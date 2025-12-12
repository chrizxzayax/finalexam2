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
std::mt19937 make_rng(uint32_t seed = 0) {// default seed 0 means use time-based seed
    if (seed == 0) {
        return std::mt19937(static_cast<uint32_t>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    } else {// fixed seed for reproducibility
        return std::mt19937(seed);
    }
}


bool coin_flip(std::mt19937 &rng) {
    std::uniform_int_distribution<int> d(0, 1);
    return d(rng) == 1;
}

std::string random_pick(std::mt19937 &rng, const std::vector<std::string> &v) {
    if (v.empty()) return std::string();
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
    void clear() {// helper to clear all nodes
        while (head) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
        tail = nullptr;
        cnt = 0;
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

void print_deque_queue(const std::deque<Customer> &dq) {
    std::cout << "[";
    for (size_t i = 0; i < dq.size(); ++i) {
        if (i) std::cout << " | ";
        std::cout << dq[i].name << "(" << dq[i].order << ")";
    }
    std::cout << "]";
}

int main() {
    uint32_t seed = 0;
    auto rng = make_rng(seed);

    // Initialize booths
    LinkedListQueue coffeeQueue;
    for (int i = 0; i < 3; ++i) coffeeQueue.push_back(make_customer_for_vendor(rng, COFFEE_ORDERS));

    std::deque<Customer> muffinQueue;
    for (int i = 0; i < 3; ++i) muffinQueue.push_back(make_customer_for_vendor(rng, MUFFIN_ORDERS));

     const int ROUNDS = 10;

    std::cout << "Starting simulation for " << ROUNDS << " rounds";
    if (seed != 0) std::cout << " (seed=" << seed << ")";
    std::cout << ".\n\n";

    for (int round = 1; round <= ROUNDS; ++round) {
        std::cout << "===== Round " << round << " =====\n";

        // Coffee booth (LinkedListQueue)
        std::cout << "Coffee Booth:\n";
        if (!coffeeQueue.empty()) {
            Customer served = coffeeQueue.pop_front();
            std::cout << "  Served: " << served.name << " (" << served.order << ")\n";
        } else {
            std::cout << "  Served: (none, queue empty)\n";
        }
        if (coin_flip(rng)) {
            Customer newcomer = make_customer_for_vendor(rng, COFFEE_ORDERS);
            coffeeQueue.push_back(newcomer);
            std::cout << "  Joined: " << newcomer.name << " (" << newcomer.order << ")\n";
        } else {
            std::cout << "  Joined: (no one)\n";
        }
        std::cout << "  Queue now: ";// print linked list queue
        coffeeQueue.print_queue();
        std::cout << "\n\n";

        // Muffin booth (std::deque)
        std::cout << "Muffin Booth:\n";
        if (!muffinQueue.empty()) {
            Customer served = muffinQueue.front();
            muffinQueue.pop_front();
            std::cout << "  Served: " << served.name << " (" << served.order << ")\n";
        } else {
            std::cout << "  Served: (none, queue empty)\n";
        }
        if (coin_flip(rng)) {
            Customer newcomer = make_customer_for_vendor(rng, MUFFIN_ORDERS);
            muffinQueue.push_back(newcomer);
            std::cout << "  Joined: " << newcomer.name << " (" << newcomer.order << ")\n";
        } else {
            std::cout << "  Joined: (no one)\n";
        }
        std::cout << "  Queue now: ";
        print_deque_queue(muffinQueue);
        std::cout << "\n\n";

    }    // Final summary: sizes of each queue

    std::cout << "===== Simulation finished =====\n";
    std::cout << "Final queue sizes:\n";
    std::cout << "  Coffee: " << coffeeQueue.size() << "\n";
    std::cout << "  Muffin: " << muffinQueue.size() << "\n";

    return 0;
}
