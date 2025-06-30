#ifndef FM_INDEX_HPP
#define FM_INDEX_HPP

#include <string>
#include <vector>
#include <utility>

class FMIndex {
public:
    FMIndex(const std::string& text);
    std::vector<int> search(const std::string& pattern);

private:
    std::string bwt;
    std::vector<int> ctable;
    std::vector<std::vector<int>> occ_checkpoint_table;
    std::vector<int> suffix_array;
    static const int CHECKPOINT_INTERVAL = 64;

    std::string prepareTextForBWT(const std::string& text);
    void buildBWT(const std::string& input);
    void buildCTable();
    void buildOccTable();
    int getOcc(unsigned char c, int k) const;
    std::pair<int, int> backwardSearch(const std::string& pattern);
};

#endif // FM_INDEX_HPP