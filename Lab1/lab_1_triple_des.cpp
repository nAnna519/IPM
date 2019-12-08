class TripleDes {
public:
    static string encrypt(string password1, string password2, string text, bool padding = false) {
        auto encrypted_data = Des::encrypt(password1, text, padding);
        auto encrypted_double_data = Des::decrypt(password2, encrypted_data, false);
        auto encrypted_triple_data = Des::encrypt(password1, encrypted_double_data, padding);

        return encrypted_triple_data;
    }

    static string decrypt(string password1, string password2, string text, bool padding = false) {
        auto decrypted_data = Des::decrypt(password1, text, padding);
        auto decrypted_double_data = Des::encrypt(password2, decrypted_data, false);
        auto decrypted_triple_data = Des::decrypt(password1, decrypted_double_data, padding);

        return decrypted_triple_data;
    }
};