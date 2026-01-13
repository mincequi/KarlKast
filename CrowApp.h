#pragma once

#include <crow/app.h>
#include <crow/middlewares/CORS.h>

using CrowApp = crow::App<crow::CORSHandler>;
