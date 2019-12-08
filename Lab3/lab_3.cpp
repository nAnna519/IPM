class LGamal {
private:
	string password;
	string text;

	static vector<long long> generate_keys(int p) {
		long long g = Utils::eiler_num_generator(p);
		long long x = Utils::random_in_range(2, p - 1);
		long long y = Utils::inf_int_to_num(Utils::pow_inf_int(g, x) % p);

		auto result = vector<long long>();
		result.push_back(x);
		result.push_back(y);
		result.push_back(g);

		return result;
	}

public:
	LGamal(string password, string text) {
		this->password = std::move(password);
		this->text = std::move(text);
	}

	vector<long long> encrypt_num(long long p, long long m) {
		if (!Utils::is_prime(p)) {
			throw InvalidPPrime();
		}

		if (m > p) {
			throw InvalidPandMessageLength();
		}

		auto keys = LGamal::generate_keys(p);

		auto x = keys[0];
		auto y = keys[1];
		auto g = keys[2];

		auto k = Utils::random_in_range(2, (p - 1) - 1);

		long long a = Utils::inf_int_to_num(Utils::pow_inf_int(g, k) % p);
		long long b = Utils::inf_int_to_num((Utils::pow_inf_int(y, k) * m) % p);

		auto result = vector<long long>();
		result.push_back(a);
		result.push_back(b);
		result.push_back(x);

		return result;
	}


	long long decrypt_num(long long x, long long p, long long a, long long b) {
		auto m = Utils::inf_int_to_num((Utils::pow_inf_int(a, p - 1 - x) * b) % p);
		return m;
	}

	vector<vector<long long>> run_encode(long long p) {
		auto result = vector<vector<long long>>();
		for (auto i = 0; i < this->text.size(); i++) {
			auto tmp = this->encrypt_num(p, (long long)this->text[i]);
			result.push_back(tmp);
		}

		return result;
	}

	string run_decode(vector<vector<long long>> text, long long p) {
		string result = "";

		for (auto i = 0; i < text.size(); i++) {
			auto a = text[i][0];
			auto b = text[i][1];
			auto x = text[i][2];
			char tmp = this->decrypt_num(x, p, a, b);

			result = result + tmp;
		}

		return result;
	}

	static vector<vector<long long>> encrypt(string password, string text, long long p) {
		return LGamal(password, text).run_encode(p);
	}

	static string decrypt(string password, vector<vector<long long>> text, long long p) {
		return LGamal(password, "").run_decode(text, p);
	}
};