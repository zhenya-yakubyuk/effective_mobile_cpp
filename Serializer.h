#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <string>
#include <fstream>
#include <iostream>

#include "ListNode.h"

class Serializer {
  public:
    ListNode* DeserializeFromText(const char* filename);
    void Serialize(ListNode* head, const char* filename);
};

#endif // SERIALIZER_H