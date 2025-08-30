#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <iostream>
# include <sstream>
# include <cstdlib>

// ---------------------------------------------------------------------------
// ANSI color namespace — functions only (avoids ODR violations from constants
// being defined in a header included across multiple translation units).
// ---------------------------------------------------------------------------
namespace color
{
	// Prints src in red to stdout and returns the stream.
	static inline std::ostream& red(const std::string& src)
	{
		return (std::cout << "\033[31m" << src << "\033[0m" << std::endl);
	}

	// Prints src in red to stderr and returns the stream.
	static inline std::ostream& err_red(const std::string& src)
	{
		return (std::cerr << "\033[31m" << src << "\033[0m" << std::endl);
	}

	// Prints src in green to stdout and returns the stream.
	static inline std::ostream& green(const std::string& src)
	{
		return (std::cout << "\033[32m" << src << "\033[0m" << std::endl);
	}

	// Prints src in green to stderr and returns the stream.
	static inline std::ostream& err_green(const std::string& src)
	{
		return (std::cerr << "\033[32m" << src << "\033[0m" << std::endl);
	}
}

// ---------------------------------------------------------------------------
// toStr — C++98-compatible integer-to-string helper.
// Avoids repeating the ostringstream boilerplate everywhere.
// ---------------------------------------------------------------------------
template <typename T>
static inline std::string toStr(const T& value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

// ---------------------------------------------------------------------------
// Global utility functions (implemented in srcs/errors/parsing/errors.cpp)
// ---------------------------------------------------------------------------

// Verifies no extra tokens remain in the stream after the expected value.
void	excluding_token(std::stringstream& ss, std::string token);

// Prints an error message in red and exits with the given code.
void	errorTypeExt(const std::string& errorMsg, int code);

#endif
