#include "SearchServer.h"
#include <algorithm>
#include "breakText.h"

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input)
{
    std::vector<std::vector<RelativeIndex>> search_result;
    for (auto &request : queries_input) {
        std::vector<std::string> request_words = breakText(request);
        std::vector<std::pair<std::string, size_t>> counted_words;
        auto last = std::unique(request_words.begin(), request_words.end());
        request_words.erase(last, request_words.end());
        for (auto &word: request_words) {
            auto freq = _index.GetWordCount(word);
            size_t count = 0;
            for (auto &entry: freq) {
                count += entry.count;
            }
            counted_words.emplace_back(std::make_pair(word, count));
        }
        std::sort(counted_words.begin(), counted_words.end(), [](std::pair<std::string, size_t> &pair1,
                                                                 std::pair<std::string, size_t> &pair2) {
            return pair1.second < pair2.second;
        });
        std::vector<size_t> docs_id;
        for (int i = 0; i < counted_words.size(); ++i) {
            if (i == 0) {
                std::vector<Entry> word_info = _index.GetWordCount(counted_words[i].first);
                for (auto &entry: word_info) {
                    docs_id.push_back(entry.doc_id);
                }
            } else {
                std::vector<Entry> word_info = _index.GetWordCount(counted_words[i].first);
                std::vector<size_t> current_word_doc;
                for (auto &entry: word_info) {
                    current_word_doc.push_back(entry.doc_id);
                }
                for (auto &element: docs_id) {
                    if (std::find(current_word_doc.begin(), current_word_doc.end(), element) ==
                        current_word_doc.end()) {
                        docs_id.erase(std::find(docs_id.begin(), docs_id.end(), element));
                    }
                }
                if (docs_id.empty())
                    break;
            }

        }
        if (!docs_id.empty()) {
            std::vector<RelativeIndex> request_result;
            std::vector<size_t> abs_relevance;
            std::vector<float> relevance;
            for (auto &doc: docs_id) {
                size_t abs_rel = 0;
                for (auto &word: request_words) {
                    std::vector<Entry> word_freq = _index.GetWordCount(word);
                    for (auto &entry: word_freq) {
                        if (entry.doc_id == doc) {
                            abs_rel += entry.count;
                            break;
                        }

                    }
                }
                abs_relevance.push_back(abs_rel);
            }

            auto max_abs_rel = *(std::max_element(abs_relevance.begin(), abs_relevance.end()));
            for (auto &abs_rel: abs_relevance) {
                relevance.push_back(static_cast<float>(abs_rel) / max_abs_rel);
            }

            for (int i = 0; i < docs_id.size(); ++i) {
                RelativeIndex index = {docs_id[i], relevance[i]};
                request_result.push_back(index);
            }

            std::sort(request_result.begin(), request_result.end(), [](RelativeIndex &index1, RelativeIndex &index2) {
                return index1.rank > index2.rank;
            });

            search_result.push_back(request_result);

        }
        else
        {
            std::vector<RelativeIndex> empty;
            search_result.push_back(empty);
        }
    }

    return search_result;

}
