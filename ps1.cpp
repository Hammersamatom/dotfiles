#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include <functional>
#include <iostream>

#include <fmt/core.h>
#include <fmt/color.h>


// Stole the code from my personal debugger LOL
#ifdef _WIN32
    #define popen _popen //Windows has _popen, but not just popen. Exact same signature and function
    #define pclose _pclose //Same reason
#endif

int runCommand(const std::string& command, std::function<void(const std::string&)> callback, size_t outputBufferSize) {
    FILE* actionOutput = popen(command.c_str(), "r");
    if (actionOutput == nullptr) {
        throw std::runtime_error("Failed to run command");
    }
    size_t bytesRead = 0;
    std::vector<char> chunk(outputBufferSize);
    while ((bytesRead = fread(chunk.data(), sizeof(char), chunk.size(), actionOutput)) > 0) {
        callback(std::string(chunk.data(), bytesRead));
    }
    if (ferror(actionOutput) != 0) {
        //Error occured. Not normal EOF
        throw std::system_error(std::make_error_code((std::errc)errno));
    }
    if (feof(actionOutput) == 0) {
        //EOF should be set, but isn't
        throw std::runtime_error("Unexpected EOF when reading command output");
    }
    return pclose(actionOutput);
}

std::string codepointToUTF8(uint32_t codepoint) {
    std::string utf8;
    if (codepoint < 0x80) {
        //One byte only. Cast directly to uint8_t (safe to do here)
        utf8.push_back(codepoint);
    } else if (codepoint < 0x0800) {
        //Two bytes
        utf8.push_back(0xC0 | (codepoint >> 6 & 0x1F));
        utf8.push_back(0x80 | (codepoint >> 0 & 0x3F));
    } else if (codepoint < 0x10000) {
        //Three
        utf8.push_back(0xE0 | (codepoint >> 12 & 0x0F));
        utf8.push_back(0x80 | (codepoint >> 6 & 0x3F));
        utf8.push_back(0x80 | (codepoint >> 0 & 0x3F));
    } else if (codepoint < 0x110000) {
        //Four
        utf8.push_back(0xF0 | (codepoint >> 18 & 0x1F));
        utf8.push_back(0x80 | (codepoint >> 12 & 0x3F));
        utf8.push_back(0x80 | (codepoint >> 6 & 0x3F));
        utf8.push_back(0x80 | (codepoint >> 0 & 0x3F));
    } else {
        //Cannot be encoded with UTF-8
        throw std::runtime_error("Invalid codepoint for UTF-8 encoding");
    }
    return utf8;
}

enum DIRECTION {
	FRONT,
	BACK
};

// Feed it in input std::string to be modified in-place
std::size_t strip_char(std::string& input, char delimiter, DIRECTION dir = BACK)
{
	std::size_t count = 0;
	
	if (input.empty())
		return count;

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
	std::string branch;
	int retval = runCommand("git rev-parse --abbrev-ref HEAD 2> /dev/null", [&branch](const std::string& buf) -> void{ branch += buf; }, 128);
	strip_char(branch, '\n');
	return branch;
}

std::string get_current_dir()
{
	std::string path = "";
	
	try
	{
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
	std::string name;
	runCommand("whoami 2> /dev/null", [&name](const std::string& buf) -> void{ name += buf; }, 128);
	strip_char(name, '\n');
	return name;
}

int main(int argc, char* argv[])
{
	fmt::print(fmt::bg(fmt::color::dark_cyan) | fmt::fg(fmt::color::white), " {} ", get_current_user());
	fmt::print(fmt::bg(fmt::color::black) | fmt::fg(fmt::color::dark_cyan), "{}", codepointToUTF8(0xe0b4));
	fmt::print(fmt::bg(fmt::color::black) | fmt::fg(fmt::color::white), " {} ", get_current_dir());	
	fmt::print(fmt::bg(fmt::color::brown) | fmt::fg(fmt::color::black), "{}", codepointToUTF8(0xe0b4));
	fmt::print(fmt::bg(fmt::color::brown) | fmt::fg(fmt::color::white), "  {} ", get_git_branch());
	fmt::print(fmt::fg(fmt::color::brown), "{} ", codepointToUTF8(0xe0b4));

	return 0;
}
