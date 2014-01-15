#if (!defined COMPONENTVERSION_IMPL_DLL)
#define COMPONENTVERSION_IMPL_DLL

#include "foobar2000/SDK/foobar2000.h"

class componentversion_impl_dll : public componentversion
{
	pfc::string8 name, version, about;
public:
	componentversion_impl_dll();

	virtual void get_file_name(pfc::string_base & out);
	virtual void get_component_name(pfc::string_base & out);
	virtual void get_component_version(pfc::string_base & out);
	virtual void get_about_message(pfc::string_base & out);
};

#endif // (!defined COMPONENTVERSION_IMPL_DLL)
