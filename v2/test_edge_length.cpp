#include "Serializer.h"
#include "ListNode.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

int main() {
    // Create a test file with data length 1000 characters
    std::ofstream out("test_edge.in");
    std::string data(1000, 'a'); // exactly 1000 characters
    out << data << ";0" << std::endl;
    out.close();

    // Try to deserialize
    ListNode* list = Serializer::DeserializeFromText("test_edge.in");
    if (!list) {
        std::cerr << "DeserializeFromText failed for data length 1000" << std::endl;
        return 1;
    }

    // Verify data
    assert(list->data == data);
    assert(list->rand == list); // rand index 0 points to itself
    assert(list->next == nullptr);
    assert(list->prev == nullptr);

    std::cout << "Edge test passed: data length 1000 works." << std::endl;

    FreeList(list);
    std::remove("test_edge.in");
    return 0;
}