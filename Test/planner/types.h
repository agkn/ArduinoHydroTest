#ifndef PLANNER_TYPES_H_
#define PLANNER_TYPES_H_


// time in seconds since the day begin.
typedef uint32_t time_t;
const time_t NOT_A_TIME = 0xffff;



// 4 bit each: id + type, end + start
typedef struct {
	uint8_t mType;
	uint8_t mEnds;

	uint8_t startTimePointIdx() {
		return mEnds & 0x0f;
	}

	uint8_t endTimePointIdx() {
		return mEnds >> 4;
	}

	uint8_t type() {
		return mType & 0x0f;
	}

	uint8_t id() {
		return mType >> 4;
	}
} store_range_t;


#endif /* PLANNER_TYPES_H_ */

