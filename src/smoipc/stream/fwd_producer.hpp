#pragma once
namespace smoipc::stream {

template<class ShBuf>
struct FwdProducer {
    using Element = typename ShBuf::Element;
    FwdProducer(ShBuf* buffer)
    : shared_buffer_( buffer )
    {}

    bool push(const Element& obj) {
        return shared_buffer_->push(obj);
    }

private:
    ShBuf*              shared_buffer_      ;
};

template<class ShBuf>
auto make_fwd_producer(ShBuf* buffer) {
    return FwdProducer<ShBuf>(buffer);
}

}