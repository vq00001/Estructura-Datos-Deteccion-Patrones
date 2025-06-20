// sais_suffix_array.hpp
#ifndef SAIS_SUFFIX_ARRAY_HPP
#define SAIS_SUFFIX_ARRAY_HPP

#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

class SAIS {
public:
    static void build_suffix_array(const std::string& text, std::vector<int>& sa) {
        std::string s = text;
        if (s.empty() || s.back() != '$') s += '$';
        int n = s.size();

        std::vector<int> s_int(n);
        for (int i = 0; i < n; ++i)
            s_int[i] = static_cast<unsigned char>(s[i]);

        sa.resize(n);
        sais_main(s_int, sa, 256); // 256 caracteres ASCII
    }

private:
    static void sais_main(const std::vector<int>& s, std::vector<int>& sa, int alphabet_size) {
        int n = s.size();
        std::vector<bool> is_s(n, false);
        std::vector<bool> is_lms(n, false);

        // Paso 1: Clasificar tipos S y L
        is_s[n - 1] = true;
        for (int i = n - 2; i >= 0; --i)
            is_s[i] = (s[i] < s[i + 1] || (s[i] == s[i + 1] && is_s[i + 1]));

        for (int i = 1; i < n; ++i)
            is_lms[i] = is_s[i] && !is_s[i - 1];

        // Paso 2: LMS-sort
        std::vector<int> lms_positions;
        for (int i = 1; i < n; ++i)
            if (is_lms[i]) lms_positions.push_back(i);

        induced_sort(s, sa, is_s, is_lms, lms_positions, alphabet_size);
    }

    static void induced_sort(const std::vector<int>& s, std::vector<int>& sa,
                             const std::vector<bool>& is_s,
                             const std::vector<bool>& is_lms,
                             const std::vector<int>& lms_positions,
                             int alphabet_size) {
        int n = s.size();
        std::vector<int> bucket_sizes(alphabet_size, 0);
        for (int c : s) bucket_sizes[c]++;

        auto get_buckets = [&](bool end) {
            std::vector<int> buckets(alphabet_size, 0);
            int sum = 0;
            for (int i = 0; i < alphabet_size; ++i) {
                sum += bucket_sizes[i];
                buckets[i] = end ? sum : sum - bucket_sizes[i];
            }
            return buckets;
        };

        sa.assign(n, -1);
        auto buckets = get_buckets(true);
        for (int i = lms_positions.size() - 1; i >= 0; --i) {
            int pos = lms_positions[i];
            sa[--buckets[s[pos]]] = pos;
        }

        // Induce L-type
        buckets = get_buckets(false);
        for (int i = 0; i < n; ++i) {
            int j = sa[i] - 1;
            if (j >= 0 && !is_s[j])
                sa[buckets[s[j]]++] = j;
        }

        // Induce S-type
        buckets = get_buckets(true);
        for (int i = n - 1; i >= 0; --i) {
            int j = sa[i] - 1;
            if (j >= 0 && is_s[j])
                sa[--buckets[s[j]]] = j;
        }
    }
};

#endif // SAIS_SUFFIX_ARRAY_HPP