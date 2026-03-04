#include "Director.H"
#include "Builder.H"

Director::Director(const std::string & filename, std::shared_ptr<Builder> builder)
{
	builder->buildFromFile(filename);
}
