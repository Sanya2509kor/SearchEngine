

#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H

#include <vector>
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <mutex>
#include <thread>


struct Entry {
    size_t doc_id, count;
    // Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
        count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex() = default;
    void UpdateDocumentBase(std::vector<std::string> input_docs);
    std::vector<Entry> GetWordCount(const std::string& word);
private:
    void Streams(size_t doc_id, const std::string& input_doc);
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;
    std::mutex mut;
};



#endif //INVERTEDINDEX_H
