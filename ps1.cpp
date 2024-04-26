#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <fmt/format.h>
#include <string>
#include <functional>

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

std::vector<std::string> split(std::string input, char delimiter = '\n')
{
	std::vector<std::string> output;
	while (input.length())
	{
		output.push_back(input.substr(0, input.find(delimiter)));
		if (input.find(delimiter) == std::string::npos)
			break;
		input = input.substr(input.find(delimiter) + 1);
	}
	return output;
}

void purge(std::vector<std::string>& in, std::string target = "")
{
	for (std::size_t i = 0; i < in.size(); i++)
	{
		if (in[i] == target)
		{
			in.erase(in.begin() + i);
			i--;
		}
	}
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
	try
	{
		return std::getenv("PWD");
	}
	catch (std::exception& e)
	{
		return fmt::format("Failed to get Path: {}", e.what());
	}
}

std::string abbrev_dir(std::size_t max_len = 20)
{
	std::string cur_dur = get_current_dir();
	std::vector<std::string> splits = split(cur_dur, '/');
	purge(splits);

	if (cur_dur.length() < max_len)
		return cur_dur;

	std::string output;
	for (const auto& str : splits)
	{
		if (&str == &splits.back())
		{
			output += std::string("/") + str;
		}
		else
		{
			output += std::string("/") + str[0];
		}
	}

	return output;
}

std::string get_current_user()
{
	std::string name;
	runCommand("whoami 2> /dev/null", [&name](const std::string& buf) -> void{ name += buf; }, 128);
	strip_char(name, '\n');
	return name;
}

std::size_t last_return()
{
	std::size_t code;
	runCommand("echo $?", [&code](const std::string& buf) -> void {
		code = std::stoi(buf);
	}, 3);
	return code;
}

int main(int argc, char* argv[])
{
	std::size_t lrc = std::stoi(argv[1]);
	// Print last return code and warn if it's not 0
	fmt::print("{}", lrc ?
		fmt::styled(" 󰅖 ", fmt::bg(fmt::color::red) | fmt::fg(fmt::color::white)) :
		fmt::styled(" 󰄬 ", fmt::bg(fmt::color::green) | fmt::fg(fmt::color::white))
	);
	fmt::print((lrc ? fmt::bg(fmt::color::red) : fmt::bg(fmt::color::green)) | fmt::fg(fmt::color::black), "{}", codepointToUTF8(0xe0b6));
	fmt::print(fmt::bg(fmt::color::black) | fmt::fg(fmt::color::white), " {} ", get_current_user());
	fmt::print(fmt::bg(fmt::color::black) | fmt::fg(fmt::rgb(48, 48, 48)), "{}", codepointToUTF8(0xe0b6));
	fmt::print(fmt::bg(fmt::rgb(48, 48, 48)) | fmt::fg(fmt::color::white), " {} ", abbrev_dir(30));	
	
	std::string cur_branch = get_git_branch();
	if (!cur_branch.empty())
	{
		fmt::print(fmt::bg(fmt::rgb(48, 48, 48)) | fmt::fg(fmt::color::saddle_brown), "{}", codepointToUTF8(0xe0b6));
		fmt::print(fmt::bg(fmt::color::saddle_brown) | fmt::fg(fmt::color::white), "  {}", get_git_branch());
	}
	fmt::print(fmt::fg(cur_branch.empty() ? fmt::rgb(48, 48, 48) : fmt::color::saddle_brown), "{} ", codepointToUTF8(0xe0b4));
	
	return 0;
}


