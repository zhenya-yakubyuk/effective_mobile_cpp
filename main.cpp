#include "Serializer.h"
#include <iostream>

int main(int argc, char **argv)
{
    Serializer serializer;
    ListNode* head = serializer.DeserializeFromText("inlet.in");
    if (head == nullptr) {
        std::cerr << "Failed to deserialize from inlet.in" << std::endl;
        return 1;
    }
    
    serializer.Serialize(head, "outlet.out");

    ListNode* current = head;
    while (current != nullptr) {
        ListNode* next = current->next;
        delete current;
        current = next;
    }
    
    return 0;
}
