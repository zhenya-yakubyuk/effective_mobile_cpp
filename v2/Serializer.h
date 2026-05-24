#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "ListNode.h"

class Serializer {
public:
    // Reads text file, builds list, serializes to "outlet.out", returns built list
    static ListNode* Serialize(const char* filename);
    static ListNode* Deserialize(const char* filename);
    static ListNode* DeserializeFromText(const char* filename);
};

#endif // SERIALIZER_H