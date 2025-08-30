#include "GenericConfigParser.hpp"

//In this template class you will find functions that will handle the server side as well as Location while taking into account if the directive contains one or more "arguments".
// Instead of making each function in each case, I preferred to make three function templates for the different cases

template <typename ConfigType>
template<typename SetterFunc>
void	GenericConfigParser<ConfigType>::parseAddDirectiveSimple(std::stringstream &ss, ConfigType &config,
					SetterFunc setter, const std::string &directive)
{
	std::string value;
	if (ss >> value)
	{
		if (!value.empty() && value[value.size() - 1] == ';')
			value.resize(value.size() - 1);
		else
			errorTypeExt(directive + ": ';' missing at the end of a directive", -2);
		(config.*setter)(value);
	}
	else
		errorTypeExt(directive + ": Problem with this directive!", -1);
}

template <typename ConfigType>
template<typename T1, typename T2>
void	GenericConfigParser<ConfigType>::parseDirectiveTwoValues(std::stringstream &ss, ConfigType &config, 
					void (ConfigType::*setter)(T1, T2), const std::string& directive)
{
	std::string value1, value2;
	if (!(ss >> value1 >> value2))
		errorTypeExt(directive + ": Missing values!", -1);
	if (!value2.empty() && value2[value2.size() - 1] == ';')
		value2.resize(value2.size() - 1);
	else
		errorTypeExt(directive + ": ';' missing at the end", -2);
	if (directive == "error_page")
	{
		for (size_t i = 0; i < value1.length(); i++)
		{
			if (!std::isdigit(value1[i]))
				errorTypeExt("client_max_body_size: Only numbers possible (Min: 1024 <-> Max: 104857600)", -3);
		}
        (config.*setter)(value1, value2);
    }
	else if (directive == "cgi")
        (config.*setter)(value1, value2);
}

template <typename ConfigType>
template<typename Container>
void GenericConfigParser<ConfigType>::parseDirectiveMultipleValues(std::stringstream &ss, ConfigType &config,
					void (ConfigType::*adder)(const typename Container::value_type&), const std::string& directive)
{
	std::string value;
	bool foundSemicolon = false;

    while (ss >> value)
	{
		if (!value.empty() && value[value.size() - 1] == ';')
		{
			value.resize(value.size() - 1);
			foundSemicolon = true;
			if (!value.empty() && (value == "GET" || value == "POST" || value == "DELETE"))
			{
				(config.*adder)(value);
			}
			else
				errorTypeExt(directive + ": Problem with this directive!", -1);
			break;
		}
		if (value == "GET" || value == "POST" || value == "DELETE")
			(config.*adder)(value);
	}
	if (!foundSemicolon)
		errorTypeExt(directive + ": ';' missing at the end", -2);
}
