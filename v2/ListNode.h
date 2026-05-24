#ifndef LISTNODE_H
#define LISTNODE_H

#include <string>
#include <vector>
#include <cstdint>

struct ListNode {
    ListNode* prev = nullptr;
    ListNode* next = nullptr;
    ListNode* rand = nullptr;
    std::string data;
};

// Serialization: writes list starting from head to binary file
void Serialize(ListNode* head, const char* filename);

// Deserialization: reads binary file and reconstructs list, returns head
ListNode* Deserialize(const char* filename);

// Helper: build list from text file (inlet.in format)
ListNode* BuildFromText(const char* filename);

// Helper: free entire list
void FreeList(ListNode* head);

#endif // LISTNODE_H