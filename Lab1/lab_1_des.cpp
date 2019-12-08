// Initial permut matrix for the datas
const vector<int> PI = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
};

// Initial permut made on the key
const vector<int> CP_1 = {
        57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4
};

// Permut applied on shifted key to get Ki+1
const vector<int> CP_2 = {
        14, 17, 11, 24, 1, 5, 3, 28,
        15, 6, 21, 10, 23, 19, 12, 4,
        26, 8, 16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55, 30, 40,
        51, 45, 33, 48, 44, 49, 39, 56,
        34, 53, 46, 42, 50, 36, 29, 32
};

// Expand matrix to get a 48bits matrix of datas to apply the xor with Ki
const vector<int> E = {
        32, 1, 2, 3, 4, 5,
        4, 5, 6, 7, 8, 9,
        8, 9, 10, 11, 12, 13,
        12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21,
        20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29,
        28, 29, 30, 31, 32, 1
};

// SBOX
const vector<vector<vector<int>>> S_BOX = {
        {
                {14, 4,  13, 1,  2,  15, 11, 8,  3,  10, 6,  12, 5,  9,  0,  7},
                {0,  15, 7,  4,  14, 2,  13, 1,  10, 6, 12, 11, 9,  5,  3,  8},
                {4,  1,  14, 8,  13, 6,  2,  11, 15, 12, 9,  7,  3,  10, 5,  0},
                {15, 12, 8,  2,  4,  9,  1,  7,  5,  11, 3,  14, 10, 0, 6,  13}
        },

        {
                {15, 1,  8,  14, 6,  11, 3,  4,  9,  7,  2,  13, 12, 0,  5,  10},
                {3,  13, 4,  7,  15, 2,  8,  14, 12, 0, 1,  10, 6,  9,  11, 5},
                {0,  14, 7,  11, 10, 4,  13, 1,  5,  8,  12, 6,  9,  3,  2,  15},
                {13, 8,  10, 1,  3,  15, 4,  2,  11, 6,  7,  12, 0,  5, 14, 9},
        },

        {
                {10, 0,  9,  14, 6,  3,  15, 5,  1,  13, 12, 7,  11, 4,  2,  8},
                {13, 7,  0,  9,  3,  4,  6,  10, 2,  8, 5,  14, 12, 11, 15, 1},
                {13, 6,  4,  9,  8,  15, 3,  0,  11, 1,  2,  12, 5,  10, 14, 7},
                {1,  10, 13, 0,  6,  9,  8,  7,  4,  15, 14, 3,  11, 5, 2,  12},
        },

        {
                {7,  13, 14, 3,  0,  6,  9,  10, 1,  2,  8,  5,  11, 12, 4,  15},
                {13, 8,  11, 5,  6,  15, 0,  3,  4,  7, 2,  12, 1,  10, 14, 9},
                {10, 6,  9,  0,  12, 11, 7,  13, 15, 1,  3,  14, 5,  2,  8,  4},
                {3,  15, 0,  6,  10, 1,  13, 8,  9,  4,  5,  11, 12, 7, 2,  14},
        },

        {
                {2,  12, 4,  1,  7,  10, 11, 6,  8,  5,  3,  15, 13, 0,  14, 9},
                {14, 11, 2,  12, 4,  7,  13, 1,  5,  0, 15, 10, 3,  9,  8,  6},
                {4,  2,  1,  11, 10, 13, 7,  8,  15, 9,  12, 5,  6,  3,  0,  14},
                {11, 8,  12, 7,  1,  14, 2,  13, 6,  15, 0,  9,  10, 4, 5,  3},
        },

        {
                {12, 1,  10, 15, 9,  2,  6,  8,  0,  13, 3,  4,  14, 7,  5,  11},
                {10, 15, 4,  2,  7,  12, 9,  5,  6,  1, 13, 14, 0,  11, 3,  8},
                {9,  14, 15, 5,  2,  8,  12, 3,  7,  0,  4,  10, 1,  13, 11, 6},
                {4,  3,  2,  12, 9,  5,  15, 10, 11, 14, 1,  7,  6,  0, 8,  13},
        },

        {
                {4,  11, 2,  14, 15, 0,  8,  13, 3,  12, 9,  7,  5,  10, 6,  1},
                {13, 0,  11, 7,  4,  9,  1,  10, 14, 3, 5,  12, 2,  15, 8,  6},
                {1,  4,  11, 13, 12, 3,  7,  14, 10, 15, 6,  8,  0,  5,  9,  2},
                {6,  11, 13, 8,  1,  4,  10, 7,  9,  5,  0,  15, 14, 2, 3,  12},
        },

        {
                {13, 2,  8,  4,  6,  15, 11, 1,  10, 9,  3,  14, 5,  0,  12, 7},
                {1,  15, 13, 8,  10, 3,  7,  4,  12, 5, 6,  11, 0,  14, 9,  2},
                {7,  11, 4,  1,  9,  12, 14, 2,  0,  6,  10, 13, 15, 3,  5,  8},
                {2,  1,  14, 7,  4,  10, 8,  13, 15, 12, 9,  0,  3,  5, 6,  11},
        }
};

// Permut made after each SBox substitution for each round
const vector<int> P = {
        16, 7, 20, 21, 29, 12, 28, 17,
        1, 15, 23, 26, 5, 18, 31, 10,
        2, 8, 24, 14, 32, 27, 3, 9,
        19, 13, 30, 6, 22, 11, 4, 25
};

// Final permut for datas after the 16 rounds
const vector<int> PI_1 = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
};

// Matrix that determine the shift for each round of keys
const vector<int> SHIFT = {
        1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

const int DES_ENCRYPT = 1;
const int DES_DECRYPT = 0;
const int DES_KEY_SIZE = 8;

class Des {
private:
    string password;
    string text;
    vector<vector<int>> keys;

    static vector<int> permut(vector<int> block, vector<int> table) {
        auto result = vector<int>();

        for (auto i = 0; i < table.size(); i++) {
            auto item = table[i];
            result.push_back(block[item - 1]);
        }

        return result;
    }

    static vector<vector<int>> shift(vector<int> g, vector<int> d, int n) {
        auto result = vector<vector<int>>();

        result.push_back(Utils::vector_concat(
                Utils::vector_slice(g, n, g.size()),
                Utils::vector_slice(g, 0, n)
        ));

        result.push_back(Utils::vector_concat(
                Utils::vector_slice(d, n, d.size()),
                Utils::vector_slice(d, 0, n)
        ));

        return result;
    }

    static vector<int> expand(vector<int> block, vector<int> table) {
        return Des::permut(block, table);
    }

    void generate_keys() {
        this->keys = vector<vector<int>>();

        auto key = Utils::string_to_bit_array(this->password);
        key = Des::permut(key, CP_1);

        auto gd = Utils::n_split(key, 28);
        auto g = gd[0];
        auto d = gd[1];

        for (auto i = 0; i < 16; i++) {
            auto tmp_gd = Des::shift(g, d, SHIFT[i]);
            g = tmp_gd[0];
            d = tmp_gd[1];

            auto tmp = Utils::vector_concat(g, d);
            this->keys.push_back(Des::permut(tmp, CP_2));
        }
    }

    static vector<int> substitute(vector<int> d_e) {
        auto sub_blocks = Utils::n_split(d_e, 6);
        auto result = vector<int>();

        for (auto i = 0; i < sub_blocks.size(); i++) {
            auto block = sub_blocks[i];

            auto block_row = std::to_string(block[0]) + std::to_string(block[5]);
            auto row = std::stoi(block_row, nullptr, 2);

            string block_column = "";

            auto tmp = Utils::vector_slice(block, 1, -1);
            for (auto j = 0; j < tmp.size(); j++) {
                block_column += std::to_string(tmp[j]);
            }

            auto column = std::stoi(block_column, nullptr, 2);
            auto val = S_BOX[i][row][column];

            auto bin = Utils::int_bin_value(val, 4);

            for (auto j = 0; j < bin.size(); j++) {
                auto bit = bin[j] == '1' ? 1 : 0;
                result.push_back(bit);
            }
        }

        return result;
    }

public:
    Des(string password, string text) {
        this->password = std::move(password);
        this->text = std::move(text);
    }

    string run(int action = 1, bool padding = false) {
        if (this->password.size() < DES_KEY_SIZE) {
            throw InvalidKeySize();
        } else if (this->password.size() > 8) {
            this->password = this->password.substr(0, DES_KEY_SIZE);
        }

        if (padding && action == DES_ENCRYPT) {
            this->text = Utils::add_padding(this->text, DES_KEY_SIZE);
        } else if (this->text.size() % 8 != 0) {
            throw InvalidTextSize();
        }

        this->generate_keys();

        auto text_blocks = Utils::n_split_string(this->text, 8);
        auto result = vector<int>();

        for (auto b = 0; b < text_blocks.size(); b++) {
            auto item = text_blocks[b];
            auto block = Utils::string_to_bit_array(item);

            block = Des::permut(block, PI);
            auto gd = Utils::n_split(block, 32);
            auto g = gd[0];
            auto d = gd[1];

            auto tmp = vector<int>();
            for (auto i = 0; i < 16; i++) {
                auto d_e = Des::expand(d, E);

                if (action == DES_ENCRYPT) {
                    tmp = Utils::vector_xor(this->keys[i], d_e);
                } else {
                    tmp = Utils::vector_xor(this->keys[15 - i], d_e);
                }

                tmp = Des::substitute(tmp);
                tmp = Des::permut(tmp, P);
                tmp = Utils::vector_xor(g, tmp);

                g = d;
                d = tmp;
            }

            result = Utils::vector_concat(result, Des::permut(
                Utils::vector_concat(d, g), PI_1
            ));
        }

        auto output = Utils::bit_array_to_string(result);
        if (padding && action == DES_DECRYPT) {
            return Utils::remove_padding(output);
        }
        return output;
    }

    static string encrypt(string password, string text, bool padding = false) {
        return Des(password, text).run(DES_ENCRYPT, padding);
    }

    static string decrypt(string password, string text, bool padding = false) {
        return Des(password, text).run(DES_DECRYPT, padding);
    }
};