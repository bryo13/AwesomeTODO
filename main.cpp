/*
 * Background sw to update TODO displayed 
 * by awesome WM 
*/
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <optional>
#include <vector>


void explainError(std::string msg);
constexpr std::optional<std::string> message(int argc, char* argv[]);
constexpr std::string capitalize(std::string& argsChain);

// Get home path
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

// Add todo elements
constexpr bool addTask(std::string& todoPath, std::string& task) {
	std::ofstream todoFile {todoPath, std::ios::app};
	if (!todoFile) {
		todoFile.close();
		std::string error {todoPath + " does not seem to exist"};
		explainError(error);
		return false;
	} else {
		todoFile  << task << "\n";
		todoFile.close();
		return true;
	}
	return false;
}

// Remove todo elements
// Removing todo saves them, motivation to get more stuff done!
constexpr bool removeTask(std::string& todoPath, std::string& donePath) {
	std::ifstream todoFile {todoPath};
	std::vector<std::string> todos;
	std::string activeTodo;
	if (!todoFile) {
		todoFile.close();
		std::string error {todoPath + " does not seem to exist"};
		explainError(error);
		return false;
	} else {
		bool first = true;
		while(std::getline(todoFile, activeTodo)) {
			if (first) {
				first = false;
				std::ofstream doneFile{donePath,std::ios::trunc};
				if (!doneFile) {
					doneFile.close();
					std::string error {donePath + " does not seem to exist"};
					explainError(error);
					return false;
				} else {
					doneFile << activeTodo<<"\n";
				}
				continue;
			}
			todos.push_back(activeTodo);
		}
		todoFile.close();

		// remaining TODO
		std::ofstream remFile{todoPath,std::ios::trunc};
		if (!remFile) {
			remFile.close();
			std::string error {todoPath + " does not seem to exist"};
			explainError(error);
			return false;
		} else {
			for (const auto& l : todos) {
				remFile << l << "\n";
			}
			remFile.close();
			return true;
		}
	}
	return false;
}

// handle args
void handleArgs(int argc, char* argv[]) {
	auto home {returnHome()};
	auto joinedArgs {message(argc, argv)};
	if (!joinedArgs || !home) {
		explainError("error handling args");
		return;
	}
	std::string chainedArgs {capitalize(*joinedArgs)};

	std::string todo {"/todo/todo.txt"};
	std::string done {"/todo/done.txt"};
	std::string todoPath = *home + todo;
	std::string donePath = *home + done;

	std::string firstArg {argv[1]};

	if ("add" == firstArg) {
		if (addTask(todoPath, chainedArgs)) {
			std::cout<<"Successfully added "<<*joinedArgs<<"\n";
		} else {
			explainError("adding todo item");
		}	
	} else if ("done" == firstArg) {
		if (removeTask(todoPath, donePath)) {
			std::cout<<"All done, onto to the next one!"<<"\n";
		} else {
			explainError("removing active task");
		}
	} else {
		explainError("not a valid argument");
		return;
	}	
}
//  add "alarm" when task are not done before 10pm
//  Reminder to set next days TODO

int main(int argc, char* argv[]) {
	if (argc < 2) {
		explainError("few args");
		return 1;
	}
	
	handleArgs(argc, argv);
	return 0;
}

// helper functions 

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

// Optional, capitalize first letter of args
constexpr std::string capitalize(std::string& argsChain) {
	if (std::islower(argsChain[0])) {
		argsChain[0] = std::toupper(argsChain[0]);
	}
	return argsChain;
}
