
#include "Core/Application.hpp"

#include <stdexcept>
#include <iostream>
#include <fstream>

int main()
{
    try
    {
        Application( ).run( );
    }
    catch( std::exception& e )
    {
        std::cout << "\nException: " << e.what( ) << std::endl;
    }
    return 0;
}
