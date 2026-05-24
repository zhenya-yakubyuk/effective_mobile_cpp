#include "ListNode.h"
#include <iostream>
#include <cassert>

// Helper function to compare two lists
bool CompareLists(ListNode* a, ListNode* b) {
    while (a != nullptr && b != nullptr) {
        if (a->data != b->data) return false;
        // compare rand pointers: both null or both point to nodes with same data
        if (a->rand == nullptr && b->rand != nullptr) return false;
        if (a->rand != nullptr && b->rand == nullptr) return false;
        if (a->rand != nullptr && b->rand != nullptr) {
            if (a->rand->data != b->rand->data) return false;
        }
        a = a->next;
        b = b->next;
    }
    return a == nullptr && b == nullptr;
}

int main() {
    // 1. Read text file and build list
    ListNode* original = BuildFromText("inlet.in");
    if (!original) {
        std::cerr << "Failed to build list from inlet.in" << std::endl;
        return 1;
    }

    // 2. Serialize to binary file
    Serialize(original, "outlet.out");
    std::cout << "Serialization completed." << std::endl;

    // 3. Deserialize for verification
    ListNode* restored = Deserialize("outlet.out");
    if (!restored) {
        std::cerr << "Failed to deserialize from outlet.out" << std::endl;
        FreeList(original);
        return 1;
    }

    // 4. Compare
    if (CompareLists(original, restored)) {
        std::cout << "Verification passed: original and restored lists are identical." << std::endl;
    } else {
        std::cerr << "Verification failed: lists differ!" << std::endl;
    }

    // Cleanup
    FreeList(original);
    FreeList(restored);

    return 0;
}