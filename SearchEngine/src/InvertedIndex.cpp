#include "../include/InvertedIndex.h"


void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = input_docs;
    freq_dictionary.clear();
    const int threadCount = std::thread::hardware_concurrency() - 1;
    std::vector<std::thread> threads;
    threads.reserve(threadCount);
    int docsCount = docs.size();
    int verifiedDoc = 0;
    int maxCheck = 0;
    while (docsCount > 0) {
        if (docsCount < threadCount) { maxCheck -= (threadCount - docsCount);}
        for (int i = verifiedDoc; i < threadCount + maxCheck; i++) {
            threads.emplace_back(Streams, this, i, docs[i]);
        }
        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        verifiedDoc += threadCount;
        maxCheck += threadCount;
        docsCount -= threadCount;
    }

}


std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    if (freq_dictionary.count(word) > 0) {
        return freq_dictionary[word];
    }
    std::vector<Entry> entry;
    return entry;
}


void InvertedIndex::Streams(size_t doc_id, const std::string& input_doc) {
    std::stringstream doc(input_doc);
    std::string word;
    std::map<std::string, Entry> freq;
    while (doc >> word) {
        freq[word].doc_id = doc_id;
        freq[word].count += 1;
    }
    mut.lock();
    for (auto &map : freq) {
        freq_dictionary[map.first].push_back(map.second);
    }
    mut.unlock();
}

