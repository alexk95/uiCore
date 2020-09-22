#include <ak_uiAPI.h>
#include <Example.h>

#include <ak_fastContainer.h>

using namespace ak;

int main(int argc, char *argv[])
{
	uiAPI::ini(true);
	Example e;
	return uiAPI::exec();
}
