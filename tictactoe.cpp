#include <iostream>
#include <utility>
#include <string>
#include <cctype>

#ifdef _WIN32
    #include <windows.h>
    void sleep_ms(int milliseconds) {
        Sleep(milliseconds);  
    }
    void clear_screen(){
        system("cls");
    }
#elif defined(__linux__)
    #include <unistd.h>
    void sleep_ms(int milliseconds) {
        usleep(milliseconds * 1000); 
    }
    void clear_screen(){
        system("clear");
    }
#else
    #error "Unsupported operating system"
#endif

const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string RESET = "\033[0m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string YELLOW = "\033[33m";

char board[7][15] = {"+---+---+---+\n",
                     "| 1 | 2 | 3 |\n",
                     "+---+---+---+\n",
                     "| 4 | 5 | 6 |\n",
                     "+---+---+---+\n",
                     "| 7 | 8 | 9 |\n",
                     "+---+---+---+\n"};

bool posAvailable(int row, int col){
    if(board[row][col] ==  'X' || board[row][col] == 'O') return 0;
    return 1;
}

void printBoard(std::pair<int, int> winPos1 = {-1, -1}, std::pair<int, int> winPos2 = {-1, -1}, std::pair<int, int> winPos3 = {-1, -1}) {
    clear_screen();
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 15; j++) {
            std::pair<int, int> currentPos = {i, j};
            if (currentPos == winPos1 || currentPos == winPos2 || currentPos == winPos3) {
                if (board[i][j] == 'X') {
                    std::cout << YELLOW << 'X' << RESET;
                } else if (board[i][j] == 'O') {
                    std::cout << YELLOW << 'O' << RESET;
                }
            } else {
                if (board[i][j] == 'X') {
                    std::cout << RED << 'X' << RESET;
                } else if (board[i][j] == 'O') {
                    std::cout << GREEN << 'O' << RESET;
                } else if(board[i][j] == '+'){
                    std::cout << MAGENTA << '+' << RESET;
                }else if(board[i][j] == '-'){
                    std::cout << MAGENTA << '-' << RESET;
                }else if(board[i][j] == '|'){
                    std::cout << MAGENTA << '|' << RESET;
                }else{
                    std::cout << CYAN << board[i][j] << RESET;
                }
            }
        }
    }
}

std::pair<bool, std::pair<int, int>> verifyRow_Win(int row){
    char value = board[row][2];
    if(board[row][6] == value && board[row][10] == value) return std::make_pair(1, std::make_pair(row, 2));
    return std::make_pair(0, std::make_pair(-1, -1));
}
std::pair<bool, std::pair<int, int>> verifyCol_Win(int col){
    char value = board[1][col];
    if(board[3][col] == value && board[5][col] == value) return std::make_pair(1, std::make_pair(1, col));
    return std::make_pair(0, std::make_pair(-1, -1));
}
std::pair<bool, std::pair<int, int>> verify_firstDiag_Win(){
    char value = board[1][2];
    if(board[3][6] == value && board[5][10] == value) return std::make_pair(1, std::make_pair(1, 2));
    return std::make_pair(0, std::make_pair(-1, -1));
}
std::pair<bool, std::pair<int, int>> verify_secondDiag_Win(){
    char value = board[1][10];
    if(board[3][6] == value && board[5][2] == value) return std::make_pair(1, std::make_pair(1, 10));
    return std::make_pair(0, std::make_pair(-1, -1));
}

std::pair<bool, std::pair<std::pair<int, int>, std::pair<int, int>>> verifyAllPossibleWins(){
    std::pair<bool, std::pair<int, int>> result;

    result = verifyRow_Win(1);
    if(result.first){
        return std::make_pair(1, std::make_pair(result.second, std::make_pair(1, 10)));
    }

    result = verifyRow_Win(3);
    if(result.first){
        return std::make_pair(1, std::make_pair(result.second, std::make_pair(3, 10)));
    }

    result = verifyRow_Win(5);
    if(result.first){
        return std::make_pair(1, std::make_pair(result.second, std::make_pair(5, 10)));
    }

    result = verifyCol_Win(2);
    if(result.first){
        return std::make_pair(1, std::make_pair(result.second, std::make_pair(5, 2)));
    }

    result = verifyCol_Win(6);
    if(result.first){
        return std::make_pair(1, std::make_pair(result.second, std::make_pair(5, 6)));
    }

    result = verifyCol_Win(10);
    if(result.first){
        return std::make_pair(1, std::make_pair(result.second, std::make_pair(5, 10)));
    }

    result = verify_firstDiag_Win();
    if(result.first){
        return std::make_pair(1, std::make_pair(result.second, std::make_pair(5, 10)));
    }

    result = verify_secondDiag_Win();
    if(result.first){
        return std::make_pair(1, std::make_pair(result.second, std::make_pair(5, 2)));
    }

    return std::make_pair(0, std::make_pair(std::make_pair(-1, -1), std::make_pair(-1, -1)));
}

int main(){
    std::string input;
    int move;
    bool Player = true;
    int availablePosition = 9;
    while(true && availablePosition > 0){

        auto finalResults = verifyAllPossibleWins();

        if(finalResults.first){
            printBoard(finalResults.second.first, finalResults.second.second, std::make_pair((finalResults.second.first.first + finalResults.second.second.first) / 2, (finalResults.second.first.second + finalResults.second.second.second) / 2));
            std::cout << "The winner is \033[34m" << (Player ? 'O' : 'X') << RESET << " !\n";
            break;
        }

        printBoard();

        std::cout<<"Enter position ( 0 - exit ) : ";
        std::cin>> input;

        bool isNumber = true;

        for(char c : input){
            if(!isdigit(c)){
                isNumber = false;
                break;
            }
        }

        if(!isNumber){
            std::cout << "Invalid input: [1 - 9] or 0!\n";
            sleep_ms(1000);
            continue;
        }

        move = std::stoi(input);

        if(move < 0 || move > 9){
            std::cout << "Invalid input: [1 - 9] or 0!\n";
            sleep_ms(1000);
            continue;
        }

        if(move == 0){ break; }
        else{
            int row = 2 * ((move - 1) / 3 ) + 1;
            int col = 4 * ((move - 1) % 3) + 2;
            if(posAvailable(row, col)){
                board[row][col] = Player ? 'X' : 'O';
                Player = !Player;
                availablePosition--;
            }
        }

    }
    if(availablePosition == 0){
        auto finalResults = verifyAllPossibleWins();
        if(finalResults.first){
            printBoard(finalResults.second.first, finalResults.second.second, std::make_pair((finalResults.second.first.first + finalResults.second.second.first) / 2, (finalResults.second.first.second + finalResults.second.second.second) / 2));
            std::cout << "The winner is \033[34m" << (Player ? 'O' : 'X') << RESET << " !\n";
            return 0;
        }
        printBoard();
        std::cout << "Draw !\n";
    }
    return 0;
}