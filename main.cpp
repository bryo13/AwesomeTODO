/*
 * Background sw to update TODO displayed 
 * by awesome WM 
*/
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <optional>

// 1. get home path
constexpr std::optional<std::string> returnHome() {
	const char* home = std::getenv("HOME");
	if (home != nullptr) {
		std::string homePth {home};
		return homePth;
	} else {
		std::cerr<<"HOME env var does not seem be set\n";
		return std::nullopt;
	}
}

// 2. read first line of todo.txt
// todo.txt is the file read by awesome WM
constexpr std::optional<std::string> currentTask(std::string todoPath) {
	std::ifstream todoFile {todoPath};
	if (!todoFile) {
		std::cerr<<todoPath<<" does not seem to exist\n";
		todoFile.close();
		return std::nullopt;
	}

	std::string activeTodo;
	if (std::getline(todoFile, activeTodo)) {
		todoFile.close();
		return activeTodo;
	} else {
		todoFile.close();
		std::cerr<<todoPath<<" file is empty or can be read, try adding a task using ./todo add finish todo project\n";
		return std::nullopt;
	}
}

// 3. add todo elements
constexpr bool addTask(std::string todoPath, std::string task) {
	std::ofstream todoFile {todoPath, std::ios::app};
	if (!todoFile) {
		std::cerr<<todoPath<<" does not seem to exist\n";
		todoFile.close();
		return false;
	} else {
		todoFile << task << "\n";
		todoFile.close();
		return true;
	}
	return false;
}
// 4. remove todo elements
// Removing todo saves them, motivation to get more stuff done!

// 5a helper func to combine string after argv[1]
constexpr std::optional<std::string> message(int argc, char* argv[]) {
	if (argc < 3) {
		std::cerr<<"Add arguments \n";
		return std::nullopt;
	}
	
	std::string joinedArgs;
	for (int i{2}; i<argc;++i) {
		joinedArgs += argv[i];	
		joinedArgs += " ";
	}

	return joinedArgs;
}

// 5. handle args
void handleArgs(int argc, char* argv[]) {
	auto home {returnHome()};
	std::string todo {"/todo.txt"};
	std::string todoPath {*home + todo};

	std::string firstArg {argv[1]};
	auto joinedArgs {message(argc, argv)};

	if ("add" == firstArg) {
		if (addTask(todoPath, *joinedArgs)) {
			std::cout<<"Successfully added "<<*joinedArgs<<"\n";
		} else {
			std::cerr<<"Error adding new todo! \n";
			return;
		}
	} else {
		std::cerr<<"Unknown arg \n";
	}
}

// 6. add "alarm" when task are not done before 10pm
int main(int argc, char* argv[]) {
	handleArgs(argc, argv);
	return 0;
}
