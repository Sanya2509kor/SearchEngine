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
    int count = 0;
    while (doc >> word) {
        if (word.size() > 100) {
            mut.lock();
            std::cerr << "Error, A word has more than 100 characters in a document ID = " << doc_id  << std::endl;
            mut.unlock();
            return;
        }
        freq[word].doc_id = doc_id;
        freq[word].count += 1;
        count += 1;
        if (count > 1000) {
            mut.lock();
            std::cerr << "Error, file ID - " << doc_id<< " contains more than 1000 words"  << std::endl;
            mut.unlock();
            return;
        }
    }
    mut.lock();
    for (auto &map : freq) {
        freq_dictionary[map.first].push_back(map.second);
    }
    mut.unlock();
}

