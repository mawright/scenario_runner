// Declaration for base class of all pipeline threads
#pragma once
#include "SyncQueue.hpp"
#include "PipelineMessage.hpp"
#include "SharedData.hpp"

namespace traffic_manager {

class PipelineCallable
{
private:
    SyncQueue<PipelineMessage>* const input_queue;
    SyncQueue<PipelineMessage>* const output_queue;

protected:
    SharedData* const shared_data;
    PipelineMessage readQueue();
    void writeQueue(PipelineMessage);
    virtual PipelineMessage action(PipelineMessage message)=0;

public:
    PipelineCallable(
        SyncQueue<PipelineMessage>* input_queue,
        SyncQueue<PipelineMessage>* output_queue,
        SharedData* shared_data);
    virtual ~PipelineCallable();
    void run();
};

}