#include <random>

int random_int(int min_val, int max_val) {
    static int stored_min_val = min_val;
    static int stored_max_val = max_val;
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution dist(stored_min_val, stored_max_val);

    if (!(min_val == stored_min_val && max_val == stored_max_val)) {
        stored_min_val = min_val;
        stored_max_val = max_val;
        dist = std::uniform_int_distribution(stored_min_val, stored_max_val);
    }

    return dist(gen);
}
