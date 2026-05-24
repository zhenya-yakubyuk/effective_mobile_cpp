#include "Serializer.h"
#include "ListNode.h"
#include <iostream>
#include <cassert>

bool compareLists(ListNode* a, ListNode* b) {
    while (a && b) {
        if (a->data != b->data) return false;
        if (a->rand == nullptr && b->rand != nullptr) return false;
        if (a->rand != nullptr && b->rand == nullptr) return false;
        if (a->rand && b->rand && a->rand->data != b->rand->data) return false;
        a = a->next;
        b = b->next;
    }
    return a == nullptr && b == nullptr;
}

int main() {
    // Use Serializer class to read inlet.in and serialize to outlet.out
    ListNode* head = Serializer::Serialize("inlet.in");
    if (!head) {
        std::cerr << "Serializer::Serialize failed" << std::endl;
        return 1;
    }

    // Deserialize from outlet.out using global Deserialize
    ListNode* restored = Deserialize("outlet.out");
    if (!restored) {
        std::cerr << "Deserialize failed" << std::endl;
        FreeList(head);
        return 1;
    }

    // Compare
    if (compareLists(head, restored)) {
        std::cout << "Serializer class test passed." << std::endl;
    } else {
        std::cerr << "Serializer class test failed." << std::endl;
    }

    // Cleanup
    FreeList(head);
    FreeList(restored);

    return 0;
}