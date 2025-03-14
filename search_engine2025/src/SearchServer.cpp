#include "../include/SearchServer.h"


std::vector<std::vector<RelativeIndex> > SearchServer::search(const std::vector<std::string> &queries_input) {
    std::vector<std::vector<RelativeIndex>> list_answers;
    for (auto & request : queries_input) {
        //проход по запросу и сортировка уникальных слов
        std::stringstream text(request);
        std::string word;
        std::vector<std::string> unique_list;
        while (text >> word) {
            bool unique = true;
            for (auto & str : unique_list) {
                if (word == str) unique = false;
            }
            if (unique) unique_list.push_back(word);
        }

        //Если делать как в ТЗ, то вот так получается
        /////////////////////////////////////////////////////////////////

        // //создание вектора с количеством слова в документе
        // std::vector<int> word_count;
        // for (auto &word : unique_list) {
        //     int count = 0;
        //     for (auto &entry : _index.GetWordCount(word)) {
        //         count += entry.count;
        //     }
        //     word_count.push_back(count);
        // }
        // //сортировка
        // for (int i = 0; i < unique_list.size(); i++) {
        //     for (int j = 0; j < unique_list.size() - 1; j++) {
        //         if (word_count[j] > word_count[j+1]) {
        //             std::swap(word_count[j], word_count[j+1]);
        //             std::swap(unique_list[j], unique_list[j+1]);
        //         }
        //     }
        // }
        //
        // std::vector<size_t> doc;
        // if (unique_list.size() > 0) {
        //     for (auto &entry : _index.GetWordCount(unique_list[0])) {
        //         doc.push_back(entry.doc_id);
        //     }
        // }
        // //
        // for (auto &word : unique_list) {
        //     std::vector<size_t> b;
        //     for (auto &entry : _index.GetWordCount(word)) {
        //         for (auto &doc_id : doc) {
        //             if (entry.doc_id == doc_id) {
        //                 b.push_back(doc_id);
        //             }
        //         }
        //     }
        //     if (doc.size() > b.size()) {
        //         doc = b;
        //     }
        // }
        // std::map<size_t, int> sum_words;
        // double max = 0;
        // for (auto &doc_id : doc) {
        //     for (auto &word : unique_list) {
        //         sum_words[doc_id] += _index.GetWordCount(word)[doc_id].count;
        //     }
        //     if (sum_words[doc_id] > max) {
        //         max = sum_words[doc_id];
        //     }
        // }
        //
        // //вычисление релевантности
        // std::vector<RelativeIndex> relative;
        // for (auto &pair : sum_words) {
        //     RelativeIndex index;
        //     index.doc_id = pair.first;
        //     index.rank = pair.second / max;
        //     relative.push_back(index);
        // }
        //
        // //сортировка документов по убыванию релевантности
        // for (int i = 0; i < relative.size(); i++) {
        //     for (int j = 0; j < relative.size() - 1; j++) {
        //         if (relative[j].rank < relative[j + 1].rank) {
        //             std::swap(relative[j], relative[j + 1]);
        //         }
        //     }
        // }
        //
        // if (relative.size() > responsesLimit) {
        //     relative.resize(responsesLimit);
        // }
        //
        // list_answers.push_back(relative);

        /////////////////////////////////////////////////////////////////


        //рассчитываем абсолютную релевантность и находим max
        std::map<int, int> sum_words;
        double max = 0;
        for ( auto &word : unique_list) {
            if (_index.GetWordCount(word).size() > 0) {
                for (auto &entry : _index.GetWordCount(word)) {
                    sum_words[entry.doc_id] += entry.count;
                    if (sum_words[entry.doc_id] > max) {
                        max = sum_words[entry.doc_id];
                    }
                }
            }
        }


        //вычисление релевантности
        std::vector<RelativeIndex> relative;
        for (auto &pair : sum_words) {
            RelativeIndex index;
            index.doc_id = pair.first;
            index.rank = pair.second / max;
            relative.push_back(index);
        }

        //сортировка документов по убыванию релевантности
        for (int i = 0; i < relative.size(); i++) {
            for (int j = 0; j < relative.size() - 1; j++) {
                if (relative[j].rank < relative[j + 1].rank) {
                    std::swap(relative[j], relative[j + 1]);
                }
            }
        }


        if (relative.size() > responsesLimit) {
            relative.resize(responsesLimit);
        }

        list_answers.push_back(relative);
    }
    return list_answers;
}

void SearchServer::setResponsesLimit(int count) {
    if (count <= 0)
        responsesLimit = 5;

    responsesLimit = count;
}




