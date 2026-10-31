-- query for showing test case table properly
SELECT
    test_case_id,
    problem_id,
    REPLACE(REPLACE(`input`, '\r', '\\r'), '\n', '\\n') AS input,
    REPLACE(REPLACE(expected_output, '\r', '\\r'), '\n', '\\n') AS expected_output,
    is_sample,
    order_no
FROM test_cases
ORDER BY problem_id, order_no;

-- query for showing execution results table properly
SELECT
    result_id,
    submission_id,
    test_case_id,
    status,
    REPLACE(REPLACE(actual_output, '\r', '\\r'), '\n', '\\n') AS actual_output,
    time_taken,
    memory_used
FROM execution_results
ORDER BY result_id;





USE growcode_app;

CREATE TABLE discussions (
    discussion_id INT AUTO_INCREMENT PRIMARY KEY,
    problem_id INT NOT NULL,
    user_id INT NOT NULL,
    title VARCHAR(200) NOT NULL,
    content TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (problem_id) REFERENCES problems(problem_id) ON DELETE CASCADE,
    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE,
    INDEX idx_discussions_problem (problem_id),
    INDEX idx_discussions_user (user_id)
);

CREATE TABLE discussion_replies (
    reply_id INT AUTO_INCREMENT PRIMARY KEY,
    discussion_id INT NOT NULL,
    user_id INT NOT NULL,
    content TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (discussion_id) REFERENCES discussions(discussion_id) ON DELETE CASCADE,
    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE,
    INDEX idx_replies_discussion (discussion_id)
);