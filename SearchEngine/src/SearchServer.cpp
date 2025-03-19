#include "../include/SearchServer.h"


std::vector<std::vector<RelativeIndex> > SearchServer::search(const std::vector<std::string> &queries_input) {
    std::vector<std::vector<RelativeIndex>> list_answers;
    for (auto & request : queries_input) {
        //проход по запросу и сортировка уникальных слов
        std::stringstream text(request);
        std::string string;
        std::vector<std::string> unique_list;
        int count_word = 0;
        while (text >> string) {
            count_word += 1;
            if (count_word > 10) {break;}
            bool unique = true;
            for (auto & str : unique_list) {
                if (string == str) unique = false;
            }
            if (unique) unique_list.push_back(string);
        }
        if (count_word > 10) {
            std::cerr << "Error, there are more than 10 words in the query. Your request: (" << request << ")" << std::endl;
            list_answers.push_back({{}});
            continue;
        }

        //считаем частоту каждого слова. Если есть слово с частотой 0, пропускаем запрос, записываем пустой вектор
        bool check_0 = false;
        std::vector<size_t> id_size;
        for (auto &word : unique_list) {
            size_t count = 0;
            for (auto &entry : _index.GetWordCount(word)) {
                count += entry.count;
            }
            if (count == 0) {
                list_answers.push_back({});
                check_0 = true;
                break;
            }
            id_size.push_back(count);
        }
        if (check_0) {
            continue;
        }

        //сортировка слов по увеличению частоты
        for (int i = 0; i < unique_list.size(); i++) {
            for (int j = 0; j < unique_list.size() - 1 - i; j++) {
                if (id_size[j] > id_size[j + 1]) {
                    std::swap(id_size[j], id_size[j + 1]);
                    std::swap(unique_list[j], unique_list[j + 1]);
                }
            }
        }

        //ищем все документы в которых есть первое слово
        std::map<size_t, int> docs;
        for (auto &entry : _index.GetWordCount(unique_list[0])) {
            docs[entry.doc_id] = 0;
        }

        //проверяем, есть ли остальные слова в этих документах, и находим абсолютную релевантность
        for (auto &word : unique_list) {
            std::map<size_t, int> docs_in_word;
            for (auto &entry : _index.GetWordCount(word)) {
                for (auto &doc : docs) {
                    if (entry.doc_id == doc.first) {
                        docs_in_word[doc.first] = 0;
                        docs[doc.first] += entry.count;
                        break;
                    }
                }
            }
            if (docs.size() > docs_in_word.size()) {
                for (auto &pair : docs_in_word) {
                    pair.second = docs[pair.first];
                }
                docs = docs_in_word;
            }
        }


        double max = 0;
        for (auto &pair : docs) {
            if (max < pair.second) {
                max = pair.second;
            }
        }

        //вычисление релевантности
        std::vector<RelativeIndex> relative;
        for (auto &pair : docs) {
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




