#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "PatternsFacade.H"

void printUsage(void)
{
	printf("Usage:\n");
	printf("\tTest t [file] ...\n");
	printf("\tTest s [file1] [file2]\n");
	printf("\tTest v [file]\n");
	printf("\tTest i\n");
	printf("\tTest d [file1] [file2]\n");
	printf("\tTest e [file]\n");
	printf("\tTest c\n");
	printf("\tTest p [file]\n");
	printf("\tTest n [file]\n");
}

static bool requireArgs(int argc, int needed)
{
	if (argc < needed)
	{
		printUsage();
		return false;
	}
	return true;
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printUsage();
		exit(0);
	}

	PatternsFacade	app;

	switch(argv[1][0])
	{
	case 'T':
	case 't':
	{
		std::vector<std::string>	files;
		for (int i = 2; i < argc; i++)
			files.emplace_back(argv[i]);
		app.tokenize(files);
		break;
	}
	case 'S':
	case 's':
		if (requireArgs(argc, 4))
			app.serialize(argv[2], argv[3]);
		break;
	case 'V':
	case 'v':
		if (requireArgs(argc, 3))
			app.validate(argv[2]);
		break;
	case 'I':
	case 'i':
		app.iterate();
		break;
	case 'D':
	case 'd':
		if (requireArgs(argc, 4))
			app.buildAndSerialize(argv[2], argv[3]);
		break;
	case 'E':
	case 'e':
		if (requireArgs(argc, 3))
			app.handleEvents(argv[2]);
		break;
	case 'C':
	case 'c':
		app.runCommands();
		break;
	case 'P':
	case 'p':
		if (requireArgs(argc, 3))
			app.prototype(argv[2]);
		break;
	case 'N':
	case 'n':
		if (requireArgs(argc, 3))
			app.interpret(argv[2]);
		break;
	}
}
