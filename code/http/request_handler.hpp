#pragma  once

#include <string>

namespace http {
namespace server {

    struct reply;
    struct request;
    
    class request_handler {
        public:
            request_handler(const request_handler&) = delete;
            request_handler& operator=(const request_handler&) = delete;

            /// Construct with a directory containing files to be served.
            explicit request_handler(const std::string & doc_root);

            /// Handle a request and produce a reply.
            void handler_request(const request & req, reply & rep);

        private:
            std::string doc_root_;
            static bool url_decode(const std::string& in, std::string& out);
    };



}
}
