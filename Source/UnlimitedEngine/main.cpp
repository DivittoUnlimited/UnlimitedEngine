
#include "Core/Application.hpp"

#include <stdexcept>
#include <iostream>
#include <fstream>

int main()
{
/*
    std::ifstream stream;

    std::string line;
    std::ifstream myfile( "Notes.txt" );
    if( myfile.is_open( ) )
    {
        std::cout << "!======================!\nGet Shit Done!!!" << std::endl;
        while( getline( myfile, line ) )
        {
             std::cout << line << "\n";
        }
        myfile.close( );
        std::cout << "!======================!\n";
    } else std::cout << "Unable to open to-do.txt" << std::endl;;
*/
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
