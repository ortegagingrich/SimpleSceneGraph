#ifndef TICK_H
#define TICK_H

struct TickRecord {
	long calls;
	long last_time;
	int record_length;
	float *tpf_record;
};

typedef struct TickRecord TickRecord;


TickRecord *create_tick_record(int size);
void delete_tick_record(TickRecord *record);
float compute_tick_record_fps(TickRecord *record);

/*
 * Sleeps to attain the provided target_fps, if possible.  If the provided target
 * is zero or negative, it is taken to mean unlimited frame rate.  Returns the
 * time (in seconds) taken since the last call to tick.
 */
float tick(int target_fps, TickRecord *record);

#endif

