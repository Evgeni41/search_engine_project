#include "ConverterJSON.h"
#include "json.hpp"
#include <fstream>

const char* InvalidRequestPathException::what() const noexcept
{
        return "Invalid request path! Cannot find request.json!";
}

std::vector<std::string> ConverterJSON::GetTextDocuments()
{
    std::ifstream config_json(config_path);
    if (!config_json.is_open())
    {
        throw std::exception();
    }
    nlohmann::json config;
    config_json >> config;
    config_json.close();
    std::vector<std::string> paths = config["files"];
    std::vector<std::string> textDocuments;
    for (auto &element: paths) {
        std::ifstream file(element);
        if (file.is_open()) {
            std::string file_text;
            while (!file.eof()) {
                std::string str;
                getline(file, str);
                file_text += str;
                if (!file.eof())
                    file_text += " ";

            }
            textDocuments.push_back(file_text);
            file.close();
        }
        else
        {
            std::cerr << "File on path " << element << " is not found!" << std::endl;
        }

    }
    return textDocuments;


}

int ConverterJSON::GetResponsesLimit()
{
    std::ifstream config_file(config_path);
    if (!config_file.is_open()) throw std::exception();
    nlohmann::json config_json;
    config_file >> config_json;
    nlohmann::json config_data;
    config_data = config_json["config"];
    int responsesLimit = config_data["max_responses"];
    config_file.close();
    if (responsesLimit <= 0) return 0;
    else return responsesLimit;

}

std::vector<std::string> ConverterJSON::GetRequests()
{
    std::ifstream requests_file(requests_path);
    if (!requests_file.is_open()) throw InvalidRequestPathException();
    nlohmann::json requests_json;
    requests_file >> requests_json;
    std::vector<std::string> requests;
    for (auto &request : requests_json["requests"])
    {
        requests.push_back(request);
    }
    requests_file.close();
    return requests;

}

void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>> answers)
{
    std::ofstream answer_file(answers_path);
    nlohmann::json search_result;
    nlohmann::json requests_answers;
    int limit = GetResponsesLimit();
    if (limit <= 0)
    {
        std::cerr << "You max_responses field has invalid value!" << std::endl;
    }
    else {
        for (int i = 0; i < answers.size(); ++i) {
            nlohmann::json request_results;
            std::string result_name;
            result_name = "request " + std::to_string(i);
            if (!answers[i].empty()) {
                request_results["result"] = true;
                if (answers[i].size() == 1 || limit == 1) {
                    request_results["docid"] = answers[i][0].doc_id;
                    request_results["rank"] = answers[i][0].rank;
                } else {

                    std::vector<nlohmann::json> relevance;
                    int j = 1;
                    for (auto &pair: answers[i]) {
                        if (j > limit) break;
                        nlohmann::json one_pair;
                        one_pair["docid"] = pair.doc_id;
                        one_pair["rank"] = pair.rank;
                        relevance.push_back(one_pair);
                        ++j;
                    }
                    request_results["relevance"] = relevance;
                }
                requests_answers[result_name] = request_results;
            } else {
                request_results["result"] = false;
                requests_answers[result_name] = request_results;
            }

        }
        search_result["answers"] = requests_answers;
        answer_file << search_result.dump(4);
        answer_file.close();
    }

}

