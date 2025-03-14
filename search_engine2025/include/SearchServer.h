#ifndef SEARCHSERVER_H
#define SEARCHSERVER_H

#include "InvertedIndex.h"


struct RelativeIndex{
    size_t doc_id;
    float rank;
    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer {
public:
    SearchServer(InvertedIndex& idx, int resp_limit) : _index(idx), responsesLimit(resp_limit){ };
    void setResponsesLimit(int count);
    /**
    * Метод обработки поисковых запросов
    * @param queries_input поисковые запросы взятые из файла
    requests.json
    * @return возвращает отсортированный список релевантных ответов для
    заданных запросов
    */
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);
private:
    InvertedIndex& _index;
    int responsesLimit;
};



#endif //SEARCHSERVER_H
