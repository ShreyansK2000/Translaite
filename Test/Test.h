#ifndef TEST_H__
#define TEST_H__

enum test_result {Failed = 0, Passed = 1};

/**
 * Unit test for parse_register_buffer
 */ 
int test_parse_register(void);

/**
 * Unit test for parse_login_buffer
 */ 
int test_parse_login(void);

/**
 * Unit test for parse_add_history_buffer
 */ 
int test_parse_add_history(void);

/**
 * Unit test for parse_remove_history_buffer
 */ 
int test_parse_remove_history(void);

/**
 * Unit test for parse_open_sock_buffer
 */ 
int test_parse_open_sock(void);

/**
 * Unit test for parse_location_buffer
 */ 
int test_parse_location(void);

/**
 * Unit test for get_Q_to_Q
 */ 
int test_parse_Q_to_Q(void);

/**
 * Unit test for get_Q_to_Q
 */ 
int test_parse_CB_to_CB(void);

/**
 * Unit test for get_Q_to_Q
 */ 
int test_check_empty_array(void);

/**
 * Test for translation response parsing
 */ 
int test_parse_translation(void);

/**
 * Test for history response parsing
 */ 
int test_parse_history(void);

#endif /* TEST_H__ */
