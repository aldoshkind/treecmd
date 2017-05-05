#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <map>

#include <readline/readline.h>
#include <readline/history.h>

typedef std::vector<std::string> tokens_t;
typedef void (*command_t)(const tokens_t &);

typedef std::map<std::string, command_t>		commands_t;
commands_t										commands;

std::string current_node_path = "/";

void pwd(const tokens_t &)
{
	printf("%s\n", current_node_path.c_str());
}

void cd(const tokens_t &t)
{
	if(t.size() < 2)
	{
		current_node_path = "/";
	}
	else
	{
		current_node_path = t[1];
	}
}

void ls(const tokens_t &)
{
	//
}

void set(const std::string &path, const std::string &value)
{
	printf("%s : %s\n", path.c_str(), value.c_str());
}

void eval(const tokens_t &tok)
{
	std::string expr;
	for(tokens_t::size_type i = 0 ; i < tok.size() ; i += 1)
	{
		expr += tok[i];
	}

	std::string::size_type ass_sign_pos = expr.find_last_of("=");
	std::string path = expr.substr(0, ass_sign_pos - 1);
	std::string value = expr.substr(ass_sign_pos + 1);

	set(path, value);
}

tokens_t tokenize(const std::string &str)
{
	tokens_t res;
	std::string::size_type pos = 0;
	for( ; pos < str.size() ; )
	{
		std::string::size_type start = str.find_first_not_of(" \t\n\r", pos);

		if(start == std::string::npos)
		{
			break;
		}

		std::string::size_type end = std::string::npos;
		if(str[start] == '"')
		{
			start += 1;
			end = str.find_first_of('"', start);
		}
		else
		{
			end = str.find_first_of(' ', start);
		}
		if(end == std::string::npos)
		{
			res.push_back(str.substr(start));
			pos = end;
		}
		else
		{
			res.push_back(str.substr(start, end - start));
			pos = end + 1;
		}
	}

	return res;
}

void exec(const tokens_t &cmd)
{
	if(cmd.size() < 1)
	{
		return;
	}

	commands_t::iterator it = commands.find(cmd[0]);
	if(it == commands.end())
	{
		eval(cmd);
	}
	else
	{
		(it->second)(cmd);
	}
}

int tab(int, int)
{
	return 0;
}

void exit_cmd(const tokens_t &)
{
	exit(0);
}

void help(const tokens_t &)
{
	printf("available commands:\n");
	for(commands_t::iterator it = commands.begin() ; it != commands.end() ; ++it)
	{
		printf("\t%s\n", it->first.c_str());
	}
}

void init_commands()
{
	commands["exit"] = exit_cmd;
	commands["help"] = help;
	commands["pwd"] = pwd;
	commands["cd"] = cd;
}

int main()
{
	init_commands();

	char *input, shell_prompt[100] = " > ";
	rl_bind_key('\t', tab);

	for( ; ; )
	{
		input = readline((current_node_path + shell_prompt).c_str());

		if(input == NULL)
		{
			break;
		}

		tokens_t ts = tokenize(input);
		exec(ts);

		add_history(input);
		free(input);
	}
}
