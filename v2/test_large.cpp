#include "ListNode.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cassert>

int main() {
    const int N = 10000; // 10k nodes for quick test
    std::srand(std::time(nullptr));

    // Create nodes and store in vector for easy rand assignment
    std::vector<ListNode*> nodes;
    for (int i = 0; i < N; ++i) {
        ListNode* node = new ListNode;
        node->data = "Node" + std::to_string(i);
        nodes.push_back(node);
    }

    // Link next/prev
    for (int i = 0; i < N; ++i) {
        if (i > 0) nodes[i]->prev = nodes[i - 1];
        if (i + 1 < N) nodes[i]->next = nodes[i + 1];
    }

    // Assign random rand pointers (some nullptr, some random)
    for (int i = 0; i < N; ++i) {
        int r = std::rand() % (N + 1); // 0..N, where N means nullptr
        if (r == N) {
            nodes[i]->rand = nullptr;
        } else {
            nodes[i]->rand = nodes[r];
        }
    }

    ListNode* head = nodes[0];

    // Serialize
    Serialize(head, "large_test.out");

    // Deserialize
    ListNode* restored = Deserialize("large_test.out");
    assert(restored != nullptr);

    // Compare
    ListNode* a = head;
    ListNode* b = restored;
    int count = 0;
    while (a && b) {
        assert(a->data == b->data);
        // compare rand
        if (a->rand == nullptr) {
            assert(b->rand == nullptr);
        } else {
            assert(b->rand != nullptr);
            // rand points to node with same data (since we can't compare pointers across lists)
            // we compare data of rand target
            assert(a->rand->data == b->rand->data);
        }
        a = a->next;
        b = b->next;
        ++count;
    }
    assert(a == nullptr && b == nullptr);
    assert(count == N);

    std::cout << "Large test passed with " << N << " nodes." << std::endl;

    // Cleanup
    FreeList(head);
    FreeList(restored);

    // Remove temporary file
    std::remove("large_test.out");

    return 0;
}