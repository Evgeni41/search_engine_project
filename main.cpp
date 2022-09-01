#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include <fstream>
#include "json.hpp"
#include "gtest/gtest.h"

std::string EngineVersion = "0.1";

class ConfigKeyMissedException : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "Failed to find config parameter!";
    }
};

class InvalidVersionException : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "config.json has incorrect file version!";
    }
};

void starting(std::ifstream& file_json)
{
    if (file_json.is_open())
    {
        nlohmann::json config_json;
        file_json >> config_json;
        if (config_json.find("config") == config_json.end())
        {
            throw ConfigKeyMissedException();
        }
        else
        {
            nlohmann::json config_data;
            config_data = config_json["config"];
            if (config_data["version"] != EngineVersion)
            {
                throw InvalidVersionException();
            }
            else {
                std::cout << config_data["name"] << std::endl;
            }
        }
    }
    else
    {
        throw std::exception();
    }
}

int main() {

    ::testing::InitGoogleTest();
    RUN_ALL_TESTS();
    try {
        std::ifstream config_file("config.json");
        starting(config_file);
        config_file.close();
    }
    catch (ConfigKeyMissedException& ex) {
        std::cerr << ex.what() << std::endl;
        return 0;
    }
    catch (InvalidVersionException& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 0;
    }
    catch (const std::exception &open_file_error) {
        std::cerr << "Config file is missing!" << std::endl;
        return 0;
    }

    std::ofstream answers_clear("answers.json");
    answers_clear.close();
    ConverterJSON converterJson;
    InvertedIndex invertedIndex;
    try
    {
        invertedIndex.UpdateDocumentBase(converterJson.GetTextDocuments());
    }
    catch (const InvalidRequestPathException &request_path_error)
    {
        std::cerr << request_path_error.what() << std::endl;
        return 0;
    }
    catch (const std::exception &config_path_error)
    {
        std::cerr << "Invalid config path! Cannot find config.json!" << std::endl;
        return 0;
    }


    std::string command = "exit";
    do
    {
        std::cout << "Input command:" << std::endl;
        std::cin >> command;
        if (command == "update") {
            invertedIndex.UpdateDocumentBase(converterJson.GetTextDocuments());
        }
        if (command == "search") {
            SearchServer searchServer(invertedIndex);
            converterJson.putAnswers(searchServer.search(converterJson.GetRequests()));
        }
    } while (command != "exit");


}







