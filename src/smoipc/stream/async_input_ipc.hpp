#pragma once
#include <boost/asio.hpp>
#include "fwd_consumer.hpp"
namespace smoipc::stream {

template<class ShBuf>
struct AsyncInputIPC 
{
    AsyncInputIPC(ShBuf* shbuf, boost::asio::io_service& ios)
    : ios_(ios)
    , shm_consumer_(shbuf)
    {}
    auto& get_io_service() {
        return ios_;
    }
    template<class MutableBuffer, class CompletionHandler>
    auto async_read_some(MutableBuffer&& buf, CompletionHandler&& func ) {
        std::size_t size = boost::asio::buffer_size(buf);
        unsigned char* data = boost::asio::buffer_cast<unsigned char*>(buf);
        boost::system::error_code ec;
        if(size > 0) {
            ios_.post([](){
                
            });
        }
    }

private:
    boost::asio::io_service& ios_;
    FwdConsumer<ShBuf> shm_consumer_;
};

}