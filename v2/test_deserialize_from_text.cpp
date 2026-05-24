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
    // Build list using BuildFromText (global function)
    ListNode* expected = BuildFromText("inlet.in");
    if (!expected) {
        std::cerr << "Failed to build expected list" << std::endl;
        return 1;
    }

    // Deserialize using Serializer::DeserializeFromText
    ListNode* result = Serializer::DeserializeFromText("inlet.in");
    if (!result) {
        std::cerr << "Failed to deserialize via DeserializeFromText" << std::endl;
        FreeList(expected);
        return 1;
    }

    // Compare
    if (compareLists(expected, result)) {
        std::cout << "Serializer::DeserializeFromText test passed." << std::endl;
    } else {
        std::cerr << "Serializer::DeserializeFromText test failed." << std::endl;
    }

    // Cleanup
    FreeList(expected);
    FreeList(result);

    return 0;
}