#include <iostream>
#include <windows.h>

#include "src\linkedlist.h"
#include "src\stack.h"
#include "src\queue.h"
#include "src\circulararray.h"


namespace ParenthesesBalanceCheck {
	class Symbol {
		wchar_t symbol;
		unsigned short column;
		// 0 - Parentheses
		// 1 - Square Brackets
		// 2 - Braces
		unsigned short type;
		// 0 - Opening
		// 1 - Closing
		unsigned short func;
	public:
		Symbol(wchar_t symbol, unsigned short column) {
			switch (symbol) {
			case '(':
				type = 0; func = 0; break;
			case ')':
				type = 0; func = 1; break;
			case '[':
				type = 1; func = 0; break;
			case ']':
				type = 1; func = 1; break;
			case '{':
				type = 2; func = 0; break;
			case '}':
				type = 2; func = 1; break;
			default:
				type = -1; func = -1; break;
			}
			this->column = column;
			this->symbol = symbol;
		}
		unsigned short get_type() const {
			return type;
		}
		unsigned short get_func() const {
			return func;
		}
		unsigned short get_column() const {
			return column;
		}
		wchar_t get_symbol() const {
			return symbol;
		}
		bool operator == (Symbol& obj) {
			return (obj.get_type() == this->get_type() && obj.get_func() == this->get_func() && obj.get_column() == this->get_column());
		}
	};

	void pause() {
		std::wcout << L"Press any key to continue . . . ";

		HANDLE hstdin;
		DWORD  mode;
		hstdin = GetStdHandle(STD_INPUT_HANDLE);
		GetConsoleMode(hstdin, &mode);
		SetConsoleMode(hstdin, ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);

		unsigned int ch = 10;
		while (ch == 10)
			ch = std::wcin.get();
		SetConsoleMode(hstdin, mode);
	}
	void clear() {
		COORD topLeft = { 0, 0 };
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO screen;
		DWORD written;

		GetConsoleScreenBufferInfo(console, &screen);
		FillConsoleOutputCharacterA(
			console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
		);
		FillConsoleOutputAttribute(
			console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
			screen.dwSize.X * screen.dwSize.Y, topLeft, &written
		);
		SetConsoleCursorPosition(console, topLeft);
	}

	void show_error(bool& error_found, unsigned short& issues) {
		std::wcout << L"҇";
		error_found = true;
		issues++;
	}
	void show_error(short column, short line = -1) {
		CONSOLE_SCREEN_BUFFER_INFO screen;
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(console, &screen);

		COORD  position = { 21 + column, (line != -1) ? line : screen.dwSize.Y };
		HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(output, position);
		std::wcout << L"҇";
	}
	int look_for(Stack<Symbol>& stack, Symbol query) {
		Stack<Symbol> temp;
		for (index_t i = 0; i < stack.size(); i++) {
			if (stack.top() == query) {
				unsigned short column = stack.top().get_column();
				for (index_t j = 0; j < stack.size(); j++)
					stack.pop();
				for (index_t j = 0; j < temp.size(); j++) {
					stack.push(temp.top());
					temp.pop();
				}
				return column;
			}
			temp.push(stack.top());
			stack.pop();
		}
		for (index_t j = 0; j < temp.size(); j++)
			stack.push(temp.top());
		return -1;
	}
	bool remove_symbol(Stack<Symbol>& stack, unsigned short column) {
		Stack<Symbol> temp;
		for (index_t i = 0; i < stack.size(); i++) {
			if (stack.top().get_column() == column) {
				for (index_t j = 0; j < stack.size(); j++)
					stack.pop();
				for (index_t j = 0; j < temp.size(); j++) {
					stack.push(temp.top());
					temp.pop();
				}
				return true;
			}
			temp.push(stack.top());
			stack.pop();
		}
		for (index_t j = 0; j < temp.size(); j++)
			stack.push(temp.top());
		return false;
	}

	int main() {
		SetConsoleOutputCP(CP_UTF8);
		setvbuf(stdout, nullptr, _IOFBF, 1000);
		std::wcout.sync_with_stdio(false);
		std::wcout.imbue(std::locale("en_US.utf8"));

		for (unsigned long long repeats = 1; true; repeats++) {
			Stack<Symbol> symbols;
			std::wstring input;
			std::wcout << "Enter an expression: ";
			std::wcin >> input;
			std::wcout << std::wstring(21, ' ');
			unsigned short i = 0, issues = 0;
			for (wchar_t& c : input) {
				bool error_found = false;
				Symbol s(c, i);

				if (s.get_type() < 0) {
					i++;
					continue;
				}
				else if (s.get_func() && !i)
					show_error(error_found, issues);
				else if (!s.get_func())
					symbols.push(s);
				else if (s.get_func() && symbols.size()) {
					if (symbols.top().get_type() != s.get_type()) {
						int column = look_for(symbols, s);
						if (column < 0 || !remove_symbol(symbols, column))
							show_error(error_found, issues);
					}
					else
						symbols.pop();
				}
				else if (s.get_func()) {
					std::cout << std::string(i, ' ');
					show_error(error_found, issues);
				}
				i++;
			}
			if (symbols.empty() && !issues)
				std::wcout << std::endl << "No issues found!" << std::endl;
			else {
				std::wcout << std::endl << "Issues were found." << std::endl;
				if (symbols.size()) {
					unsigned short spaces = 0, i = 0;
					for (Symbol s = symbols.top(); i < symbols.size(); i++) {
						CONSOLE_SCREEN_BUFFER_INFO cbsi;
						GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cbsi);
						show_error(s.get_column(), cbsi.dwCursorPosition.X + 1);
						s = symbols.top(), symbols.pop();
						std::cout << std::endl << std::endl;
						break;
					}
				}
			}
			pause();
			clear();
		}
	}
}

int main(int argc, char** argv) {
	
}
