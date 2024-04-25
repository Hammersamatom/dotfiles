#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iterator>
#include <string>
#include <fstream>
#include <filesystem>

#include <fmt/core.h>
#include <fmt/color.h>



enum DIRECTION {
	FRONT,
	BACK
};

// Feed it in input std::string to be modified in-place
std::size_t strip_char(std::string& input, char delimiter, DIRECTION dir = BACK)
{
	std::size_t count = 0;
	
	switch (dir)
	{
		case BACK:
			while (input.back() == delimiter)
			{
				input.pop_back();
				count++;
			}
			break;
		case FRONT:
			while (input.front() == delimiter)
			{
				input = input.substr(1, input.length() - 1);
				count++;
			}
			break;
	}

	return count;
}

std::string get_git_branch()
{
	int retval = std::system("git rev-parse --abbrev-ref HEAD 2> /dev/null > /tmp/git.tmp");
	//if (retval)
	//{
	//	fmt::print("Command returned with code {}\n", retval);
	//	exit(1);
	//}

	std::ifstream tmpfile_handler = std::ifstream("/tmp/git.tmp");
	if (!tmpfile_handler.is_open())
	{
		fmt::print("Failed to open file\n");
		exit(2);
	}

	std::string contents((std::istreambuf_iterator<char>(tmpfile_handler)), (std::istreambuf_iterator<char>()));

	//return "No Branch";

	strip_char(contents, '\n');
	return contents;
}

std::string get_current_dir()
{
	std::string path = "";
	
	try {
		path = std::getenv("PWD");
	}
	catch (std::exception& e)
	{
		path = fmt::format("Failed to get Path: {}", e.what());
	}

	return path;
}

std::string get_current_user()
{
	std::system("whoami 2> /dev/null > /tmp/usr.tmp");
	std::ifstream name_handler = std::ifstream("/tmp/usr.tmp");
	std::string name((std::istreambuf_iterator<char>(name_handler)), (std::istreambuf_iterator<char>()));
	strip_char(name, '\n');
	return name;
}

int main(int argc, char* argv[])
{
	fmt::print("{}", fmt::styled(fmt::format(" {} ", get_current_user()), fmt::bg(fmt::color::dark_cyan) | fmt::fg(fmt::color::white)));
	fmt::print("{}", fmt::styled(fmt::format(" {} ", get_current_dir()), fmt::bg(fmt::color::slate_gray) | fmt::fg(fmt::color::white)));	
	fmt::print("{}", fmt::styled(fmt::format("  {} ", get_git_branch()), fmt::bg(fmt::color::purple) | fmt::fg(fmt::color::white)));
	return 0;
}
