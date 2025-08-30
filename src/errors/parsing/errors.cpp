#include "../../../include/utils.hpp"

void	excluding_token(std::stringstream& ss, std::string token) // msg custom ?
{
	if (ss >> token && !ss.eof())
	{
		std::cerr << "Error: 1!" << std::endl;
		std::exit(-1);
	}
}

void	errorTypeExt(const std::string &errorMsg, int code)
{
	color::err_red("Error: " + errorMsg);
	std::exit(code);
}
