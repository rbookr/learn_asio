#pragma once

#include <tuple>

namespace http {
namespace server {
    struct request;
    
    class request_parser {
        public:
            request_parser();
            void reset();
    };


}
}
