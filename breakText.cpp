#include "breakText.h"

std::vector<std::string> breakText(const std::string& text)
{
    std::vector<std::string> words;
    std::string word;
    for (auto &symbol : text)
    {
        if (symbol != ' ')
        {
            word += symbol;
        }
        else if (!word.empty())
        {
            words.push_back(word);
            word.clear();
        }

    }
    if (!word.empty())
        words.push_back(word);

    return words;
}
