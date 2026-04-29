#ifndef LISTNODE_H
#define LISTNODE_H

#include <string>
#include <vector>

struct ListNode {
    ListNode* prev = nullptr;
    ListNode* next = nullptr;
    ListNode* rand = nullptr;
    std::string data;
};

#endif // LISTNODE_H