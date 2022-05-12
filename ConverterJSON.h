#pragma once
#include "SearchServer.h"
#include <iostream>
#include <vector>

class ConverterJSON
{
    std::string config_path = "config.json";
    std::string requests_path = "requests.json";
    std::string answers_path = "answers.json";
public:
    ConverterJSON() = default;
    std::vector<std::string> GetTextDocuments();
    int GetResponsesLimit();
    std::vector<std::string> GetRequests();
    void putAnswers(std::vector<std::vector<RelativeIndex>> answers);

};
