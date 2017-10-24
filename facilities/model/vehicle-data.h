

#ifndef VEHICLE_DATA_H
#define VEHICLE_DATA_H

/** Data shared between cars */

struct SharedData {

  		int64_t    position[2];

        int64_t    timestamp;

        int32_t    clock;

        int32_t    wheelbase;

        int32_t    wheelbase_lanechange;

        int32_t    global_offset;

        int32_t    heading[2];

        int32_t    velocity[2];

        int16_t    id;

        int16_t    reference_index;

        int8_t     lane_number;

        int8_t     helper;

        int8_t     state;

};


struct MessageLcm {

        int64_t    position[2];

        int32_t    type;

		SharedData data;
};



#endif   /* VEHICLE_DATA_H */
