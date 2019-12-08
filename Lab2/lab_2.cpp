const vector<vector<int>> H_TABLE = {
        {0xB1, 0x94, 0xBA, 0xC8, 0x0A, 0x08, 0xF5, 0x3B, 0x36, 0x6D, 0x00, 0x8E, 0x58, 0x4A, 0x5D, 0xE4},
        {0x85, 0x04, 0xFA, 0x9D, 0x1B, 0xB6, 0xC7, 0xAC, 0x25, 0x2E, 0x72, 0xC2, 0x02, 0xFD, 0xCE, 0x0D},
        {0x5B, 0xE3, 0xD6, 0x12, 0x17, 0xB9, 0x61, 0x81, 0xFE, 0x67, 0x86, 0xAD, 0x71, 0x6B, 0x89, 0x0B},
        {0x5C, 0xB0, 0xC0, 0xFF, 0x33, 0xC3, 0x56, 0xB8, 0x35, 0xC4, 0x05, 0xAE, 0xD8, 0xE0, 0x7F, 0x99},
        {0xE1, 0x2B, 0xDC, 0x1A, 0xE2, 0x82, 0x57, 0xEC, 0x70, 0x3F, 0xCC, 0xF0, 0x95, 0xEE, 0x8D, 0xF1},
        {0xC1, 0xAB, 0x76, 0x38, 0x9F, 0xE6, 0x78, 0xCA, 0xF7, 0xC6, 0xF8, 0x60, 0xD5, 0xBB, 0x9C, 0x4F},
        {0xF3, 0x3C, 0x65, 0x7B, 0x63, 0x7C, 0x30, 0x6A, 0xDD, 0x4E, 0xA7, 0x79, 0x9E, 0xB2, 0x3D, 0x31},
        {0x3E, 0x98, 0xB5, 0x6E, 0x27, 0xD3, 0xBC, 0xCF, 0x59, 0x1E, 0x18, 0x1F, 0x4C, 0x5A, 0xB7, 0x93},
        {0xE9, 0xDE, 0xE7, 0x2C, 0x8F, 0x0C, 0x0F, 0xA6, 0x2D, 0xDB, 0x49, 0xF4, 0x6F, 0x73, 0x96, 0x47},
        {0x06, 0x07, 0x53, 0x16, 0xED, 0x24, 0x7A, 0x37, 0x39, 0xCB, 0xA3, 0x83, 0x03, 0xA9, 0x8B, 0xF6},
        {0x92, 0xBD, 0x9B, 0x1C, 0xE5, 0xD1, 0x41, 0x01, 0x54, 0x45, 0xFB, 0xC9, 0x5E, 0x4D, 0x0E, 0xF2},
        {0x68, 0x20, 0x80, 0xAA, 0x22, 0x7D, 0x64, 0x2F, 0x26, 0x87, 0xF9, 0x34, 0x90, 0x40, 0x55, 0x11},
        {0xBE, 0x32, 0x97, 0x13, 0x43, 0xFC, 0x9A, 0x48, 0xA0, 0x2A, 0x88, 0x5F, 0x19, 0x4B, 0x09, 0xA1},
        {0x7E, 0xCD, 0xA4, 0xD0, 0x15, 0x44, 0xAF, 0x8C, 0xA5, 0x84, 0x50, 0xBF, 0x66, 0xD2, 0xE8, 0x8A},
        {0xA2, 0xD7, 0x46, 0x52, 0x42, 0xA8, 0xDF, 0xB3, 0x69, 0x74, 0xC5, 0x51, 0xEB, 0x23, 0x29, 0x21},
        {0xD4, 0xEF, 0xD9, 0xB4, 0x3A, 0x62, 0x28, 0x75, 0x91, 0x14, 0x10, 0xEA, 0x77, 0x6C, 0xDA, 0x1D}};

class Stb {
private:
    string password;
    string text;
    vector<vector<int>> sub_keys;

    static long long get_data_from_H_table(string val) {
        if (val[0] != '0' && val[1] != 'x') {
            val = Utils::string_slice(val, 2);
        }

        while ((val[0] == '0' || val[0] == 'x') && val.size() > 2) {
            val = Utils::string_slice(val, 1);
        }

        if (val.size() > 2) {
            throw HTableOutOfRange();
        } else if (val.size() == 1) {
            auto i = std::stoi("0x0", nullptr, 16);
            auto j = std::stoi(val, nullptr, 16);

            return H_TABLE[i][j];
        }

        vector<string> data = Utils::n_split_string(val, 1);
        auto i = std::stoi(data[0], nullptr, 16);
        auto j = std::stoi(data[1], nullptr, 16);
        return H_TABLE[i][j];
    }

    static vector<int> plus_mod_32(vector<int> a, vector<int> b) {
        auto int_a = Utils::byte_to_long(a);
        auto int_b = Utils::byte_to_long(b);

        auto int_result = Utils::inf_int_to_long_num(Utils::add_inf_int(int_a, int_b) % 4294967296);
        auto result = Utils::long_long_to_bin_value_num(int_result, 32);

        return result;
    }

    static vector<int> minus_mod_32(vector<int> a, vector<int> b) {
        auto int_a = Utils::byte_to_long(a);
        auto int_b = Utils::byte_to_long(b);

        auto int_result = Utils::inf_int_to_long_num(Utils::sub_inf_int(int_a, int_b) % 4294967296);

        if (int_result < 0) {
            int_result = 4294967296 + int_result;
        }

        auto result = Utils::long_long_to_bin_value_num(int_result, 32);
        return result;
    }

    static vector<int> g_transition(vector<int> data, int num) {
        auto int_data = Utils::n_split(data, 8);
        auto new_data = vector<int>();

        for (const auto &val : int_data) {
            auto int_val = Utils::byte_to_int(val);
            auto hex_val = Utils::int_to_hex(int_val);

            auto H = Stb::get_data_from_H_table(hex_val);
            new_data.push_back(H);
        }

        auto new_data_bin = vector<int>();
        for (auto i = 0; i < new_data.size(); i++) {
            auto temp = Utils::int_bin_value_num(new_data[i], 8);
            for (auto j = 0; j < temp.size(); j++) {
                new_data_bin.push_back(temp[j]);
            }
        }

        auto int_s = Utils::byte_to_long(new_data_bin);
        int_s = ((int_s >> (32 - num)) | (int_s << num)) & 0xFFFFFFFF;
        auto result = Utils::long_long_to_bin_value_num(int_s, 32);

        return result;
    }

public:
    Stb(string password, string text) {
        this->password = std::move(password);
        this->text = std::move(text);

        auto encode_key = Utils::string_to_bit_array(this->password);
        this->sub_keys = Utils::n_split(encode_key, 32);
    }

    string run_encode() {
        auto raw_text_bits = Utils::string_to_bit_array(this->text);

        if (raw_text_bits.size() != 128) {
            throw InvalidTextBlockSize(128);
        }

        auto raw_splitted_text = Utils::n_split(raw_text_bits, 32);

        auto a = raw_splitted_text[0];
        auto b = raw_splitted_text[1];
        auto c = raw_splitted_text[2];
        auto d = raw_splitted_text[3];

        auto all_keys = vector<vector<int>>();

        for (auto i = 0; i < 7; i++) {
            auto copy = Utils::vector_copy(this->sub_keys);
            for (const auto &j : copy) {
                all_keys.push_back(j);
            }
        }

        for (auto i = 1; i < 9; i++) {
            auto sub_key = Utils::vector_copy(all_keys[7 * i - 6 - 1]);
            auto temp = Stb::plus_mod_32(a, sub_key);
            temp = Stb::g_transition(temp, 5);
            b = Utils::vector_xor(b, temp);

            sub_key = Utils::vector_copy(all_keys[7 * i - 5 - 1]);
            temp = Stb::plus_mod_32(d, sub_key);
            temp = Stb::g_transition(temp, 21);
            c = Utils::vector_xor(c, temp);

            sub_key = Utils::vector_copy(all_keys[7 * i - 4 - 1]);
            temp = Stb::plus_mod_32(b, sub_key);
            temp = Stb::g_transition(temp, 13);
            a = Stb::minus_mod_32(a, temp);

            sub_key = Utils::vector_copy(all_keys[7 * i - 3 - 1]);
            temp = Stb::plus_mod_32(b, c);
            temp = Stb::plus_mod_32(temp, sub_key);
            temp = Stb::g_transition(temp, 21);

            auto val_i_int = i % 4294967296;
            auto val_i = Utils::long_long_to_bin_value_num(val_i_int, 32);
            auto e = Utils::vector_xor(temp, val_i);

            b = Stb::plus_mod_32(b, e);
            c = Stb::minus_mod_32(c, e);

            sub_key = Utils::vector_copy(all_keys[7 * i - 2 - 1]);
            temp = Stb::plus_mod_32(c, sub_key);
            temp = Stb::g_transition(temp, 13);
            d = Stb::plus_mod_32(d, temp);

            sub_key = Utils::vector_copy(all_keys[7 * i - 1 - 1]);
            temp = Stb::plus_mod_32(a, sub_key);
            temp = Stb::g_transition(temp, 21);
            b = Utils::vector_xor(b, temp);

            sub_key = Utils::vector_copy(all_keys[7 * i - 1]);
            temp = Stb::plus_mod_32(d, sub_key);
            temp = Stb::g_transition(temp, 5);
            c = Utils::vector_xor(c, temp);

            auto tmp = Utils::vector_copy(a);
            a = Utils::vector_copy(b);
            b = Utils::vector_copy(tmp);

            tmp = Utils::vector_copy(d);
            d = Utils::vector_copy(c);
            c = Utils::vector_copy(tmp);

            tmp = Utils::vector_copy(b);
            b = Utils::vector_copy(c);
            c = Utils::vector_copy(tmp);
        }

        auto encoded_text = Utils::vector_concat(Utils::vector_concat(b, d), Utils::vector_concat(a, c));
        auto result = Utils::bit_array_to_string(encoded_text);
        return result;
    }

    string run_decode() {
        auto raw_text_bits = Utils::string_to_bit_array(this->text);

        if (raw_text_bits.size() != 128) {
            throw InvalidTextBlockSize(128);
        }

        auto raw_splitted_text = Utils::n_split(raw_text_bits, 32);

        auto a = raw_splitted_text[0];
        auto b = raw_splitted_text[1];
        auto c = raw_splitted_text[2];
        auto d = raw_splitted_text[3];

        auto all_keys = vector<vector<int>>();

        for (auto i = 0; i < 7; i++) {
            auto copy = Utils::vector_copy(this->sub_keys);
            for (const auto &j : copy) {
                all_keys.push_back(j);
            }
        }

        for (auto i = 8; i > 0; i--) {
            auto sub_key = Utils::vector_copy(all_keys[7 * i - 1]);
            auto temp = Stb::plus_mod_32(a, sub_key);
            temp = Stb::g_transition(temp, 5);
            b = Utils::vector_xor(b, temp);

            sub_key = Utils::vector_copy(all_keys[7 * i - 1 - 1]);
            temp = Stb::plus_mod_32(d, sub_key);
            temp = Stb::g_transition(temp, 21);
            c = Utils::vector_xor(c, temp);

            sub_key = Utils::vector_copy(all_keys[7 * i - 2 - 1]);
            temp = Stb::plus_mod_32(b, sub_key);
            temp = Stb::g_transition(temp, 13);
            a = Stb::minus_mod_32(a, temp);

            sub_key = Utils::vector_copy(all_keys[7 * i - 3 - 1]);
            temp = Stb::plus_mod_32(b, c);
            temp = Stb::plus_mod_32(temp, sub_key);
            temp = Stb::g_transition(temp, 21);

            auto val_i_int = i % 4294967296;
            auto val_i = Utils::long_long_to_bin_value_num(val_i_int, 32);
            auto e = Utils::vector_xor(temp, val_i);

            b = Stb::plus_mod_32(b, e);
            c = Stb::minus_mod_32(c, e);

            sub_key = Utils::vector_copy(all_keys[7 * i - 4 - 1]);
            temp = Stb::plus_mod_32(c, sub_key);
            temp = Stb::g_transition(temp, 13);
            d = Stb::plus_mod_32(d, temp);

            sub_key = Utils::vector_copy(all_keys[7 * i - 5 - 1]);
            temp = Stb::plus_mod_32(a, sub_key);
            temp = Stb::g_transition(temp, 21);
            b = Utils::vector_xor(b, temp);

            sub_key = Utils::vector_copy(all_keys[7 * i - 6 - 1]);
            temp = Stb::plus_mod_32(d, sub_key);
            temp = Stb::g_transition(temp, 5);
            c = Utils::vector_xor(c, temp);

            auto tmp = Utils::vector_copy(a);
            a = Utils::vector_copy(b);
            b = Utils::vector_copy(tmp);

            tmp = Utils::vector_copy(d);
            d = Utils::vector_copy(c);
            c = Utils::vector_copy(tmp);

            tmp = Utils::vector_copy(d);
            d = Utils::vector_copy(a);
            a = Utils::vector_copy(tmp);
        }

        auto decoded_text = Utils::vector_concat(Utils::vector_concat(c, a), Utils::vector_concat(d, b));
        auto result = Utils::bit_array_to_string(decoded_text);
        return result;
    }

    string run_encode_simple_change() {
        if (this->text.size() % 16 != 0) {
            throw InvalidTextBlockSize(16);
        }

        auto encoded_text = vector<string>();
        auto raw_text_splitted = Utils::n_split_string(this->text, 16);
        for (const auto & part: raw_text_splitted) {
            encoded_text.push_back(Stb::encrypt(this->password, part));
        }

        return Utils::join_strings(encoded_text);
    }

    string run_decode_simple_change() {
        if (this->text.size() % 16 != 0) {
            throw InvalidTextBlockSize(16);
        }

        auto decoded_text = vector<string>();
        auto raw_text_splitted = Utils::n_split_string(this->text, 16);
        for (const auto & part: raw_text_splitted) {
            decoded_text.push_back(Stb::decrypt(this->password, part));
        }

        return Utils::join_strings(decoded_text);
    }

    string run_encode_clutch_blocks(string synchrosend) {
        if (this->text.size() % 16 != 0) {
            throw InvalidTextBlockSize(16);
        }

        auto encoded_text = vector<string>();
        auto raw_text_splitted = Utils::n_split_string(this->text, 16);
        auto temp = Stb::encrypt(this->password, synchrosend);
        for (const auto & part: raw_text_splitted) {
            auto res = Utils::bit_array_to_string(
                    Utils::vector_xor(
                    Utils::string_to_bit_array(temp),
                    Utils::string_to_bit_array(part))
            );

            auto encoded_part = Stb::encrypt(this->password, res);
            temp = encoded_part;
            encoded_text.push_back(encoded_part);
        }

        return Utils::join_strings(encoded_text);
    }

    string run_decode_clutch_blocks(string synchrosend) {
        if (this->text.size() % 16 != 0) {
            throw InvalidTextBlockSize(16);
        }

        auto decoded_text = vector<string>();
        auto raw_text_splitted = Utils::n_split_string(this->text, 16);
        auto temp = Stb::encrypt(this->password, synchrosend);
        for (const auto & part: raw_text_splitted) {
            auto res = Utils::bit_array_to_string(
                    Utils::vector_xor(
                            Utils::string_to_bit_array(Stb::decrypt(this->password, part)),
                            Utils::string_to_bit_array(temp))
            );

            decoded_text.push_back(res);
            temp = part;
        }

        return Utils::join_strings(decoded_text);
    }

    string run_encode_gamma_with_feedback(string synchrosend) {
        if (this->text.size() % 16 != 0) {
            throw InvalidTextBlockSize(16);
        }

        auto encoded_text = vector<string>();
        auto raw_text_splitted = Utils::n_split_string(this->text, 16);
        auto temp = synchrosend;
        for (const auto & part: raw_text_splitted) {
            auto enc_temp = Stb::encrypt(this->password, temp);
            auto enc_sliced = Utils::string_slice(enc_temp, 0, part.size());

            auto res = Utils::bit_array_to_string(
                    Utils::vector_xor(
                            Utils::string_to_bit_array(part),
                            Utils::string_to_bit_array(enc_sliced))
            );

            encoded_text.push_back(res);
            temp = res;
        }

        return Utils::join_strings(encoded_text);
    }

    string run_decode_gamma_with_feedback(string synchrosend) {
        if (this->text.size() % 16 != 0) {
            throw InvalidTextBlockSize(16);
        }

        auto decoded_text = vector<string>();
        auto raw_text_splitted = Utils::n_split_string(this->text, 16);
        auto temp = synchrosend;
        for (const auto & part: raw_text_splitted) {
            auto enc_temp = Stb::encrypt(this->password, temp);
            auto enc_sliced = Utils::string_slice(enc_temp, 0, part.size());

            auto res = Utils::bit_array_to_string(
                    Utils::vector_xor(
                            Utils::string_to_bit_array(part),
                            Utils::string_to_bit_array(enc_sliced))
            );

            decoded_text.push_back(res);
            temp = part;
        }

        return Utils::join_strings(decoded_text);
    }

    string run_encode_with_counter(string synchrosend) {
        if (this->text.size() % 16 != 0) {
            throw InvalidTextBlockSize(16);
        }

        auto encoded_text = vector<string>();
        auto raw_text_splitted = Utils::n_split_string(this->text, 16);

        auto temp = Stb::encrypt(this->password, synchrosend);
        auto ones = vector<int>();
        for (auto i = 0; i < 128; i++) {
            ones.push_back(1);
        }

        for (const auto & part: raw_text_splitted) {
            InfInt int_a = Utils::byte_to_inf_long(Utils::string_to_bit_array(temp));
            InfInt int_b = Utils::byte_to_inf_long(ones);
            InfInt sum_a_b = int_a + int_b;
            InfInt mod = sum_a_b % 4294967296;

            auto int_result = Utils::inf_int_to_long_num(mod);
            auto plus_mode = Utils::long_long_to_bin_value_num(int_result, 32);
            auto s = vector<int>();
            for (auto const & item: plus_mode) {
                auto byte = Utils::int_bin_value_num(item, 8);
                for (auto const & bit: byte) {
                    s.push_back(bit);
                }

                if (s.size() >= 128) {
                    break;
                }
            }

            temp = Utils::bit_array_to_string(s);
            temp = Stb::encrypt(this->password, temp);
            auto sliced_s = Utils::string_slice(temp, 0, part.size());

            auto res = Utils::bit_array_to_string(
                    Utils::vector_xor(
                            Utils::string_to_bit_array(part),
                            Utils::string_to_bit_array(sliced_s))
            );

            encoded_text.push_back(res);
        }

        return Utils::join_strings(encoded_text);
    }

    string run_decode_with_counter(string synchrosend) {
        return Stb::encrypt_with_counter(this->password, this->text, synchrosend);
    }

    static string encrypt(string password, string text) {
        return Stb(password, text).run_encode();
    }

    static string decrypt(string password, string text) {
        return Stb(password, text).run_decode();
    }

    static string encrypt_clutch_blocks(string password, string text, string synchrosend) {
        return Stb(password, text).run_encode_clutch_blocks(synchrosend);
    }

    static string decrypt_clutch_blocks(string password, string text, string synchrosend) {
        return Stb(password, text).run_decode_clutch_blocks(synchrosend);
    }

    static string encrypt_simple_change(string password, string text) {
        return Stb(password, text).run_encode_simple_change();
    }

    static string decrypt_simple_change(string password, string text) {
        return Stb(password, text).run_decode_simple_change();
    }

    static string encrypt_gamma_with_feedback(string password, string text, string synchrosend) {
        return Stb(password, text).run_encode_gamma_with_feedback(synchrosend);
    }

    static string decrypt_gamma_with_feedback(string password, string text, string synchrosend) {
        return Stb(password, text).run_decode_gamma_with_feedback(synchrosend);
    }

    static string encrypt_with_counter(string password, string text, string synchrosend) {
        return Stb(password, text).run_encode_with_counter(synchrosend);
    }

    static string decrypt_with_counter(string password, string text, string synchrosend) {
        return Stb(password, text).run_decode_with_counter(synchrosend);
    }
};