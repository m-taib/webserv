#include "BitcoinExchange.hpp"

int     main(int ac ,char **av)
{
    if (ac != 2)
    {
        std::cout << "Error: could not open file." << std::endl;
        return (0);
    }
    try 
    {
        BitcoinExchange     database("data.csv");
        database.extract_data(av[1]);
    } 
    catch (const std::exception& exp) 
    {
        std::cout << exp.what() << std::endl;   
    }
    return (0);
}
