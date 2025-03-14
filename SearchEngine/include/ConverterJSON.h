//
// Created by user on 18.02.2025.
//

#ifndef CONVERTERJSON_H
#define CONVERTERJSON_H

#include <fstream>
#include <iostream>
#include <vector>
#include "SearchServer.h"
#include "../nlohmann_json/include/nlohmann/json.hpp"

class ConverterJSON {
public:
    ConverterJSON();
    std::vector<std::string> GetTextDocuments();
    int GetResponsesLimit();
    std::vector<std::string> GetRequests();
    void putAnswers(std::vector<std::vector<RelativeIndex>>answers);
private:
    std::string config_json = "config.json";
    std::string requests_json = "requests.json";
    std::string answers_json = "answers.json";
};



#endif //CONVERTERJSON_H
