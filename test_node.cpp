#include "gtest/gtest.h"

#ifdef _WIN32
#include <windows.h>
namespace interlock {
    using counter_t = int64_t;
    inline counter_t increment(counter_t* ctr) {
        return _InterlockedIncrement64(ctr);
    }
    inline counter_t decrement(counter_t* ctr) {
        return _InterlockedDecrement64(ctr);
    }
}
#endif

struct INode {
    virtual ~INode() = default;
    
    // @brief When true a NodeRef will delete it, return false if owned by for example a pool 
    virtual bool destruct() { return true; } 
};

struct NodeCtrlBlock;
struct INodeCtrlBlockPool {
    virtual ~INodeCtrlBlockPool() = default;

    // @brief When true a NodeRef will delete it, override and return false if owned / should not be deleted
    virtual bool destruct() { return true; }

    // @brief Reserve a free control block
    virtual NodeCtrlBlock* reserve() = 0;

    // @brief Release a control block
    virtual void release(NodeCtrlBlock* nodeCtrlBlock) = 0;
};

struct NodeCtrlBlock {
    struct Pool {
        interlock::counter_t refCount;
        INodeCtrlBlockPool* impl;
    };
    interlock::counter_t refCount;
    INode* node;    
    Pool*  pool;
    int    id;

    void add_reference() {
        assert(refCount > 0);
        assert(pool != nullptr);
        assert(pool->impl != nullptr);
        interlock::increment(&pool->refCount);
        interlock::increment(&refCount);
    }
    void remove_reference() {
        assert(refCount > 0);
        assert(pool != nullptr);
        assert(pool->impl != nullptr);
        interlock::counter_t counter=interlock::decrement(&refCount);
        if (counter > 0)
            return;
        if (node->destruct())
            delete node;
        node = nullptr;
        pool->impl->release(this);
        counter = interlock::decrement(&pool->refCount);
        if (counter > 0)
            return;
        if (pool->impl->destruct())
            delete pool->impl;
    }
};


template<class T>
struct NodeRef {
    NodeRef() : nodeCtrlBlock(nullptr) {}
    
    explicit NodeRef(const NodeRef<T>& other) noexcept : nodeCtrlBlock(other.nodeCtrlBlock) {
        if (nodeCtrlBlock)
            nodeCtrlBlock->add_reference();        
    }
    NodeRef<T>& operator=(const NodeRef<T>& other) noexcept {
        std::swap(NodeRef<T>(other), *this);
        return *this;
    }
private:
    NodeCtrlBlock* nodeCtrlBlock;
};



template <size_t N>
struct TestNodeCtrlBlockPool : INodeCtrlBlockPool {

    // @brief Reserve a free control block
    NodeCtrlBlock* reserve() override {
        return nullptr;
    }
    
    // @brief Release a control block
    void release(NodeCtrlBlock* nodeCtrlBlock) override {

    }
};


TEST(node, pool)
{
    
    EXPECT_EQ(1, 1);
    EXPECT_TRUE(true);
}