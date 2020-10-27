#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <regex>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iterator>
#include <functional>
#include "utility.h"


using std::cout, std::endl;


template<typename InputIt>
void fill_randomly(InputIt first, InputIt last, int max_val = 10) { // Iteratoren für die Bereichsangabe
    while (first != last)
    *first++ = random_int(0, max_val);      // aus utility.h / utility.cpp
    // *first++ = std::rand() % (max_val + 1);
}


namespace Demo {
    void runTolstoi(const std::string& path_to_tolstoi_book) {
        std::ifstream ifstream_book{ path_to_tolstoi_book };

        if (!ifstream_book) {
            std::cerr << "Buch nicht gefunden" << endl;
            exit(1);
        }

        std::vector<std::string> book_vec;

        std::string line;
        while (std::getline(ifstream_book, line))
            book_vec.push_back(std::move(line));

        cout << "Tolstoi gelesen: " << book_vec.size() << " Zeilen" << endl;

        cout << "Ein Fragment daraus:\n" << std::setfill('-') << std::setw(80) << '-' << endl;
        for (int i = 100; i <= 106; ++i)
            std::cout << book_vec[i] << std::endl;
        cout << std::setw(80) << '-' << "\n\n" << endl;

        cout << std::setfill(' ');  // zurücksetzen für spätere Ausgaben

        std::vector<std::string> words_vec;
        {
            // isalpha() erkennt die Umlaute nicht, aber die strchr-Lsg. ist auch nicht sauber.
            // Die Umlaute werden in UTF-8 mit 2 Bytes dargestellt, was hier zwar funktioniert,
            // aber es könnte andere UTF-8-Zeichen mit dem gleichen 1. Byte geben, deren 2. Byte dann nicht mitgenommen wird
            // oder umgekehrt das gleiche 2. Byte haben, aber ein anderes 1. Byte.
            auto is_word_character = [](unsigned char c) { return std::isalpha(c) || std::strchr("äöüÄÖÜß", c); };

            for (const auto& line : book_vec) {
                auto pos = line.begin();

                while (pos != line.end()) {
                    auto word_start = std::find_if(pos, line.end(), is_word_character);
                    auto word_end = std::find_if_not(word_start, line.end(), is_word_character);

                    if (word_start != word_end)
                        words_vec.push_back(std::string{ word_start, word_end });

                    pos = word_end;
                }
            }
        }

        cout << "Wortvektor angelegt: " << words_vec.size() << " Worte" << endl;


        {
            cout << "\nSuche Phrase \"Es war eine dunkle \": " << endl;

            std::vector<std::string> search_phrase{ "Es", "war", "eine", "dunkle" };
            std::vector<size_t> positions;

            auto it = words_vec.begin();

            while ((it = std::search(it, words_vec.end(), search_phrase.begin(), search_phrase.end()))
                != words_vec.end()) {
                auto position = it - words_vec.begin();
                positions.push_back(position);
                cout << "Position " << position << ": ";
                print(it, it + 10);  // Fragment ausgeben
                ++it;
            }

            if (positions.size() >= 2) {
                cout << "\nErster Mismatch der Phrasen ab Position " << positions[0] << " und " << positions[1] << ": ";
                auto it_1_2 = std::mismatch(words_vec.begin() + positions[0], words_vec.end(),
                    words_vec.begin() + positions[1], words_vec.end());
                cout << *it_1_2.first << " vs. " << *it_1_2.second << endl;
            }
        }

        {
            cout << "\nSuche Phrase (nur Anfangsbuchstaben der Wörter: E w e d): " << endl;

            std::vector<std::string> search_phrase{ "Es", "war", "eine", "dunkle" };
            auto it = words_vec.begin();
            while ((it = std::search(it, words_vec.end(), search_phrase.begin(), search_phrase.end(),
                [](auto& w1, auto& w2) { return w1[0] == w2[0]; }))
                != words_vec.end()) {
                cout << "Position " << (it - words_vec.begin()) << ": ";
                print(it, it + 10);  // Fragment ausgeben
                ++it;
            }
        }

        {
            cout << "\nSuche benachbarte Wörter mit Längenunterschied von min. 20 Zeichen: " << endl;

            auto it = words_vec.begin();
            while ((it = std::adjacent_find(it, words_vec.end(),
                [](auto& w1, auto& w2) { return std::llabs(w1.size() - w2.size()) > 20; }))
                != words_vec.end()) {
                cout << "Position " << (it - words_vec.begin()) << ": ";
                print(it, it + 2);
                ++it;
            }
        }

        {
            size_t number_of_occurrences =
                std::count_if(words_vec.begin(), words_vec.end(),
                    [](auto& word) { return word.size() == 12; });

            cout << "\nEs gibt " << number_of_occurrences << " Wörter mit 12 Zeichen Länge (inkl. Duplikate)" << endl;

            std::set<std::string> word_set;
            std::copy_if(words_vec.begin(), words_vec.end(), std::inserter(word_set, word_set.end()),
                [](auto& word) { return word.size() == 12; });

            cout << "Es gibt " << word_set.size() << " unterschiedliche Wörter mit 12 Zeichen Länge" << endl;
        }

        {
            std::string search_word = "Rostow";
            size_t number_of_occurrences = std::count(words_vec.begin(), words_vec.end(), search_word);

            cout << "\nDer Name " << search_word << " kommt " << number_of_occurrences << "x vor" << endl;
        }


        {
            std::map<std::string, size_t> word_freq;
            for (auto& word : words_vec)
                ++word_freq[word];

            auto it = std::max_element(word_freq.begin(), word_freq.end(),
                [](const auto& kv1, const auto& kv2) { return kv1.second < kv2.second; });

            cout << "\nDas häufigste Wort ist \"" << it->first << "\". Es kommt " << it->second << "x vor" << endl;
        }

        {
            std::map<std::string, size_t> word_freq_map;
            for (auto& word : words_vec)
                ++word_freq_map[word];

            const int top_n = 25;

            std::vector<std::pair<std::string, size_t>> word_freq_vec{ word_freq_map.begin(), word_freq_map.end() };
            std::partial_sort(word_freq_vec.begin(), word_freq_vec.begin() + top_n, word_freq_vec.end(),
                [](const auto& kv1, const auto& kv2) { return kv1.second > kv2.second; }); // absteigende Reihenfolge

            cout << "\nDie " << top_n << " häufigsten Wörter sind:" << endl;
            for (int i = 0; i < top_n; ++i) {
                cout << std::setw(10) << word_freq_vec[i].first << ": " << word_freq_vec[i].second << "x" << endl;
            }
        }
    }


    void runTolstoiRegex(const std::string& path_to_tolstoi_book) {
        // Hinweis:
        //   Umlaute machen einige Probleme, weil sie in UTF-8-Kodierung zwei Bytes umfassen.
        //   Es müsste näher geprüft werden, ob mit std::locale-Einstellungen die regulären Ausdrücke "umlautfreundlich" werden
        //   oder ob z.B. nach UTF-16 umkodiert werden sollte.

        std::ifstream ifstream_book{ path_to_tolstoi_book };

        if (!ifstream_book) {
            std::cerr << "Buch nicht gefunden" << endl;
            exit(1);
        }

        std::string book;

        {
            std::string line;
            while (std::getline(ifstream_book, line)) {
                book += std::move(line);
                book.push_back('\n');
            }
        }

        cout << "Tolstoi gelesen: " << book.size() << " Zeichen" << endl;

        auto print_passage = [&book](size_t position) {
            cout << "Position " << position << ": ";
            auto start_it = book.begin() + position;
            auto stop_it = std::find(start_it, book.end(), '\n');
            if (stop_it - start_it > 80)
                stop_it = start_it + 80;
            //      std::copy(book.begin() + position, stop_it, std::ostream_iterator<char>(cout));  // UTF-8-Probleme in Windowskonsole bei Einzelzeichenausgabe :-(
            std::string out_string{ start_it, stop_it };
            cout << out_string << endl;
        };

        {
            cout << "\nSuche Phrase \"Es war eine dunkle \": " << endl;

            std::regex pattern{ "Es war eine dunkle" };
            auto it = std::sregex_iterator(book.begin(), book.end(), pattern);
            auto end_it = std::sregex_iterator{};
            for (; it != end_it; ++it)
                print_passage(it->position());
        }

        {
            cout << "\nSuche Phrase (nur Anfangsbuchstaben der Wörter: E w e d): " << endl;

            std::regex pattern{ R"(E\S+\s+w\S+\s+e\S+\s+d)" };
            auto it = std::sregex_iterator(book.begin(), book.end(), pattern);
            auto end_it = std::sregex_iterator{};
            for (; it != end_it; ++it)
                print_passage(it->position());
        }

        {
            cout << "\nSuche Passagen mit drei Worten zwischen Interpunktion: " << endl;

            std::regex pattern{ R"([[:punct:]]( \w+){3}\s*(?=[[:punct:]]))" };
            auto it = std::sregex_iterator(book.begin(), book.end(), pattern);
            auto end_it = std::sregex_iterator{};
            for (int count = 1; it != end_it && count <= 10; ++it, ++count)
                print_passage(it->position());
            if (it != end_it)
                cout << "... und weitere Treffer ..." << endl;
        }

        {
            cout << "\nSuche Kombinationen von Wörtern mit Länge 5 -> 6 -> 5 -> 7: " << endl;

            std::regex pattern{ R"(\b\w{5}\W+\w{6}\W+\w{5}\W+\w{7}\b)" };
            auto it = std::sregex_iterator(book.begin(), book.end(), pattern);
            auto end_it = std::sregex_iterator{};
            for (int count = 1; it != end_it && count <= 10; ++it, ++count)
                print_passage(it->position());
            if (it != end_it)
                cout << "... und weitere Treffer ..." << endl;
        }

        {
            cout << "\nSuche Wortwiederholung nach 2-4 Zwischenwörtern: " << endl;

            std::regex pattern{ R"(\b(\w+)(\W+\w+){2,4}\W+(\1)\b)" };
            auto it = std::sregex_iterator(book.begin(), book.end(), pattern);
            auto end_it = std::sregex_iterator{};
            for (int count = 1; it != end_it && count <= 10; ++it, ++count)
                print_passage(it->position());
            if (it != end_it)
                cout << "... und weitere Treffer ..." << endl;
        }
    }
} // namespace Demo

