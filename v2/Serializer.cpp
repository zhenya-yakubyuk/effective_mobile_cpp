#include "Serializer.h"
#include "ListNode.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

ListNode* Serializer::Serialize(const char* filename) {
    // 1. Build list from text file
    ListNode* head = BuildFromText(filename);
    if (!head) {
        std::cerr << "Serializer: failed to build list from " << filename << std::endl;
        return nullptr;
    }

    // 2. Serialize to binary file "outlet.out"
    ::Serialize(head, "outlet.out"); // call global Serialize

    // 3. Return the built list
    return head;
}

ListNode* Serializer::Deserialize(const char* filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Cannot open text file: " << filename << std::endl;
        return nullptr;
    }

    std::vector<std::string> datas;
    std::vector<int> randIndices;

    char dataBuffer[1001];
    char indexBuffer[1001];
    int bufPos = 0;
    bool readingData = true;
    bool readingIndex = false;
    char ch;

    while (in.get(ch)) {
        if (ch == ';') {
            if (readingData) {
                dataBuffer[bufPos] = '\0';
                datas.emplace_back(dataBuffer);
                bufPos = 0;
                readingData = false;
                readingIndex = true;
            } else {
                // unexpected semicolon
                std::cerr << "Unexpected ';' in file" << std::endl;
                in.close();
                return nullptr;
            }
        } else if (ch == '\r') {
            // ignore carriage return
            continue;
        } else if (ch == '\n') {
            if (readingIndex) {
                indexBuffer[bufPos] = '\0';
                try {
                    int idx = std::stoi(indexBuffer);
                    randIndices.push_back(idx);
                } catch (...) {
                    std::cerr << "Invalid rand index: " << indexBuffer << std::endl;
                    in.close();
                    return nullptr;
                }
                bufPos = 0;
                readingIndex = false;
                readingData = true;
            } else if (readingData) {
                // empty line? ignore
                bufPos = 0;
            }
        } else {
            if (bufPos >= 1000) {
                std::cerr << "Data or index too long" << std::endl;
                in.close();
                return nullptr;
            }
            if (readingData) {
                dataBuffer[bufPos++] = ch;
            } else if (readingIndex) {
                indexBuffer[bufPos++] = ch;
            }
        }
    }

    // handle last line if file does not end with newline
    if (readingIndex && bufPos > 0) {
        indexBuffer[bufPos] = '\0';
        try {
            int idx = std::stoi(indexBuffer);
            randIndices.push_back(idx);
        } catch (...) {
            std::cerr << "Invalid rand index at EOF: " << indexBuffer << std::endl;
            in.close();
            return nullptr;
        }
    } else if (readingData && bufPos > 0) {
        // missing semicolon? error
        std::cerr << "Missing ';' before EOF" << std::endl;
        in.close();
        return nullptr;
    }

    in.close();

    // Build list
    size_t n = datas.size();
    if (n == 0) {
        return nullptr;
    }

    std::vector<ListNode*> nodes(n, nullptr);
    for (size_t i = 0; i < n; ++i) {
        nodes[i] = new ListNode;
        nodes[i]->data = datas[i];
    }

    // Link next/prev
    for (size_t i = 0; i < n; ++i) {
        if (i > 0) nodes[i]->prev = nodes[i - 1];
        if (i + 1 < n) nodes[i]->next = nodes[i + 1];
    }

    // Set rand pointers
    for (size_t i = 0; i < n; ++i) {
        int idx = randIndices[i];
        if (idx >= 0 && static_cast<size_t>(idx) < n) {
            nodes[i]->rand = nodes[idx];
        } else {
            nodes[i]->rand = nullptr;
        }
    }

    return nodes[0];
}

ListNode* Serializer::DeserializeFromText(const char* filename)
{
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Cannot open text file: " << filename << std::endl;
        return nullptr;
    }
    
    std::vector<std::string> datas;
    std::vector<int> randIndices;
    char dataBuffer[1001];
    char indexBuffer[1001];
    int counter = 0;
    int dataLength = 0;
    bool dataFound = false, indexFound = false;
    char ch;
    
    try {
        while (in.get(ch)) {
            
            if(dataFound && indexFound && (ch == '\r' || ch == '\n'))
            {
                continue;
            }
            else if (dataFound && indexFound)
            {
                dataFound = false;
                indexFound = false;
                counter = 0;
                dataLength = 0;
            }
            
            if(!dataFound)
            {
                if (ch == ';') {
                    dataFound = true;
                    dataBuffer[counter] = '\0';
                    datas.emplace_back(dataBuffer);
                    dataLength = counter;
                    continue;
                }
                
                if (counter >= 1000) {
                    std::cerr << "Data long" << std::endl;
                    in.close();
                    return nullptr;
                }

                dataBuffer[counter++] = ch;
            }
            else if(dataFound && !indexFound)
            {
                if (ch == '\r' || ch == '\n') {
                    indexFound = true;
                    indexBuffer[counter - dataLength] = '\0';
                    try {
                        randIndices.push_back(std::stoi(indexBuffer));
                    } catch (...) {
                        std::cerr << "Invalid rand index: " << indexBuffer << std::endl;
                        in.close();
                        return nullptr;
                    }
                    continue;
                }
                
                if (counter - dataLength >= 1000) {
                    std::cerr << "Index too long" << std::endl;
                    in.close();
                    return nullptr;
                }
                indexBuffer[counter - dataLength] = ch;
                counter++;
            }
        }

        if (dataFound && !indexFound && counter > dataLength) {
            indexBuffer[counter - dataLength] = '\0';
            try {
                randIndices.push_back(std::stoi(indexBuffer));
            } catch (...) {
                std::cerr << "Invalid rand index at EOF" << std::endl;
                in.close();
                return nullptr;
            }
        } else if (!dataFound && counter > 0) {
            std::cerr << "Missing ';' in the last line" << std::endl;
            in.close();
            return nullptr;
        }
    }
    catch (...) {
        std::cerr << "Unknown error occurred while reading file '"
                  << filename << "'" << std::endl;

        if (in.is_open()) {
            in.close();
        }
        
        return nullptr;
    }
    
    in.close();

    size_t n = datas.size();
    if (n == 0) {
        return nullptr;
    }

    std::vector<ListNode*> nodes(n, nullptr);
    for (size_t i = 0; i < n; ++i) {
        nodes[i] = new ListNode;
        nodes[i]->data = datas[i];
    }

    for (size_t i = 0; i < n; ++i) {
        if (i > 0) nodes[i]->prev = nodes[i - 1];
        if (i + 1 < n) nodes[i]->next = nodes[i + 1];
    }

    for (size_t i = 0; i < n; ++i) {
        int idx = randIndices[i];
        if (idx >= 0 && static_cast<size_t>(idx) < n) {
            nodes[i]->rand = nodes[idx];
        } else {
            nodes[i]->rand = nullptr;
        }
    }

    return nodes[0];
}