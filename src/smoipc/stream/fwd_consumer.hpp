#pragma once
namespace smoipc::stream {

template<class ShBuf>
struct FwdConsumer {
    using Element = typename ShBuf::Element;
    FwdConsumer(ShBuf* buffer)
    : shared_buffer_    ( buffer )
    {}

    bool pop(Element& obj) {
        return shared_buffer_->pop(obj);
    }

private:
    ShBuf*                      shared_buffer_  ;
};

template<class ShBuf>
auto make_fwd_consumer(ShBuf* buffer) {
    return FwdConsumer<ShBuf>(buffer);
}

}
