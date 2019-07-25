// Member Defination for Class TrafficLightStateCallable

#include "TrafficLightStateCallable.hpp"

namespace traffic_manager {

    TrafficLightStateCallable::TrafficLightStateCallable(
        SyncQueue<PipelineMessage>* input_queue,
        SyncQueue<PipelineMessage>* output_queue,
        SharedData* shared_data):
        PipelineCallable(input_queue, output_queue, shared_data){}

    TrafficLightStateCallable::~TrafficLightStateCallable(){}

    PipelineMessage TrafficLightStateCallable::action(PipelineMessage in_message)
    {
        PipelineMessage out_message;

        float throttle = in_message.getAttribute("throttle");
        float brake = in_message.getAttribute("brake");
        float steer = in_message.getAttribute("steer");

        auto vehicle = boost::static_pointer_cast<carla::client::Vehicle>(in_message.getActor());
        auto traffic_light_state = vehicle->GetTrafficLightState();
        
        //Implement a faster way to find closest waypoint
        auto closest_waypoint = shared_data->local_map->getWaypoint(vehicle->GetLocation());

        auto next_waypoint = shared_data->buffer_map[vehicle->GetId()]->getContent(5);

        float traffic_hazard = -1;
        
        if (
            !(closest_waypoint->checkJunction())
            and
            (
                traffic_light_state == carla::rpc::TrafficLightState::Red
                or
                traffic_light_state == carla::rpc::TrafficLightState::Yellow
            )
            and
            next_waypoint[4]->checkJunction()
        ) {
            traffic_hazard = 1;
        }


        out_message.setActor(in_message.getActor());
        out_message.setAttribute("traffic_light", traffic_hazard);
        out_message.setAttribute("collision", in_message.getAttribute("collision"));
        out_message.setAttribute("velocity", in_message.getAttribute("velocity"));
        out_message.setAttribute("deviation", in_message.getAttribute("deviation"));

        return out_message;
    }
}