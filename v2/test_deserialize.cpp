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
    // Build list using Serializer::Serialize (which also reads inlet.in)
    ListNode* head = Serializer::Serialize("inlet.in");
    if (!head) {
        std::cerr << "Failed to build list via Serializer::Serialize" << std::endl;
        return 1;
    }

    // Deserialize using Serializer::Deserialize (reading same text file)
    ListNode* deserialized = Serializer::Deserialize("inlet.in");
    if (!deserialized) {
        std::cerr << "Failed to deserialize via Serializer::Deserialize" << std::endl;
        FreeList(head);
        return 1;
    }

    // Compare
    if (compareLists(head, deserialized)) {
        std::cout << "Serializer::Deserialize test passed." << std::endl;
    } else {
        std::cerr << "Serializer::Deserialize test failed." << std::endl;
    }

    // Cleanup
    FreeList(head);
    FreeList(deserialized);

    return 0;
}