#include "ListNode.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include <cstdio>

int main() {
    // Test 1: empty file
    {
        ListNode* empty = BuildFromText("test_empty.in");
        assert(empty == nullptr);
        std::cout << "Test 1 passed: empty file returns nullptr" << std::endl;
    }

    // Test 2: single node, rand -1
    {
        std::ofstream out("test_single.in");
        out << "hello;-1" << std::endl;
        out.close();
        ListNode* single = BuildFromText("test_single.in");
        assert(single != nullptr);
        assert(single->next == nullptr);
        assert(single->prev == nullptr);
        assert(single->rand == nullptr);
        assert(single->data == "hello");
        Serialize(single, "test_single.out");
        ListNode* restored = Deserialize("test_single.out");
        assert(restored != nullptr);
        assert(restored->data == "hello");
        assert(restored->rand == nullptr);
        FreeList(single);
        FreeList(restored);
        std::cout << "Test 2 passed: single node with rand -1" << std::endl;
    }

    // Test 3: single node, rand 0 (self)
    {
        std::ofstream out("test_self.in");
        out << "self;0" << std::endl;
        out.close();
        ListNode* self = BuildFromText("test_self.in");
        assert(self != nullptr);
        assert(self->rand == self);
        Serialize(self, "test_self.out");
        ListNode* restored = Deserialize("test_self.out");
        assert(restored != nullptr);
        assert(restored->rand == restored);
        FreeList(self);
        FreeList(restored);
        std::cout << "Test 3 passed: self-referential node" << std::endl;
    }

    // Test 4: two nodes, cross references
    {
        std::ofstream out("test_cross.in");
        out << "A;1\nB;0" << std::endl;
        out.close();
        ListNode* cross = BuildFromText("test_cross.in");
        assert(cross != nullptr);
        assert(cross->next != nullptr);
        assert(cross->rand == cross->next);
        assert(cross->next->rand == cross);
        Serialize(cross, "test_cross.out");
        ListNode* restored = Deserialize("test_cross.out");
        assert(restored != nullptr);
        assert(restored->next != nullptr);
        assert(restored->rand == restored->next);
        assert(restored->next->rand == restored);
        FreeList(cross);
        FreeList(restored);
        std::cout << "Test 4 passed: two nodes cross-referencing" << std::endl;
    }

    // Test 5: data with spaces and special characters
    {
        std::ofstream out("test_special.in");
        out << "hello world;0\n\"quotes\";-1" << std::endl;
        out.close();
        ListNode* special = BuildFromText("test_special.in");
        assert(special != nullptr);
        assert(special->data == "hello world");
        assert(special->rand == special);
        assert(special->next != nullptr);
        assert(special->next->data == "\"quotes\"");
        assert(special->next->rand == nullptr);
        Serialize(special, "test_special.out");
        ListNode* restored = Deserialize("test_special.out");
        assert(restored != nullptr);
        assert(restored->data == "hello world");
        assert(restored->rand == restored);
        assert(restored->next->data == "\"quotes\"");
        assert(restored->next->rand == nullptr);
        FreeList(special);
        FreeList(restored);
        std::cout << "Test 5 passed: special characters" << std::endl;
    }

    // Cleanup temporary files
    std::remove("test_single.in");
    std::remove("test_single.out");
    std::remove("test_self.in");
    std::remove("test_self.out");
    std::remove("test_cross.in");
    std::remove("test_cross.out");
    std::remove("test_special.in");
    std::remove("test_special.out");

    std::cout << "All edge case tests passed!" << std::endl;
    return 0;
}