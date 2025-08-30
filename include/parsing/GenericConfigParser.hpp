#ifndef GENERICCONFIGPARSER_HPP
# define GENERICCONFIGPARSER_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <vector>
# include <map>
# include <fstream>
# include <cstring>
# include <cstdlib>
# include "../utils.hpp"

template <typename ConfigType>
class GenericConfigParser
{
	public:
		template<typename SetterFunc>
		void	parseAddDirectiveSimple(std::stringstream &ss, ConfigType &config, SetterFunc setter, const std::string &directive);
		template<typename T1, typename T2>
		void	parseDirectiveTwoValues(std::stringstream &ss, ConfigType &config, 
						void (ConfigType::*setter)(T1, T2), const std::string& directive);
		template<typename Container>
		void	parseDirectiveMultipleValues(std::stringstream &ss, ConfigType &config,
						void (ConfigType::*adder)(const typename Container::value_type&), const std::string& directive);
};

# include "GenericConfigParser.tpp"

#endif
