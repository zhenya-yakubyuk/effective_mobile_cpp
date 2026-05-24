#include "ListNode.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <cassert>
#include <unordered_map>

namespace {
    // Helper: write integer in binary (little-endian)
    template<typename T>
    void writeBinary(std::ofstream& out, T value) {
        out.write(reinterpret_cast<const char*>(&value), sizeof(T));
    }

    // Helper: read integer in binary (little-endian)
    template<typename T>
    T readBinary(std::ifstream& in) {
        T value;
        in.read(reinterpret_cast<char*>(&value), sizeof(T));
        return value;
    }

    // Helper: write string with length prefix
    void writeString(std::ofstream& out, const std::string& s) {
        uint32_t len = static_cast<uint32_t>(s.size());
        writeBinary(out, len);
        out.write(s.c_str(), len);
    }

    // Helper: read string with length prefix
    std::string readString(std::ifstream& in) {
        uint32_t len = readBinary<uint32_t>(in);
        std::vector<char> buf(len);
        in.read(buf.data(), len);
        return std::string(buf.data(), len);
    }
}

void Serialize(ListNode* head, const char* filename) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        std::cerr << "Cannot open file for writing: " << filename << std::endl;
        return;
    }

    // Count nodes
    uint32_t count = 0;
    std::vector<ListNode*> nodes;
    for (ListNode* p = head; p != nullptr; p = p->next) {
        nodes.push_back(p);
        ++count;
    }

    // Write count
    writeBinary(out, count);

    // Map node pointer to index
    std::unordered_map<ListNode*, uint32_t> indexMap;
    for (uint32_t i = 0; i < nodes.size(); ++i) {
        indexMap[nodes[i]] = i;
    }

    // Write each node: data and rand index
    for (ListNode* p : nodes) {
        writeString(out, p->data);
        int32_t randIndex = -1;
        if (p->rand != nullptr) {
            auto it = indexMap.find(p->rand);
            assert(it != indexMap.end());
            randIndex = static_cast<int32_t>(it->second);
        }
        writeBinary(out, randIndex);
    }

    out.close();
}

ListNode* Deserialize(const char* filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        std::cerr << "Cannot open file for reading: " << filename << std::endl;
        return nullptr;
    }

    // Read count
    uint32_t count = readBinary<uint32_t>(in);
    if (count == 0) {
        return nullptr;
    }

    // Create nodes and read data & rand indices
    std::vector<ListNode*> nodes(count, nullptr);
    std::vector<int32_t> randIndices(count);

    for (uint32_t i = 0; i < count; ++i) {
        nodes[i] = new ListNode;
        nodes[i]->data = readString(in);
        randIndices[i] = readBinary<int32_t>(in);
    }

    // Link next/prev
    for (uint32_t i = 0; i < count; ++i) {
        if (i > 0) {
            nodes[i]->prev = nodes[i - 1];
        }
        if (i + 1 < count) {
            nodes[i]->next = nodes[i + 1];
        }
    }

    // Set rand pointers
    for (uint32_t i = 0; i < count; ++i) {
        int32_t idx = randIndices[i];
        if (idx >= 0 && static_cast<uint32_t>(idx) < count) {
            nodes[i]->rand = nodes[idx];
        } else {
            nodes[i]->rand = nullptr;
        }
    }

    in.close();
    return nodes[0];
}

ListNode* BuildFromText(const char* filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Cannot open text file: " << filename << std::endl;
        return nullptr;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) {
        lines.push_back(line);
    }

    const size_t n = lines.size();
    if (n == 0) {
        return nullptr;
    }

    // Parse each line: data;rand_index
    std::vector<std::string> datas(n);
    std::vector<int> randIndices(n);
    for (size_t i = 0; i < n; ++i) {
        const std::string& l = lines[i];
        size_t sep = l.find(';');
        if (sep == std::string::npos) {
            std::cerr << "Invalid line format: " << l << std::endl;
            FreeList(nullptr); // cleanup?
            return nullptr;
        }
        datas[i] = l.substr(0, sep);
        try {
            randIndices[i] = std::stoi(l.substr(sep + 1));
        } catch (...) {
            std::cerr << "Invalid rand index: " << l.substr(sep + 1) << std::endl;
            FreeList(nullptr);
            return nullptr;
        }
    }

    // Create nodes
    std::vector<ListNode*> nodes(n, nullptr);
    for (size_t i = 0; i < n; ++i) {
        nodes[i] = new ListNode;
        nodes[i]->data = datas[i];
    }

    // Link next/prev
    for (size_t i = 0; i < n; ++i) {
        if (i > 0) {
            nodes[i]->prev = nodes[i - 1];
        }
        if (i + 1 < n) {
            nodes[i]->next = nodes[i + 1];
        }
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

void FreeList(ListNode* head) {
    while (head) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}