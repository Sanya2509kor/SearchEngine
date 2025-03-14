#include "../include/ConverterJSON.h"

//реализация методов класса

ConverterJSON::ConverterJSON() {
    std::ifstream file(config_json);
    if (!file.is_open()) {
        throw std::invalid_argument("config file is missing");
    }
    nlohmann::json json;
    file >> json;
    if (json.count("config") <= 0) {
        throw std::invalid_argument("config file is empty");
    }
    if (json["config"]["name"] > 0) {
        std::cout << "---------" << json["config"]["name"] << "---------" << std::endl;
    }


}


std::vector<std::string> ConverterJSON::GetTextDocuments() {

    std::ifstream file(config_json);
    if (!file.is_open()) {
        throw std::invalid_argument("config file is missing");
    }
    nlohmann::json json;
    file >> json;
    std::vector<std::string> paths;
    for (std::string files : json["files"]) {
        paths.push_back(files);
    }

    std::vector<std::string> docs;

    if (paths.size() > 0) {
        for (auto &path : paths) {
            std::ifstream txt(path);
            if (txt.is_open()) {
                std::string text;
                std::string word;
                while (txt >> word) {
                    text += word + " ";
                }
                docs.push_back(text);

            } else {
                std::cerr << "incorrect path " << path << std::endl;
            }
            txt.close();
        }
    } else {
        std::cerr << "No file paths!" << std::endl;
    }
    file.close();
    return docs;
}

int ConverterJSON::GetResponsesLimit() {
    nlohmann::json json;
    std::ifstream file(config_json);
    if (!file.is_open()) {
        throw std::invalid_argument("config file is missing");
    }
    file >> json;
    file.close();
    if (json.count("config") > 0) {
        if (json["config"].count("max_responses") > 0) {
            return json["config"]["max_responses"];
        }
        return 5;
    }
    throw std::invalid_argument("config file is empty");
}


std::vector<std::string> ConverterJSON::GetRequests() {
    nlohmann::json json;
    std::ifstream file(requests_json);
    std::vector<std::string> requests;
    if (!file.is_open()) {
        throw std::invalid_argument("config file is missing");
    }
    file >> json;
    if (json.count("requests") > 0) {
        int count_requests = 0;
        for (std::string files : json["requests"]) {
            count_requests += 1;
            if (count_requests > 1000) {
                std::cerr << "Error, there are more than 1000 requests" << std::endl;
                break;
            }
            requests.push_back(files);
        }
    } else {
        std::cerr << "There are no requests. Edit the requests file.json and enter \"update\"" << std::endl;
    }
    file.close();
    return requests;
}

void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>> answers) {
    nlohmann::ordered_json json;
    std::ofstream file(answers_json, std::ios::trunc);
    json["answers"] = {};

    bool result = false;
    for (int i = 0; i < answers.size(); i++) {
        std::string request;
        (i >= 9) ? ((i >= 99) ? (request = "request") : (request = "request0")) : (request = "request00");
        nlohmann::ordered_json relevance;
        if (answers[i].size() > 0) {result = true;}

        for (int j = 0; j < answers[i].size(); j ++) {
            relevance += {{"docid", answers[i][j].doc_id}, {"rank", answers[i][j].rank}};
        }

        if (answers[i].size() > 1) {
            json["answers"][request + std::to_string(i + 1)] = {
                {"result", result},
                {"relevance", relevance}
            };
        } else if (answers[i].size() > 0){
            json["answers"][request + std::to_string(i + 1)] = {
                {"result", result},
                {"docid", answers[i][0].doc_id},
                {"rank", answers[i][0].rank}
            };
        } else {
            json["answers"][request + std::to_string(i + 1)] = {
                {"result", result}
            };
        }
        result = false;
    }
    file << json.dump(4);
    file.close();
}