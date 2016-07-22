#include <stdlib.h>
#include <time.h>

#include "tick.h"


static TickRecord *MASTER_RECORD = NULL;




static long time_ms();

#ifdef _WIN32
#include <windows.h>
static long time_ms(){
	return timeGetTime();
}
#else
#include <sys/time.h>
#include <unistd.h>
static long time_ms(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (long) tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
#endif




TickRecord *create_tick_record(int size){
	TickRecord *record = (TickRecord*) malloc(sizeof(TickRecord));
	record->tpf_record = (float*) malloc(size * sizeof(float));
	int i;
	for(i = 0; i < size; i++){
		record->tpf_record[i] = -1.0f;
	}
	
	record->calls = 0;
	record->record_length = size;
	record->last_time = time_ms();
	
	return record;
}


void delete_tick_record(TickRecord *record){
	if(record != NULL){
		free(record->tpf_record);
		free(record);
	}
}


float compute_tick_record_fps(TickRecord *record){
	if(record == NULL){
		if(MASTER_RECORD == NULL){
			return 0.0f;
		}
		record = MASTER_RECORD;
	}
	
	float time_sum = 0;
	int nonzeros = 0;
	int i;
	for(i = 0; i < record->record_length; i++){
		float val = record->tpf_record[i];
		if(val > 0){
			time_sum += record->tpf_record[i];
			nonzeros++;
		}
	}
	
	if(time_sum > 0){
		return (float) nonzeros / time_sum;
	}else{
		return 0.0f;
	}
}


float tick(int target_fps, TickRecord *record){
	if(record == NULL){
		if(MASTER_RECORD == NULL){
			MASTER_RECORD = create_tick_record(10);
		}
		record = MASTER_RECORD;
	}
	
	long current_time = time_ms();
	float target_tpf;
	if(target_fps < 1){
		target_tpf = 0.0;
	}else{
		target_tpf = 1.0 / (float) target_fps;
	}
	float tpf = (float) (current_time - record->last_time) / 1000.0f;
	
	float diff = target_tpf - tpf;
	if(diff > 0){
#ifdef __linux__
		usleep((int) (diff * 1000 * 1000));
#endif
#ifdef _WIN32
		Sleep((int) (diff * 1000));
#endif
		current_time = time_ms();
		tpf = (float) (current_time - record->last_time) / 1000.0f;
	}
	
	record->last_time = current_time;
	record->tpf_record[record->calls % record->record_length] = tpf;
	record->calls++;
	
	return tpf;
}


