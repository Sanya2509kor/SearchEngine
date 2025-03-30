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


        //создание вектора с частотой слов
        std::vector<size_t> vec;
        for (auto &word : unique_list) {
            size_t count_word = 0;
            for (auto &[doc_id, count] : _index.GetWordCount(word)) {
                count_word += count;
            }
            vec.push_back(count_word);
        }
        //сортировка от самого редкого
        for (int i = 0; i < unique_list.size(); i++) {
            for (int j = 0; j < unique_list.size() - 1 - i; j++) {
                if (vec[j] > vec[j + 1]) {
                    std::swap(vec[j], vec[j + 1]);
                    std::swap(unique_list[j], unique_list[j + 1]);
                }
            }
        }
        //Получаем список документов, где слово встречается и увеличиваем счётчик совпадений
        //считаем абсолютную релевантность для каждого документа
        std::map<size_t, int> doc_to_hitcount;
        std::map<size_t, int> abs_relevance;
        for (auto &word : unique_list) {
            for (auto &[doc_id, count] : _index.GetWordCount(word)) {
                doc_to_hitcount[doc_id] += 1;
                abs_relevance[doc_id] += count;
            }
        }
        //Проходим по списку слов, ищем max в документах с этим словом. После считаем rank
        std::vector<RelativeIndex> relative_index;
        std::map<size_t, double> check_docs;
        for (auto &word : unique_list) {
            double max = 0;
            for (auto &entry : _index.GetWordCount(word)) {
                if (max < abs_relevance[entry.doc_id] && check_docs[entry.doc_id] == 0) {
                    max = abs_relevance[entry.doc_id];
                }
            }
            for (auto &entry : _index.GetWordCount(word)) {
                if (check_docs[entry.doc_id] == 0) {
                    RelativeIndex relative;
                    check_docs[entry.doc_id] = abs_relevance[entry.doc_id] / max * doc_to_hitcount[entry.doc_id] / count_word;
                    relative.doc_id = entry.doc_id;
                    relative.rank = check_docs[entry.doc_id];
                    relative_index.push_back(relative);
                }
            }
        }


        //сортировка документов по убыванию релевантности
        for (int i = 0; i < relative_index.size(); i++) {
            for (int j = 0; j < relative_index.size() - 1; j++) {
                if (relative_index[j].rank < relative_index[j + 1].rank) {
                    std::swap(relative_index[j], relative_index[j + 1]);
                }
            }
        }


        if (relative_index.size() > responsesLimit) {
            relative_index.resize(responsesLimit);
        }

        list_answers.push_back(relative_index);
    }
    return list_answers;
}

void SearchServer::setResponsesLimit(int count) {
    if (count <= 0)
        responsesLimit = 5;

    responsesLimit = count;
}




