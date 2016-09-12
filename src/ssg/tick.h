#ifndef TICK_H
#define TICK_H

#include "shared_exports.h"


namespace ssg {

	struct SHARED_EXPORT TickRecord {
		long calls;
		long last_time;
		int record_length;
		float *tpf_record;
	};

	typedef struct TickRecord TickRecord;


	SHARED_EXPORT TickRecord *create_tick_record(int size);
	SHARED_EXPORT void delete_tick_record(TickRecord *record);
	SHARED_EXPORT float compute_tick_record_fps(TickRecord *record);

	/*
	 * Sleeps to attain the provided target_fps, if possible.  If the provided target
	 * is zero or negative, it is taken to mean unlimited frame rate.  Returns the
	 * time (in seconds) taken since the last call to tick.
	 */
	SHARED_EXPORT float tick(int target_fps, TickRecord *record);


}

#endif

