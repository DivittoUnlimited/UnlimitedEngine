#include "Dice.hpp"
#include <iostream>
#include "Core/Utility.hpp" // for randomizer

Dice::Dice( std::string exp )
{
    bool minSolved = false;
    std::string min = "";
    std::string max = "";

    for( auto c : exp )
    {
        switch( c )
        {
        case '+':
            mPositiveValue = true;
            minSolved = true;
            break;
        case '-':
            mPositiveValue = false;
            minSolved = true;
            break;
        default:
            if( minSolved ) max += c;
            else min += c;
            break;
        }
    }
    mMinResult = std::atoi( min.c_str( ) );
    mMaxResult = std::atoi( max.c_str( ) );
}

int Dice::roll( void )
{
    int score = randomInt( mMaxResult ) + mMinResult;
    if( score > mMaxResult ) score = mMaxResult;
    // std::cout << "Dice Roll: Min: " << mMinResult << " Max: " << mMaxResult << " Result: " << score << std::endl;
    if( mPositiveValue ) return score;
    return -1 * score;
}
