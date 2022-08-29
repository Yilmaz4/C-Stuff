#pragma once

#include <iostream>
#include <cmath>
#include <exception>
#include <windows.h>
#include <map>
#include <limits>
#include <conio.h>
#include <algorithm>
#include <cctype>

#undef max
#undef min

void clear_console() {
	COORD tl = { 0,0 };
	CONSOLE_SCREEN_BUFFER_INFO s;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(console, &s);
	DWORD written, cells = s.dwSize.X * s.dwSize.Y;
	FillConsoleOutputCharacter(console, ' ', cells, tl, &written);
	FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
	SetConsoleCursorPosition(console, tl);
}

void pause() {
	std::wcout << L"Press any key to continue . . . ";

	HANDLE hstdin;
	DWORD  mode;
	hstdin = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hstdin, &mode);
	SetConsoleMode(hstdin, ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);

	unsigned int ch = 10;
	while (ch == 10) {
		ch = std::wcin.get();
	}
	SetConsoleMode(hstdin, mode);
}

#define KEY_ENTER 10
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

#define X   +1
#define O   -1
#define TIE -2

typedef short Player;

template <typename type>
concept supports_comparison = requires(type obj1, type obj2) {
	{obj1 > obj2} noexcept -> std::convertible_to<bool>;
	{obj1 < obj2} noexcept -> std::convertible_to<bool>;
};

template <typename type> requires supports_comparison<type>
constexpr type min(type x, type y) noexcept {
	return ((x < y) ? x : y);
}

template <typename type> requires supports_comparison<type>
constexpr type max(type x, type y) noexcept {
	return ((x > y) ? x : y);
}

struct out_of_range : public std::exception {
	virtual const char* what() const noexcept {
		return "Matrix index out of range";
	}
};
struct game_not_over : public std::exception {
	virtual const char* what() const noexcept {
		return "The game is not over";
	}
};

std::map<short, short> scores{
	{X, +10},
	{O, -10},
	{TIE, 0}
};
std::map<short, wchar_t> charmap{
	{0, L' '}, {X, L'X'}, {O, L'O'}
};

class Board {
	signed short* const base = new signed short[3 * 3](NULL);
public:
	Board() {
		for (short i = 0; i < 9; i++) {
			base[i] = 0;
		}
	}
	Board(std::initializer_list<std::initializer_list<short>> matrix) {
		for (short i = 0; i < matrix.size() * matrix.begin()->size(); i++) {
			base[i] = *((matrix.begin() + static_cast<unsigned __int64>(floor(i / 3)))->begin() + (i % 3));
		}
	}
	~Board() {
		// delete[] base;
	}
	Board simulate(short x, short y, short val) const {
		Board tmp;
		for (short i = 0; i < 3; i++) {
			for (short j = 0; j < 3; j++) {
				tmp.set_cell(i, j, get_cell(i, j));
			}
		}
		tmp.set_cell(x, y, val);
		return tmp;
	}
	void reset() noexcept {
		for (short i = 0; i < 9; i++) {
			base[i] = 0;
		}
	}
	bool is_board_empty(short x, short y) const noexcept {
		for (short i = 0; i < 3; i++) {
			for (short j = 0; j < 3; j++) {
				if (!is_cell_empty(i, j)) {
					return false;
				}
			}
		}
		return true;
	}
	bool is_cell_empty(short x, short y) const noexcept {
		return !base[x * 3 + y];
	}
	short is_game_over() const noexcept {
		// Horizontal
		for (short x = 0; x < 3; x++) {
			short sum = 0;
			for (short y = 0; y < 3; y++) {
				sum += get_cell(x, y);
			}
			switch (sum) {
			case X * 3:
				return X;
			case O * 3:
				return O;
			}
		}
		// Vertical
		for (short y = 0; y < 3; y++) {
			short sum = 0;
			for (short x = 0; x < 3; x++) {
				sum += get_cell(x, y);
			}
			switch (sum) {
			case X * 3:
				return X;
			case O * 3:
				return O;
			}
		}
		// Diagonal
		short sum1 = 0;
		for (short x = 0, y = 0; x < 3 && y < 3; x++, y++) {
			sum1 += get_cell(x, y);
		}
		switch (sum1) {
		case X * 3:
			return X;
		case O * 3:
			return O;
		}
		// Anti-diagonal
		short sum2 = 0;
		for (short x = 2, y = 0; x >= 0 && y < 3; x--, y++) {
			sum2 += get_cell(x, y);
		}
		switch (sum2) {
		case X * 3:
			return X;
		case O * 3:
			return O;
		}

		// Check if the game is a tie
		for (short x = 0; x < 3; x++) {
			for (short y = 0; y < 3; y++) {
				if (!get_cell(x, y)) {
					return false;
				}
			}
		}
		return TIE;
	}
	short get_cell(short x, short y) const {
		if (x >= 3 || y >= 3)
			throw ::out_of_range();
		return *(base + (x * 3 + y));
	}
	void set_cell(short x, short y, short val) {
		if (x >= 3 || y >= 3)
			throw ::out_of_range();
		*(base + (x * 3 + y)) = val;
	}
	void render(short turn) const noexcept {
		COORD  position = { 0, 0 };
		HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(output, position);

		clear_console();

		std::wcout << L"    1   2   3" << std::endl << L"  ┌───┬───┬───┐" << std::endl << std::flush;
		for (short x = 0; x < 3; x++) {
			std::wcout << x + 1 << std::flush;
			for (short y = 0; y < 3; y++)
				std::wcout << L" │ " << charmap[get_cell(x, y)] << std::flush;
			std::wcout << L" │" << std::flush;
			if (x == 0)
				std::wcout << L"  Turn: " << charmap[turn] << std::endl << std::flush;
			else
				std::wcout << std::endl << std::flush;
			std::wcout << ((x != 2) ? L"  ├───┼───┼───┤" : L"  └───┴───┴───┘") << std::endl << std::flush;
		}
		std::wcout << std::flush;
	}
};

int minimax(Board board, uint64_t depth, Player player) {
	short winner = board.is_game_over();
	if (winner) {
		return scores[winner];
	}
	int best_score;
	best_score = ((player == O) ? std::numeric_limits<int>::max() : std::numeric_limits<int>::min());
	for (short i = 0; i < 3; i++) {
		for (short j = 0; j < 3; j++) {
			if (board.is_cell_empty(i, j)) {
				best_score = ((player == X) ? &max<int> : &min<int>)(minimax(board.simulate(i, j, player), depth + 1, player * -1), best_score);
			}
		}
	}
	return best_score;
}

class TicTacToe {
	Board board;
public:
	TicTacToe() {
		SetConsoleOutputCP(CP_UTF8);
		setvbuf(stdout, nullptr, _IOFBF, 1000);
		std::wcout.sync_with_stdio(false);
		std::wcout.imbue(std::locale("en_US.utf8"));

		SetCursor(LoadCursorW(NULL, IDC_HAND));

		while (true) {
			start();
		}
	}

	void start() {
		board.reset();
		clear_console();
		std::wcout << std::endl << std::wstring(5, '*') << L" TicTacToe "
			<< std::wstring(5, '*') << std::endl << std::endl << L"˃ Play" << std::endl
			<< L"  Settings" << std::endl << L"  Quit" << std::endl;

		unsigned int ch = 0, offset = 0;
		bool loop = true;
		while (loop) {
			ch = 0;
			HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
			switch (ch = _getch()) {
			case KEY_DOWN:
				SetConsoleCursorPosition(output, { 0, (short)(3 + offset) });
				std::wcout << (wchar_t)0x20 << std::flush;
				((offset != 2) ? offset++ : offset = 0);
				SetConsoleCursorPosition(output, { 0, (short)(3 + offset) });
				std::wcout << L"\r˃" << std::flush;
				SetConsoleCursorPosition(output, { 0, 6 });
				break;
			case KEY_UP:
				SetConsoleCursorPosition(output, { 0, (short)(3 + offset) });
				std::wcout << (wchar_t)0x20 << std::flush;
				((offset != 0) ? offset-- : offset = 2);
				SetConsoleCursorPosition(output, { 0, (short)(3 + offset) });
				std::wcout << L"\r˃" << std::flush;
				SetConsoleCursorPosition(output, { 0, 6 });
				break;
			case VK_RETURN:
				switch (offset) {
				case 0: // Play
					play();
					loop = false;
					break;
				case 1: // Settings
					settings();
					loop = false;
					break;
				case 2: // Quit
					exit(0);
				}
			}
		}
	}

	void play() noexcept {
		clear_console();
		unsigned int choice = 0;
		std::wcout << L"Who should start first? < X >" << std::flush;

		unsigned int ch = 0, val = X;
		bool loop = true;
		while (loop) {
			ch = 0;
			HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
			switch (ch = _getch()) {
			case KEY_RIGHT:
			case KEY_LEFT:
				val *= -1;
				SetConsoleCursorPosition(output, { 26, 0 });
				std::wcout << charmap[val] << std::flush;
				SetConsoleCursorPosition(output, { 29, 0 });
				break;
			case VK_RETURN:
				switch (val) {
				case X:
					board.render(O);
					make_move(true);
					__fallthrough;
				case O:
					board.render(O);
					std::wcout << L"Use arrow keys to navigate and Enter to place" << std::flush;
					short state = false;
					while (!(state = board.is_game_over())) {
						prompt();
					}
					SetConsoleCursorPosition(output, { 0, 9 });
					std::wcout << ((state != -2) ? (L"AI WINS! That was easy.") : L"TIE! You thought you could beat the AI? *laughs maniacally*")
						<< std::flush << std::endl;
					pause();
					loop = false;
				}
			}
		}
	}

	void settings() noexcept {
		clear_console();
		std::wcout << L"Settings" << std::endl << std::wstring(8, L'‾') << std::endl;
		std::wcout
			<< L"˃ Opponent" << (wchar_t)0x20 << "< AI >" << std::endl
			<< L"  Time limit for each move" << (wchar_t)0x20 << "< NONE >" << std::endl;
		int ch = 0, offset = 0;
		while (true) {
			ch = 0;
			HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
			switch (ch = _getch()) {
			case KEY_DOWN:
				SetConsoleCursorPosition(output, { 0, (short)(2 + offset) });
				std::wcout << (wchar_t)0x20 << std::flush;
				((offset != 1) ? offset++ : offset = 0);
				SetConsoleCursorPosition(output, { 0, (short)(2 + offset) });
				std::wcout << L"\r˃" << std::flush;
				SetConsoleCursorPosition(output, { 0, 6 });
				break;
			case KEY_UP:
				SetConsoleCursorPosition(output, { 0, (short)(2 + offset) });
				std::wcout << (wchar_t)0x20 << std::flush;
				((offset != 0) ? offset-- : offset = 1);
				SetConsoleCursorPosition(output, { 0, (short)(2 + offset) });
				std::wcout << L"\r˃" << std::flush;
				SetConsoleCursorPosition(output, { 0, 6 });
				break;
			case VK_RETURN:
				switch (offset) {
				case 0: // Play
					play();
					break;
				case 1: // Settings
					settings();
					return;
				case 2: // Quit
					exit(0);
				}
			}
		}
	}

	void make_move(bool maximizing) noexcept {
		int best_score = ((!maximizing) ? std::numeric_limits<int>::max() : std::numeric_limits<int>::min());
		short move[]{ -1, -1 };
		for (short i = 0; i < 3; i++) {
			for (short j = 0; j < 3; j++) {
				if (board.is_cell_empty(i, j)) {
					int score = minimax(board.simulate(i, j, X), 0, O);
					if (((maximizing) ? (best_score < score) : (score < best_score))) {
						move[0] = i; move[1] = j;
						best_score = score;
					}
				}
			}
		}
		board.set_cell(move[0], move[1], X);
	}
	void prompt(unsigned int starter = X) noexcept {
		unsigned int ch = 0, x = 0, y = 0;
		for (short a = 0; a < 3; a++) {
			bool fnd = false;
			for (short b = 0; b < 3; b++) {
				if (board.is_cell_empty(a, b)) {
					y = a;
					x = b;
					fnd = true;
					break;
				}
			}
			if (fnd) break;
		}
		HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(output, { (short)(x * 4 + 4), (short)(y * 2 + 2) });
		while (true) {
			ch = 0;
			switch (ch = _getch()) {
			case KEY_UP:
				for (; y > 0; y--) {
					if (!board.is_cell_empty(y - 1, x) && y == 1 || y == 2 && !board.is_cell_empty(y - 1, x) && !board.is_cell_empty(y - 2, x)) {
						break;
					}
					else if (!board.is_cell_empty(y - 1, x) && y != 1) {
						continue;
					}
					else {
						y--;
						break;
					}
				}
				SetConsoleCursorPosition(output, { (short)(x * 4 + 4), (short)(y * 2 + 2) });
				break;
			case KEY_DOWN:
				for (; y + 1 < 3; y++) {
					if (!board.is_cell_empty(y + 1, x) && y == 1) {
						break;
					}
					else if (!board.is_cell_empty(y + 1, x) && y != 2) {
						continue;
					}
					else {
						y++;
						break;
					}
				}
				SetConsoleCursorPosition(output, { (short)(x * 4 + 4), (short)(y * 2 + 2) });
				break;
			case KEY_RIGHT:
				for (; x + 1 < 3; x++) {
					if (!board.is_cell_empty(y, x + 1) && x == 1 || x == 0 && !board.is_cell_empty(y, x + 1) && !board.is_cell_empty(y, x + 2)) {
						break;
					}
					else if (!board.is_cell_empty(y, x + 1)) {
						continue;
					}
					else {
						x++;
						break;
					}
				}
				SetConsoleCursorPosition(output, { (short)(x * 4 + 4), (short)(y * 2 + 2) });
				break;
			case KEY_LEFT:
				for (; x > 0; x--) {
					if (!board.is_cell_empty(y, x - 1) && x == 1 || x == 2 && !board.is_cell_empty(y, x - 1) && !board.is_cell_empty(y, x - 2)) {
						break;
					}
					else if (!board.is_cell_empty(y, x - 1)) {
						continue;
					}
					else {
						x--;
						break;
					}
				}
				SetConsoleCursorPosition(output, { (short)(x * 4 + 4), (short)(y * 2 + 2) });
				break;
			case VK_RETURN:
				board.set_cell(y, x, O);
				make_move(starter == X);
				board.render(O);
				return;
			}
		}
	}
};