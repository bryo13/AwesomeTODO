/*
 * Background sw to update TODO displayed 
 * by awesome WM 
*/
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <optional>


void explainError(std::string msg);
constexpr std::optional<std::string> message(int argc, char* argv[]);
constexpr std::optional<std::string> iconPath(std::string home);

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
	auto home {returnHome()};
	if (!home) {
		explainError("Home env not defined");
		return false;
	}
	auto iconPth {iconPath(*home)};
	if (!iconPth)  {
		explainError("icon path not defined");
		return false;
	}
	std::ofstream todoFile {todoPath, std::ios::app};
	if (!todoFile) {
		todoFile.close();
		std::string error {todoPath + " does not seem to exist"};
		explainError(error);
		return false;
	} else {
		todoFile <<*iconPth<<"- " << task << "\n";
		todoFile.close();
		return true;
	}
	return false;
}
// 4. remove todo elements
// Removing todo saves them, motivation to get more stuff done!


// 5. handle args
void handleArgs(int argc, char* argv[]) {
	auto home {returnHome()};
	auto joinedArgs {message(argc, argv)};
	if (!joinedArgs) {
		explainError("error reading todo task");
		return;
	}
	if (home) {
		std::string todo {"/todo/todo.txt"};
		std::string todoPath = *home + todo;

		std::string firstArg {argv[1]};

		if ("add" == firstArg) {
			if (addTask(todoPath, *joinedArgs)) {
				std::cout<<"Successfully added "<<*joinedArgs<<"\n";
			} else {
				explainError("adding todo item");
			}
		} else {
			explainError("unknown arg");
			return;
		}
	}	else {
		explainError("Home env not defined");
		return;
	}	
}
// 6. add "alarm" when task are not done before 10pm
// 7. Reminder to set next days TODO

int main(int argc, char* argv[]) {
	if (argc < 2) {
		explainError("few args");
		return 1;
	}
	
	handleArgs(argc, argv);
	return 0;
}

// 8. helper functions 

void explainError(std::string msg) {
	std::string error{msg};
	std::cout<<"Error caused by: "<<error<<"\n";
	std::cerr<<" program usage: \n"; 
	std::cerr<<" ./todo add finish this project\n"; 
	std::cerr<<" ./todo done //removes the active task\n"; 
}

// helper func to combine string after argv[1]
constexpr std::optional<std::string> message(int argc, char* argv[]) {
	if (argc < 2) {
		explainError("few args");
		return std::nullopt;
	} else {
	
		std::string joinedArgs;
		for (int i{2}; i<argc;++i) {
			joinedArgs += argv[i];	
			if (i < argc -1) {
				joinedArgs += " ";
			}
		}

		return joinedArgs;
	}
}

// get icons path
constexpr std::optional<std::string> iconPath(std::string home) {
	std::string ic {"/todo/icons/thunder.png"};
	std::string icons {home + ic};
	return icons;
}
