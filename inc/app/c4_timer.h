void display_user_feedback_handler(void *context);

void turn_off_buzzer_feedback_handler(void *context);

void nfc_event_handler(void *context);

void turn_on_buzzer_feedback_timeout();

void turn_on_user_feedback();

void clear_vote_buffer_timer_handler(void *context);

void turn_on_buffer_clear_timeout();

void turn_off_buffer_clear_timeout();

void send_vote_timer_handler(void *context);

void turn_on_send_vote_timeout();

void turn_off_send_vote_timeout();

void turn_on_nfc_reader();

void clock_timer_handler(void *context);

void c4_timer_initialize(void);
