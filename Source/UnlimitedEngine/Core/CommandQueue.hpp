#ifndef BOOK_COMMANDQUEUE_HPP
#define BOOK_COMMANDQUEUE_HPP

#include "Command.hpp"

#include <queue>

///
/// \brief The CommandQueue class
/// Simple Abstraction of the std::Queue to handle command lists frame by frame.
class CommandQueue
{
	public:
        void	push(const Command& command);
        Command	pop();
        bool	isEmpty() const;
	private:
        std::queue<Command>	mQueue;
};

#endif // BOOK_COMMANDQUEUE_HPP
