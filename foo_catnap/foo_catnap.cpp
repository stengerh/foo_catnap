#include "stdafx.h"
#include "foobar2000/SDK/foobar2000.h"

#include "catnap_server.h"

DECLARE_COMPONENT_VERSION(
	"RESTful API",
	"1.0.0",
	"TODO: Add description"
)

class initquit_catnap : public initquit
{
public:
	virtual void on_init();
	virtual void on_quit();
};

namespace {
	CatNap::CatNapServer g_server;
}

void initquit_catnap::on_init()
{
	g_server.start();
}

void initquit_catnap::on_quit()
{
	g_server.stop();
}

namespace {
	initquit_factory_t<initquit_catnap> g_initquit_factory;
}
