#include "InvertedIndex.h"
#include "json.hpp"
#include "breakText.h"
#include <fstream>
#include "gtest/gtest.h"
#include <exception>

using namespace std;

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs)
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
    for (auto &element : paths) {
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
            docs.push_back(file_text);
            file.close();
        } else {
            std::cerr << "File on path " << element << " is not found!" << std::endl;
        }
    }

    for (int i = 0; i < input_docs.size(); ++i)
    {
        std::vector<std::string> file_words = breakText(input_docs[i]);
        for(auto &word : file_words)
        {
            if (freq_dictionary.find(word) != freq_dictionary.end())
            {
                if (i < freq_dictionary[word].size())
                    ++freq_dictionary[word][i].count;
                else
                {
                    Entry entry = {(size_t)i , 1};
                    freq_dictionary[word].push_back(entry);
                }
            }
            else
            {
                Entry entry = {(size_t)i , 1};
                freq_dictionary[word] = { entry };
            }
        }

    }

}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word)
{
    if (freq_dictionary.find(word) != freq_dictionary.end())
    {
        return freq_dictionary[word];
    }
    else
    {
        std::vector<Entry> vec;
        return vec;
    }

}

void TestInvertedIndexFunctionality(
        const vector<string>& docs,
        const vector<string>& requests,
        const std::vector<vector<Entry>>& expected
) {
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for(auto& request : requests) {
        std::vector<Entry> word_count = idx.GetWordCount(request);
        result.push_back(word_count);
    }
    ASSERT_EQ(result, expected);
}

TEST(TestCaseInvertedIndex, TestBasic) {
    const vector<string> docs = {
            "london is the capital of great britain",
            "big ben is the nickname for the Great bell of the striking clock"
    };
    const vector<string> requests = {"london", "the"};
    const vector<vector<Entry>> expected = {
            {
                    {0, 1}
            }, {
                    {0, 1}, {1, 3}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestBasic2) {
    const vector<string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };
    const vector<string> requests = {"milk", "water", "cappuccino"};
    const vector<vector<Entry>> expected = {
            {
                    {0, 4}, {1, 1}, {2, 5}
            }, {
                    {0, 3}, {1, 2}, {2, 5}
            }, {
                    {3, 1}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    const vector<string> docs = {
            "a b c d e f g h i j k l",
            "statement"
    };
    const vector<string> requests = {"m", "statement"};
    const vector<vector<Entry>> expected = {
            {
            }, {
                    {1, 1}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}