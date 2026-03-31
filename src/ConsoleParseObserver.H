#ifndef CONSOLEPARSEOBSERVER_H
#define CONSOLEPARSEOBSERVER_H

#include "ParseObserver.H"

//
// ConsoleParseObserver: Concrete Observer colleague that formats
// parsing events as human-readable progress lines on stdout.
//
class ConsoleParseObserver : public ParseObserver
{
private:
	void	printIndent(int depth) const;
public:
	virtual void	update(const ParseEvent & event);
};

#endif // CONSOLEPARSEOBSERVER_H
