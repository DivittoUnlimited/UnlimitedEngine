#ifndef DIALOG_NODE
#define DIALOG_NODE

#include <iostream>
#include <string>
#include <vector>

class DialogNode
{
public:
    DialogNode( std::string dialog, std::vector<std::pair<std::string, int>> responses )
		: mDialog( dialog )
		, mResponses( responses )
	{

	}

	std::string getDialog( void ) { return mDialog; }
    std::vector<std::pair<std::string, int>> getResponses( void ) { return mResponses; }

private:
	std::string mDialog;
    std::vector<std::pair<std::string, int>> mResponses;
};

#endif // DIALOG_NODE
