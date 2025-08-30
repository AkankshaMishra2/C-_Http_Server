#ifndef LOCATIONCONFIGPARSER_HPP
# define LOCATIONCONFIGPARSER_HPP

# include <iostream>
# include <string>
# include "LocationConfig.hpp"
# include "GenericConfigParser.hpp"

class LocationConfigParser : public GenericConfigParser<LocationConfig>
{
	private:

	public:
		LocationConfigParser();
		LocationConfigParser(const LocationConfigParser &copy);
		~LocationConfigParser();
		LocationConfigParser& operator=(const LocationConfigParser& copy);
		void	parseLocationDirectives(std::stringstream &ss, LocationConfig &Location);
};

#endif
