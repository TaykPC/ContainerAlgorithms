#include <iostream>
#include <iomanip>
#include <windows.h>

#include "demo.h"

// added as preproccessor macro
// #define SOLUTION_DIR=R"($(SolutionDir))"

int main() {
    // Versuch, die Windows-Konsole zur Kooperation zu überreden (funktioniert nur teilweise)
    SetConsoleOutputCP(CP_UTF8);

    // Pfad muss an lokale Verhältnisse angepasst werden
    //const std::string path_to_tolstoi_book{ R"(I:\Tolstoi__Krieg und Frieden_UTF8.txt)" };
    const std::string bookname = { R"(Tolstoi__Krieg und Frieden_UTF8.txt)" };
    const std::string path_to_tolstoi_book{ SOLUTION_DIR + bookname };

    try {
        Demo::runTolstoi(path_to_tolstoi_book);
        Demo::runTolstoiRegex(path_to_tolstoi_book);
    }
    catch (std::exception & e) {
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
    }

    std::cin.ignore();
}
