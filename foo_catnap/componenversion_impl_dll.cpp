#include "componentversion_impl_dll.h"
#include "resources.h"
#include "pfc/pfc.h"

componentversion_impl_dll::componentversion_impl_dll() {
	DWORD filenameSize = ::GetModuleFileNameW(core_api::get_my_instance(), NULL, 0);

	pfc::array_t<WCHAR> filename;
	filename.set_size(filenameSize + 1);
	::GetModuleFileNameW(core_api::get_my_instance(), filename.get_ptr(), filename.get_size());

	DWORD handle = 0;
	DWORD dataLength = ::GetFileVersionInfoSizeW(filename.get_ptr(), &handle);

	if (dataLength > 0) {
		pfc::array_t<t_uint8> data;
		data.set_size(dataLength);

		if (::GetFileVersionInfoW(filename.get_ptr(), handle, dataLength, static_cast<LPVOID>(data.get_ptr())))
		{
			::VerQueryValueW();
		}
		else
		{
		}
	}
	else
	{
	}
}

void componentversion_impl_dll::get_file_name(pfc::string_base & out) {
	out.set_string(core_api::get_my_file_name());
}

void componentversion_impl_dll::get_component_name(pfc::string_base & out) {
	out.set_string(name);
}

void componentversion_impl_dll::get_component_version(pfc::string_base & out) {
	out.set_string(version);
}

void componentversion_impl_dll::get_about_message(pfc::string_base & out) {
	out.set_string(about);
}
