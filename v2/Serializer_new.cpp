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
                    std::cerr << "Data or index too long" << std::endl;
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
                    std::cerr << "Data or index too long" << std::endl;
                    in.close();
                    return nullptr;
                }
                indexBuffer[counter - dataLength] = ch;
                counter++;
            }
        }

        // handle last line if file does not end with newline
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
            // missing semicolon? error
            std::cerr << "Missing ';' before EOF" << std::endl;
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