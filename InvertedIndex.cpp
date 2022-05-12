#include "InvertedIndex.h"
#include "breakText.h"

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs)
{

    for (auto &doc_text : input_docs)
    {
        docs.push_back(doc_text);
    }

    for (int i = 0; i < docs.size(); ++i)
    {
        std::vector<std::string> file_words = breakText(docs[i]);
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
