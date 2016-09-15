#include "app/votes.h"

#include <sdk_common.h>
#include <stdlib.h>

#include "app/feedback.h"
#include "app/storage.h"
#include "system/timer.h"
#include "system/util.h"

#define CLEAR_VOTE_BUFFER_DURATION_IN_MS 3000

static userVote buffer = { 0 };
static userVote bufferForStorage __attribute__((aligned(4))) = { 0 };

SYS_TIMER_DEF(clearBufferTimer);

void votes_initialize() {
  create_single_shot_timer(&clearBufferTimer);
}

static bool save_and_clear_vote_buffer_content();
static void start_clear_buffer_timeout() {
  start_timer(&clearBufferTimer, CLEAR_VOTE_BUFFER_DURATION_IN_MS, save_and_clear_vote_buffer_content);
}

static void stop_clear_buffer_timeout() {
  stop_timer(&clearBufferTimer);
}

static bool local_vote_buffer_is_empty() {
  return buffer.voterId == 0 && buffer.hitCount == 0;
}

static bool save_and_clear_vote_buffer_content() {
  if (local_vote_buffer_is_empty()) return true;

  if (get_vote_count() > MAX_VOTE_COUNT) {
    start_clear_buffer_timeout();
    return false;
  }

  memcpy(&bufferForStorage, &buffer, sizeof(buffer));
  set_data_in_storage(&bufferForStorage, sizeof(bufferForStorage), VOTES_STORAGE_FILE_ID, bufferForStorage.voterId);
  memset(&buffer, 0, sizeof(buffer));

  return true;
}

void save_vote(uint16_t voterId) {
  stop_clear_buffer_timeout();

  if (buffer.voterId == voterId) {
    buffer.hitCount++;
  } else {
    if (!save_and_clear_vote_buffer_content()) return;

    buffer.voterId = voterId;
    buffer.hitCount = 1;
    get_vote_configuration(&buffer.config);

    display_vote_recorded_feedback();
  }
  rtc_get_timestamp(buffer.timeOfLastVote);

  start_clear_buffer_timeout();
}

void save_vote_from_command_line(char** parsedCommandArray, uint8_t numCommands) {
  SYS_UNUSED_PARAMETER(numCommands);
  uint16_t voterId = (uint16_t) atoi(parsedCommandArray[1]);
  save_vote(voterId);
}

uint16_t get_vote_count() {
  uint16_t storedVotes = get_record_count_in_storage(VOTES_STORAGE_FILE_ID);
  return (uint16_t) (storedVotes + (local_vote_buffer_is_empty() ? 0 : 1));
}

bool vote_storage_is_full() {
  return get_vote_count() > MAX_VOTE_COUNT;
}