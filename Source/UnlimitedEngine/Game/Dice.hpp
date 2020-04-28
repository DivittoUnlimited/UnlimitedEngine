#ifndef DICE_HPP
#define DICE_HPP

#include <string>
#include <random>

class Dice
{
public:
    Dice( ){}
    Dice( std::string exp );
    int roll( void );

private:
    int mMinResult;
    int mMaxResult;
    bool mPositiveValue;
};

#endif // DICE_HPP
