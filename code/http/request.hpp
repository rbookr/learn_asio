#pragma  once

#include <string>
#include <vector>
#include "header.hpp"

namespace http {
namespace server {

    struct request {
        std::string method;
        std::string uri;
        int http_version_major;
        int http_version_minor;
        std::vector<header> headers;
    };


}
}
